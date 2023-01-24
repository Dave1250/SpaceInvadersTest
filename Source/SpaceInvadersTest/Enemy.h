// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.h"
#include "Projectile.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

class AEarth;
UCLASS()
class SPACEINVADERSTEST_API AEnemy
		: public AActor
		, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMoveRotation(float MoveRotation);

	void SetEarth(AEarth* Earth);

	void SetCurrentTier(int32 Tier);

	void Shoot();

	bool CanShoot();

	void OnDamageReceived_Implementation() override;

	int32 GetPointsForHit_Implementation() override;
	
protected:

	void MoveToNextTier();

protected:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	UStaticMeshComponent* StaticMesh;

	/** Projectile type used by enemy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Projectile")
	TSubclassOf<AProjectile> Projectile;

	/** Shoot cds in second, takes from range after every shoot*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Projectile")
	FVector2D ShootCD;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	float RotationSpeed;

	/** Amount of points that adds to player when destroyed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	int32 PointsToAdd;
	
	float m_MoveRotation;

	int m_CurrentTier;

	bool m_bIsMovingRight;

	float m_MovementPassed;

	float m_ShootTimer;

	UPROPERTY()
	AEarth* m_pEarth;
};


