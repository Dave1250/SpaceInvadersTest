// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DamageInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UDamageInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SPACEINVADERSTEST_API IDamageInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/** Called when damage is going to be receive (during projectile hit) */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Damage)
	void OnDamageReceived();
	virtual void OnDamageReceived_Implementation();


	/** Returns amount of points that player will receive for hitting */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category=Damage)
	int32 GetPointsForHit();
	virtual int32 GetPointsForHit_Implementation();
};
