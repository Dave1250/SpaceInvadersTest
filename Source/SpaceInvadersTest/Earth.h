// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AsteroidBase.h"
#include "BossEnemy.h"
#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Earth.generated.h"


UCLASS()
class SPACEINVADERSTEST_API AEarth : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEarth();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float GetDistanceOnTier(int32 Tier);

	void EarthDamageReceive(int32 DamageAmount);

	void RemoveEnemy(AEnemy* InEnemy);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category=Earth)
	int32 GetHP();
	
protected:
	
	void SpawnEnemies();

	void SpawnAsteroids();

	void SpawnSpecialEnemy();
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Enemy")
	TSubclassOf<AEnemy> Enemy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Enemy")
	int32 AmountOfTiers;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Enemy")
	int32 AmountOfEnemiesPerTier;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Enemy")
	TArray<float> DistancesOnTier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Enemy")
	TSubclassOf<ABossEnemy> SpecialEnemy;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Enemy")
	FVector2D SpecialEnemyRespawnTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Enemy")
	float DistanceToSpawnSpecialEnemyAt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Asteroids")
	TArray<TSubclassOf<AAsteroidBase>> Asteroids;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Asteroids")
	FVector2D RandomSpawnTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Asteroids")
	FVector2D AsteroidsPerSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game|Asteroids")
	float DistanceToSpawnAsteroidsAt;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Game)
	int32 Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY()
	TArray<AEnemy*> m_Enemies;

	UPROPERTY()
	ABossEnemy* pBoss;
	
	float m_AsteroidTimer;

	float m_SpecialEnemyTimer;
	
};

