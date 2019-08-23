// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "Pickup_Coin.generated.h"

/**
 * 
 */
UCLASS()
class MASTERINGUNREAL_API APickup_Coin : public APickup
{
	GENERATED_BODY()
	
public:
	APickup_Coin();

	//부모 Pickup클래스에서 정의안 CollectPickup_Implementation 오버라이드
	virtual void CollectPickup_Implementation(ABaseCharacter* Character) override;
};
