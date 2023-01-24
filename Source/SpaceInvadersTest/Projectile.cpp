// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "DamageInterface.h"
#include "Enemy.h"
#include "PlayerPawn.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
	
	MovementSpeed = 300.f;
	CanDamageEnemies = true;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(GetActorLocation() == TargetLocation)
	{
		Destroy();
		SetActorTickEnabled(false);
		return;
	}
	FVector vec = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), TargetLocation, DeltaTime, MovementSpeed);
	SetActorLocation(vec);
}

void AProjectile::SetTargetLocation(FVector& InLocation)
{
	TargetLocation = InLocation;
	SetActorTickEnabled(true);
}

void AProjectile::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(!IsValid(OtherActor) || !OtherActor->Implements<UDamageInterface>())
	{
		return;
	}
	
	if(!CanDamageEnemies)
	{
		AEnemy* pEnemy = Cast<AEnemy>(OtherActor);
		if(IsValid(pEnemy))
		{
			return;
		}
	}
	
	/*Gets owning player */
	APlayerPawn* pPlayer = Cast<APlayerPawn>(GetOwner());
	if(IsValid(pPlayer))
	{
		pPlayer->AddPoints(IDamageInterface::Execute_GetPointsForHit(OtherActor));
		
	}
	
	IDamageInterface::Execute_OnDamageReceived(OtherActor);
	Destroy();
}

void AProjectile::OnDamageReceived_Implementation()
{
	Destroy();
}

