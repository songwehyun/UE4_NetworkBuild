// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlatformerPlayerState.generated.h"

/**
 * 
 */

/*PlayerState는 클라이언트와 서버에 존재한다. 
연결된 모든 클라이언트의 레플리케이트된 플레이어의 특정한 정보들을 가지고 있다
가지고 있는 정보들이란 현재 체력, 수집된 코인 갯수 등이 있다.*/


UCLASS()
class MASTERINGUNREAL_API APlatformerPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	APlatformerPlayerState();
	
	UFUNCTION(BlueprintCallable, Category = "Player State")
		void CollectCoin();
	UFUNCTION(BlueprintCallable, Category = "Player State")
		void CollectHeart();
protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Platformer Player State")
		int NumberOfCoinCollected;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Platformer Player State")
		int CurrentHealth;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Platformer Player State")
		int NumberOfDeaths;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Player State")
		int MaxHealth;
};
