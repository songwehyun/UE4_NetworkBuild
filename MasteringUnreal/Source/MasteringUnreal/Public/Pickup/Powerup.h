// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/Pickup.h"
#include "Powerup.generated.h"

/**
 * 
 */
UCLASS()
class MASTERINGUNREAL_API APowerup : public APickup
{
	GENERATED_BODY()
public:
	APowerup();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Power Up")
		UTexture2D* icon;

	virtual void CollectPickup_Implementation(ABaseCharacter* Character) override;

	//powerup을 사용하기 위한 함수
	virtual void UsePowerup(ABaseCharacter* Character, FVector LaunchLocation, FVector LaunchDirection);
};
