// Fill out your copyright notice in the Description page of Project Settings.


#include "Earth.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AEarth::AEarth()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
	
	DistancesOnTier.Add(150.f);
	DistancesOnTier.Add(300.f); 
	DistancesOnTier.Add(450.f);
	DistancesOnTier.Add(600.f);
	DistancesOnTier.Add(750.f);

	AmountOfEnemiesPerTier = 10;
	RandomSpawnTime = FVector2D(1, 5);
	AsteroidsPerSpawn = FVector2D(1, 3);
	m_AsteroidTimer = 2.f;
	AmountOfTiers = 4;

	DistanceToSpawnSpecialEnemyAt = 3000.f;
	DistanceToSpawnAsteroidsAt = 6000.f;
	pBoss = nullptr;
	SpecialEnemyRespawnTimer = FVector2D(15, 30);
}

// Called when the game starts or when spawned
void AEarth::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector::ZeroVector);

	checkf(AmountOfTiers <= DistancesOnTier.Num(), TEXT("To small DistancesOnTier amounts"));

	m_SpecialEnemyTimer = GetWorld()->GetTimeSeconds() + UKismetMathLibrary::RandomIntegerInRange(SpecialEnemyRespawnTimer.X, SpecialEnemyRespawnTimer.Y);
	SpawnEnemies();
}

// Called every frame
void AEarth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SpawnAsteroids();
	SpawnSpecialEnemy();
}

float AEarth::GetDistanceOnTier(int32 Tier)
{
	if(DistancesOnTier.Num() <= Tier || Tier < 0)
	{
		return -1.f;
	}
	return DistancesOnTier[Tier];
}

void AEarth::EarthDamageReceive(int32 DamageAmount)
{
	Health -= DamageAmount;
	if(Health <= 0)
	{
		UGameplayStatics::OpenLevel(this, "MainMenu");
	}
}

void AEarth::SpawnEnemies()
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	/** Spawning enemies on last tiers */
	for(int32 index = DistancesOnTier.Num()-1; index >= DistancesOnTier.Num()-AmountOfTiers; index--)
	{
		FVector vec = FVector::ZeroVector;
		vec.X = GetDistanceOnTier(index);
		for(int32 number = 1; number <= AmountOfEnemiesPerTier; number++)
		{
			AEnemy* pEnemy = GetWorld()->SpawnActor<AEnemy>(Enemy, vec, vec.Rotation(), params);
			if(IsValid(pEnemy))
			{

				/** Setting enemies location using that Earth is on loc (0,0,0), so we can just rotate vectors to move them on circle */
				FVector2D rotateVec{pEnemy->GetActorLocation().X, pEnemy->GetActorLocation().Y};
				float rotation = number * 360/AmountOfEnemiesPerTier;
				rotateVec = UKismetMathLibrary::GetRotated2D(rotateVec, rotation);
				FVector newLocation{rotateVec.X, rotateVec.Y, 0.f};
				pEnemy->SetActorLocation(newLocation);
				pEnemy->SetActorRotation(newLocation.Rotation());
				pEnemy->SetEarth(this);
				pEnemy->SetMoveRotation(180/AmountOfEnemiesPerTier);
				pEnemy->SetCurrentTier(index);
				m_Enemies.Add(pEnemy);
			}
		}
	}
}

void AEarth::SpawnAsteroids()
{
	if(GetWorld()->GetTimeSeconds() >= m_AsteroidTimer)
	{
		if(Asteroids.Num() == 0)
		{
			return;
		}
		
		m_AsteroidTimer = GetWorld()->GetTimeSeconds() + FMath::RandRange(RandomSpawnTime.X, RandomSpawnTime.Y);
		
		int32 amount = UKismetMathLibrary::RandomIntegerInRange(AsteroidsPerSpawn.X, AsteroidsPerSpawn.Y);

		/** spawning random amount of random asteroids types*/
		for(int32 index = 0; index < amount; index++)
		{
			int32 num = UKismetMathLibrary::RandomInteger(Asteroids.Num());
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AAsteroidBase* pAsteroid =	GetWorld()->SpawnActor<AAsteroidBase>(Asteroids[num], FVector(10000.f, 10000.f, -1000.f), FRotator(0,0,0), params);
			if(IsValid(pAsteroid))
			{
				pAsteroid->SetDistanceToSpawn(DistanceToSpawnAsteroidsAt);
				FVector target = pAsteroid->GetRandomVector();
				FVector start = pAsteroid->GetRandomVectorOnDifferentAxis(target);
				pAsteroid->SetTargetLocation(target);
				pAsteroid->SetStartLocation(start);
				pAsteroid->SetActorLocation(start);
			}
		}
	}
}

void AEarth::SpawnSpecialEnemy()
{
	if(IsValid(pBoss) || GetWorld()->GetTimeSeconds() >= m_SpecialEnemyTimer || !IsValid(SpecialEnemy))
	{
		return;
	}

	FVector spawnVec = UKismetMathLibrary::RandomUnitVector() * DistanceToSpawnSpecialEnemyAt;
	spawnVec.Z = 0.f;
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	pBoss = GetWorld()->SpawnActor<ABossEnemy>(SpecialEnemy, spawnVec, FRotator(0,0,0), params);
	if(IsValid(pBoss))
	{
		pBoss->SetEarth(this);
		m_SpecialEnemyTimer = GetWorld()->GetTimeSeconds() + UKismetMathLibrary::RandomIntegerInRange(SpecialEnemyRespawnTimer.X, SpecialEnemyRespawnTimer.Y);
	}
}

void AEarth::RemoveEnemy(AEnemy* InEnemy)
{
	m_Enemies.Remove(InEnemy);
	if(m_Enemies.Num() == 0)
	{
		SpawnEnemies();
	}
}

int32 AEarth::GetHP()
{
	return Health;
}



