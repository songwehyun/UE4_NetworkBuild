// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerPlayerState.h"
#include "UnrealNetwork.h"

APlatformerPlayerState::APlatformerPlayerState()
{
	NumberOfCoinCollected = 0;
	MaxHealth = 3;
	CurrentHealth = MaxHealth;
	NumberOfDeaths = 0;
	SelectedPowerup = nullptr;
}

/*
UPROPERTY�� Replicated��ũ�θ� �߰��ϴ� �͸����δ� �����ϰ� ���ø�����Ʈ�Ǿ��ٰ� �� �� ����.
������Ƽ ���ø����̼��� �ϼ��ϱ� ���ؼ���,
GetLifetimeREplicatedProps�Լ��� �����ؾ��Ѵ�.
�̸� ����, UnrealNetwork.h������ ��Ŭ��� �ؾ��Ѵ�.

*/
void APlatformerPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlatformerPlayerState, CurrentHealth);
	DOREPLIFETIME(APlatformerPlayerState, NumberOfCoinCollected);
	DOREPLIFETIME(APlatformerPlayerState, NumberOfDeaths);
	DOREPLIFETIME(APlatformerPlayerState, SelectedPowerup);
}

void  APlatformerPlayerState::ReceiveDamage(int amount)
{
	CurrentHealth -= amount;

	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
}

void APlatformerPlayerState::CollectCoin()
{
	NumberOfCoinCollected += 1;
}

void APlatformerPlayerState::CollectHeart()
{
	CurrentHealth += 1;

	CurrentHealth = FMath::Clamp(CurrentHealth, 0, MaxHealth);
}