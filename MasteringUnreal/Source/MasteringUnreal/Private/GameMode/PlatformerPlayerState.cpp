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

	bIsReady = false;
	bSpectator = false;
	SelectedCharacterIndex = 0;
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
	DOREPLIFETIME(APlatformerPlayerState, bIsReady);
	DOREPLIFETIME(APlatformerPlayerState, bSpectator);
	DOREPLIFETIME(APlatformerPlayerState, SelectedCharacterIndex);
}

void APlatformerPlayerState::ToggleReady_Implementation()
{
	bIsReady = !bIsReady;
}

bool APlatformerPlayerState::ToggleReady_Validate() 
{
	return true;
}

void APlatformerPlayerState::ToggleSpectator_Implementation()
{
	bSpectator = !bSpectator;
}

bool APlatformerPlayerState::ToggleSpectator_Validate() 
{
	return true;
}

void APlatformerPlayerState::SelectCharacter_Implementation(int index)
{
	SelectedCharacterIndex = index;
}

bool APlatformerPlayerState::SelectCharacter_Validate(int index) 
{
	return true;
}


int APlatformerPlayerState::GetCurrentHealth()
{
	return CurrentHealth;
}
void APlatformerPlayerState::PlayerRespawnedAfterDeath_Implementation()
{
	if (Role == ROLE_Authority)
	{
		CurrentHealth = MaxHealth;

		NumberOfCoinCollected = NumberOfCoinCollected / 2;
		SelectedPowerup = nullptr;
		NumberOfDeaths++;
	}
}

bool APlatformerPlayerState::PlayerRespawnedAfterDeath_Validate()
{
	return true;
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