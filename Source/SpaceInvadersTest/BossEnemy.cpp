// Fill out your copyright notice in the Description page of Project Settings.


#include "BossEnemy.h"

#include "Earth.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ABossEnemy::ABossEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Health = 5;
	RotationSpeed = 20.f;
	MovementSpeed = 100.f;
	Damage = 10;
	
}

// Called when the game starts or when spawned
void ABossEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABossEnemy::Tick(float DeltaTime)
{
	Super::Super::Tick(DeltaTime);
	FVector2D vec{GetActorLocation().X, GetActorLocation().Y};
	vec = UKismetMathLibrary::GetRotated2D(vec, RotationSpeed * DeltaTime);
	FVector newLocation{vec.X, vec.Y, 0.f};
	newLocation = UKismetMathLibrary::VInterpTo_Constant(newLocation, FVector(0,0,0), DeltaTime, MovementSpeed);
	SetActorLocation(newLocation);
	SetActorRotation(newLocation.Rotation());
	float dist = UKismetMathLibrary::Vector_Distance(newLocation, FVector(0,0,0));
	if(IsValid(m_pEarth) && dist <= m_pEarth->GetDistanceOnTier(0))
	{
		m_pEarth->EarthDamageReceive(Damage);
		Destroy();
	}
}

void ABossEnemy::OnDamageReceived_Implementation()
{
	Health -= 1;
	if(Health <= 0)
	{
		Destroy();
	}
}


