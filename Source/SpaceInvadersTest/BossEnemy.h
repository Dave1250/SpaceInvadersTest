// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "GameFramework/Actor.h"
#include "BossEnemy.generated.h"

UCLASS()
class SPACEINVADERSTEST_API ABossEnemy : public AEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossEnemy();
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	void OnDamageReceived_Implementation() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	int32 Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	int32 Damage;

	/** Defines how fast enemy get close to Earth */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	float MovementSpeed;
};
