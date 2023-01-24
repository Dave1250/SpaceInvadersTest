// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.h"
#include "GameFramework/Actor.h"
#include "AsteroidBase.generated.h"

UCLASS()
class SPACEINVADERSTEST_API AAsteroidBase
		: public AActor
		, public IDamageInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAsteroidBase();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnDamageReceived_Implementation() override;

	int32 GetPointsForHit_Implementation() override;

	void SetTargetLocation(FVector& Location);

	void SetStartLocation(FVector& Location);

	void SetDistanceToSpawn(float InDist);

	FVector GetRandomVector();
	
	FVector GetRandomVectorOnDifferentAxis(const FVector Vector);
	
	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	TSubclassOf<AAsteroidBase> SmallerAsteroid;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	int32 EarthHPToRemove;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	float MovementSpeed;
	
	/** Amount of points that adds to player when destroyed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	int32 PointsToAdd;

	float m_DistanceToSpawn;
	
	FVector m_Target;

	FVector m_StartLocation;
};



