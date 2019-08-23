// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlatformerPlayerState.generated.h"

/**
 * 
 */

/*PlayerState�� Ŭ���̾�Ʈ�� ������ �����Ѵ�. 
����� ��� Ŭ���̾�Ʈ�� ���ø�����Ʈ�� �÷��̾��� Ư���� �������� ������ �ִ�
������ �ִ� �������̶� ���� ü��, ������ ���� ���� ���� �ִ�.*/


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
