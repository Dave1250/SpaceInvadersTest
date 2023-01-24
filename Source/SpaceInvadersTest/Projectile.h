// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInterface.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class SPACEINVADERSTEST_API AProjectile
		: public AActor
		, public IDamageInterface
		
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetTargetLocation(FVector& InLocation);

	UFUNCTION()
	void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	void OnDamageReceived_Implementation() override;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Projectile)
	float MovementSpeed;

	/** Decides if can destroy enemies and add points to player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Projectile)
	bool CanDamageEnemies;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	UStaticMeshComponent* StaticMesh;
	
	FVector TargetLocation;
};
