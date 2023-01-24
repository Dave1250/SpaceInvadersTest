// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroidBase.h"

#include "Earth.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AAsteroidBase::AAsteroidBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
	
	MovementSpeed = 200.f;
	EarthHPToRemove = 1.f;
	PointsToAdd = 1;
}

// Called when the game starts or when spawned
void AAsteroidBase::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AAsteroidBase::OnBeginOverlap);
}

// Called every frame
void AAsteroidBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector vec = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), m_Target, DeltaTime, MovementSpeed);
	SetActorLocation(vec);
	
	/** When reach target, revesre it and set as actor location, to show it at other side of screen */
	if(vec.Equals(m_Target, 50.f))
	{
		FVector reversed = -m_Target;
		FVector loc = -m_StartLocation;
		SetTargetLocation(loc);
		SetActorLocation(reversed);
	}
}

void AAsteroidBase::OnDamageReceived_Implementation()
{
	IDamageInterface::OnDamageReceived_Implementation();

	/** Gets smaller asteroid type and spawns 2 of them*/
	if(IsValid(SmallerAsteroid))
	{
		for(int32 index = 0; index < 2; index++)
		{
			
			AAsteroidBase* pAsteroid = GetWorld()->SpawnActor<AAsteroidBase>(SmallerAsteroid, GetActorLocation(), FRotator(0,0,0));
			if(IsValid(pAsteroid))
			{
				FVector start = GetRandomVector();
				FVector end = GetRandomVectorOnDifferentAxis(start);
				pAsteroid->SetTargetLocation(end);
				pAsteroid->SetStartLocation(start);
				pAsteroid->SetDistanceToSpawn(m_DistanceToSpawn);
			}
		}
	}
	Destroy();
}

int32 AAsteroidBase::GetPointsForHit_Implementation()
{
	return PointsToAdd;
}

void AAsteroidBase::SetTargetLocation(FVector& Location)
{
	m_Target = Location;
}

void AAsteroidBase::SetStartLocation(FVector& Location)
{
	m_StartLocation = Location;
}

void AAsteroidBase::SetDistanceToSpawn(float InDist)
{
	m_DistanceToSpawn = InDist;
}

FVector AAsteroidBase::GetRandomVector()
{
	bool bIsXFirst = UKismetMathLibrary::RandomBool();
	float x = UKismetMathLibrary::RandomFloatInRange(-m_DistanceToSpawn, m_DistanceToSpawn);
	float y;
	bool bRan = UKismetMathLibrary::RandomBool();
	
	/** Decides if value is on + or -  */
	if(bRan)
	{
		y = m_DistanceToSpawn;
	}
	else
	{
		y = -m_DistanceToSpawn;
	}
	
	FVector vec{0.f,0.f,0.f};
	
	/** Decides if 4k value is on X or Y axis */
	if (bIsXFirst)
	{
		vec.X = x;
		vec.Y = y;
	}
	else
	{
		vec.X = y;
		vec.Y = x;
	}
	return vec;
}

FVector AAsteroidBase::GetRandomVectorOnDifferentAxis(const FVector Vector)
{
	float x = 0.f;
	float y = 0.f;
	
	bool bRan = UKismetMathLibrary::RandomBool();
	
	/** Decides if value is on + or -  */
	if(Vector.X == m_DistanceToSpawn)
	{
		x = UKismetMathLibrary::RandomFloatInRange(-m_DistanceToSpawn, (m_DistanceToSpawn * 0.75f));
	}
	if(Vector.X == -m_DistanceToSpawn)
	{
		 x = UKismetMathLibrary::RandomFloatInRange((-m_DistanceToSpawn* 0.75f), m_DistanceToSpawn);
	}
	y = bRan ? m_DistanceToSpawn : -m_DistanceToSpawn;
	
	FVector vec(x, y, 0.f);
	
	/** if x was assigned return vector*/
	if(x != 0)
	{
		return vec;
	}
	
	x = bRan ? m_DistanceToSpawn : -m_DistanceToSpawn;
	
	if(Vector.Y == m_DistanceToSpawn)
	{
		y = UKismetMathLibrary::RandomFloatInRange(-m_DistanceToSpawn, (m_DistanceToSpawn * 0.75f));
	}
	if(Vector.Y == -m_DistanceToSpawn)
	{
		y = UKismetMathLibrary::RandomFloatInRange((-m_DistanceToSpawn* 0.75f), m_DistanceToSpawn);
	}

	vec.X = x;
	vec.Y = y;
	return vec;
}


void AAsteroidBase::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(!IsValid(OtherActor))
	{
		return;
	}

	AEarth* pEarth = Cast<AEarth>(OtherActor);
	if(!IsValid(pEarth))
	{
		return;
	}

	pEarth->EarthDamageReceive(EarthHPToRemove);
	Destroy();
}

