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
UPROPERTY에 Replicated매크로를 추가하는 것만으로는 적절하게 레플리케이트되었다고 할 수 없다.
프로퍼티 레플리케이션을 완성하기 위해서는,
GetLifetimeREplicatedProps함수를 구현해야한다.
이를 위해, UnrealNetwork.h파일을 인클루드 해야한다.

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