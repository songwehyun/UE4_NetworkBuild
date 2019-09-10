// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerGameState.h"
#include "PlatformerGameInstance.h"
#include "PlatformerPlayerState.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"


APlatformerGameState::APlatformerGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentWidget = nullptr;
	CurrentMatchState = EMatchState::ENotConnected;
}

void APlatformerGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlatformerGameState, CurrentMatchState);

}

void APlatformerGameState::ChangeState_Implementation(EMatchState newState)
{
	if (CurrentMatchState != newState) {
		LeaveState();
		EnterState(newState);
	}
}

EMatchState APlatformerGameState::GetMatchState()
{
	return CurrentMatchState;
}

void APlatformerGameState::EnterState(EMatchState newState)
{
	CurrentMatchState = newState;

	//Change state on player
	APlatformerPlayerState* CurrentPlayerState = Cast<APlatformerPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);

	if (CurrentPlayerState)
	{
		CurrentPlayerState->CurrentLocalMatchState = CurrentMatchState;
	}

	//switch on new state
	switch (CurrentMatchState) 
	{
	case EMatchState::ENotConnected: 
	{
		break;
	}
	case EMatchState::EWaitingForPlayers: 
	{
		//Show waiting for players UI
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cWaitingForPlayers);
		CurrentWidget->AddToViewport();

		//change input mode
		UPlatformerGameInstance *GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

		if (GameInstance) 
		{
			GameInstance->SetInputMode(EInputMode::EUIOnly, true);
		}
		break;
	}
	case EMatchState::EStartingGame: 
	{
		if (Role == ROLE_Authority)
		{
			ChangeState(EMatchState::EGameInProgress);
		}
		break;
	}
	case EMatchState::EGameInProgress: 
	{
		UPlatformerGameInstance* GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

		if (GameInstance)
		{
			GameInstance->SetInputMode(EInputMode::EGameOnly, false);
		}
		break;
	}
	case EMatchState::EGameComplete: 
	{
		break;
	}
	case EMatchState::ERestartingGame: 
	{
		break;
	}
	}
}

void APlatformerGameState::LeaveState()
{
	//switch on new state
	switch (CurrentMatchState) 
	{
	case EMatchState::ENotConnected: 
	{
		//		break;
	}
	case EMatchState::EWaitingForPlayers: 
	{
		//		break;
	}
	case EMatchState::EStartingGame: 
	{
		//		break;
	}
	case EMatchState::EGameInProgress: 
	{
		//		break;
	}
	case EMatchState::EGameComplete: 
	{
		//		break;
	}
	case EMatchState::ERestartingGame: 
	{
		//hide any widget
		if (CurrentWidget) {
			CurrentWidget->RemoveFromViewport();
			CurrentWidget = nullptr;
		}
		break;
	}
	}
}
void APlatformerGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ensure we're not out of sync with server
	//get player state
	APlatformerPlayerState *PlayerState = Cast<APlatformerPlayerState>(GetWorld()->GetFirstPlayerController()->PlayerState);

	if (PlayerState) 
	{
		//if we're out of sync
		if (PlayerState->CurrentLocalMatchState != CurrentMatchState) 
		{
			LeaveState();
			EnterState(CurrentMatchState);
		}
	}
}
