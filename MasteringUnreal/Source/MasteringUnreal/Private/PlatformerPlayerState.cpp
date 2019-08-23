// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerPlayerState.h"
#include "UnrealNetwork.h"

APlatformerPlayerState::APlatformerPlayerState()
{
	NumberOfCoinCollected = 0;
	MaxHealth = 3;
	CurrentHealth = MaxHealth;
	NumberOfDeaths = 0;
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