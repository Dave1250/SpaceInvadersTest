// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"

#include "Earth.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
APlayerPawn::APlayerPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);

	ShootStartLocation = CreateDefaultSubobject<UArrowComponent>("ShootStartLocation");
	ShootStartLocation->SetupAttachment(RootComponent);

	ShootCooldown = 0.1f;
	m_ShootTimer = 0.f;
	m_Points = 0;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if(!IsValid(Earth))
	{
		UE_LOG(LogTemp, Warning, TEXT("Earth does not exist at player"));
	}
	FVector vec = FVector::ZeroVector;
	vec.X = Earth->GetDistanceOnTier(0);
	SetActorLocation(vec);
	SetActorRotation(vec.Rotation());
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveLeftRight);
	
	InputComponent->BindAction("Shoot", IE_Pressed, this, &APlayerPawn::Shoot);

	InputComponent->BindAction("Escape", IE_Pressed, this, &APlayerPawn::Pause).bExecuteWhenPaused = true;
}

void APlayerPawn::Pause()
{
	if(UGameplayStatics::IsGamePaused(this))
	{
		EffectOnGameUnpaused();
		UGameplayStatics::SetGamePaused(this, false);
	}
	else
	{
		EffectOnGamePaused();
		UGameplayStatics::SetGamePaused(this, true);
	}
}

void APlayerPawn::MoveLeftRight(float AxisValue)
{
	/** Cause Earth is (0,0,0) we can just rotate location vector */
	if(AxisValue != 0)
	{
		FVector2D vec{GetActorLocation().X, GetActorLocation().Y};
		vec = UKismetMathLibrary::GetRotated2D(vec, AxisValue);
		FVector newLocation{vec.X, vec.Y, 0.f};
		SetActorLocation(newLocation);
		SetActorRotation(newLocation.Rotation());
	}
}

void APlayerPawn::Shoot()
{
	if(!IsValid(Projectile))
	{
		return;
	}

	if(GetWorld()->GetTimeSeconds() <= m_ShootTimer)
	{
		return;
	}

	/** Spawn projecile and set it targets */
	
	FVector targetLocation = ShootStartLocation->GetForwardVector() * 10000.f;
	FActorSpawnParameters params;
	params.Owner = this;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector spawnLocation = ShootStartLocation->GetComponentLocation();
	FRotator spawnRotation{0.f,0.f,0.f};
	AProjectile* pProjectile = GetWorld()->SpawnActor<AProjectile>(Projectile, spawnLocation, spawnRotation, params);
	if(IsValid(pProjectile))
	{
		pProjectile->SetTargetLocation(targetLocation);
	}
	m_ShootTimer = GetWorld()->GetTimeSeconds() + ShootCooldown;
}

void APlayerPawn::EffectOnGamePaused_Implementation()
{
}

void APlayerPawn::EffectOnGameUnpaused_Implementation()
{
}

void APlayerPawn::AddPoints(int32 PointsToAdd)
{
	m_Points += PointsToAdd;
}

int32 APlayerPawn::GetPoints()
{
	return m_Points;
}

