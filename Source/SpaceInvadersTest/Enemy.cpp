// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Earth.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);

	m_bIsMovingRight = true;

	RotationSpeed = 1.f;
	ShootCD = FVector2D(15, 360);

	PointsToAdd = 2;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	m_ShootTimer = GetWorld()->GetTimeSeconds() + UKismetMathLibrary::RandomIntegerInRange(ShootCD.X, ShootCD.Y);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Shoot();

	/** Enemy is rotating in left/right side and after reaching destination points move to the closer tier */
	FVector2D vec{GetActorLocation().X, GetActorLocation().Y};
	if(m_bIsMovingRight)
	{
		vec = UKismetMathLibrary::GetRotated2D(vec, RotationSpeed * DeltaTime);
		m_MovementPassed += RotationSpeed * DeltaTime;
	}
	else
	{
		vec = UKismetMathLibrary::GetRotated2D(vec, -RotationSpeed * DeltaTime);
		m_MovementPassed += RotationSpeed * DeltaTime;
	}
	FVector newLocation{vec.X, vec.Y, 0.f};
	SetActorLocation(newLocation);
	SetActorRotation(newLocation.Rotation());
	if(m_MovementPassed >= m_MoveRotation)
	{
		MoveToNextTier();
	}
	
}

void AEnemy::SetMoveRotation(float MoveRotation)
{
	m_MoveRotation = MoveRotation;
}

void AEnemy::SetEarth(AEarth* Earth)
{
	m_pEarth = Earth;
}

void AEnemy::SetCurrentTier(int32 Tier)
{
	m_CurrentTier = Tier;
}

void AEnemy::Shoot()
{
	if(CanShoot() && IsValid(Projectile))
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AProjectile* pProjectile = GetWorld()->SpawnActor<AProjectile>(Projectile, GetActorLocation(), FRotator(0,0,0), params);
		if(IsValid(pProjectile))
		{
			FVector target{0,0,0};
			pProjectile->SetTargetLocation(target);
		}
		m_ShootTimer = GetWorld()->GetTimeSeconds() + UKismetMathLibrary::RandomIntegerInRange(ShootCD.X, ShootCD.Y);
	}
}

bool AEnemy::CanShoot()
{
	if(GetWorld()->GetTimeSeconds() < m_ShootTimer)
	{
		return false;
	}
	
	TArray<AActor*> actorsToIgnore;
	FHitResult hit;
	
	/** Check if is 1st in line to the earth (to avoid shooting at allies) */
	UKismetSystemLibrary::LineTraceSingle(this, GetActorLocation(), FVector(0,0,0), ETraceTypeQuery::TraceTypeQuery1,
		false, actorsToIgnore, EDrawDebugTrace::None, hit, true);
	if(IsValid(hit.GetActor()))
	{
		AEnemy* pEnemy = Cast<AEnemy>(hit.GetActor());
		if(IsValid(pEnemy))
		{
			return false;
		}
	}

	return true;
}

void AEnemy::OnDamageReceived_Implementation()
{
	if(IsValid(m_pEarth))
	{
		m_pEarth->RemoveEnemy(this);
		Destroy();
	}
}

int32 AEnemy::GetPointsForHit_Implementation()
{
	return PointsToAdd;
}


void AEnemy::MoveToNextTier()
{
	if(m_CurrentTier == 1)
	{
		m_pEarth->EarthDamageReceive(1);
		m_pEarth->RemoveEnemy(this);
		Destroy();
		return;
	}
	
	float newDistance = m_pEarth->GetDistanceOnTier(m_CurrentTier-1);
	FVector vec = UKismetMathLibrary::GetDirectionUnitVector(FVector(0,0,0), GetActorLocation()) * newDistance;
	SetActorLocation(vec); 
	SetCurrentTier(m_CurrentTier-1);
	
	m_MovementPassed = 0.f;
	
	if(m_bIsMovingRight)
	{
		m_bIsMovingRight = false;
	}
	else
	{
		m_bIsMovingRight = true;
	}
}

