// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "Components/Widget.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class AEarth;
UCLASS()
class SPACEINVADERSTEST_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void AddPoints(int32 PointsToAdd);
	
	UFUNCTION(BlueprintPure, Category=Points)
	int32 GetPoints();
	
protected:

	void Pause();

	void MoveLeftRight(float AxisValue);

	void Shoot();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Pause)
	void EffectOnGamePaused();
	virtual void EffectOnGamePaused_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Pause)
	void EffectOnGameUnpaused();
	virtual void EffectOnGameUnpaused_Implementation();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Game)
	AEarth* Earth;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Game)
	TSubclassOf<AProjectile> Projectile;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Game)
	float ShootCooldown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	UArrowComponent* ShootStartLocation;
	
	/** timer that allows to decide when player can shot*/
	float m_ShootTimer;


	/* points of player */
	int32 m_Points;
};
