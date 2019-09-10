// Fill out your copyright notice in the Description page of Project Settings.


#include "MasteringUnrealGameModeBase.h"
#include "PlatformerGameState.h"
#include "PlatformerGameInstance.h"

AMasteringUnrealGameModeBase::AMasteringUnrealGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AMasteringUnrealGameModeBase::Logout(AController* exiting)
{
	Super::Logout(exiting);

	APlatformerGameState* PlatGameState = Cast<APlatformerGameState>(GameState);

	if (PlatGameState)
	{
		//remove the player state from the lists of player;
		PlatGameState->RemovePlayerState(exiting->PlayerState);
	}
}

void AMasteringUnrealGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ensure we're on server
	if (Role == ROLE_Authority) 
	{
		//get current game state
		APlatformerGameState *PlatGameState = Cast<APlatformerGameState>(GameState);

		//get game instance
		UPlatformerGameInstance *GameInstance = Cast<UPlatformerGameInstance>(GetGameInstance());

		if (GameInstance && PlatGameState) 
		{
			//check we're in game
			if (GameInstance->GetGameState() == EGameState::EMultiplayerInGame) 
			{
				//check match state
				switch (PlatGameState->GetMatchState()) 
				{
				case EMatchState::ENotConnected: 
				{
					PlatGameState->ChangeState(EMatchState::EWaitingForPlayers);
					break;
				}
				}
			}
		}
	}
}