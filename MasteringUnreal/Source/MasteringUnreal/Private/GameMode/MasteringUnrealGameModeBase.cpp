// Fill out your copyright notice in the Description page of Project Settings.


#include "MasteringUnrealGameModeBase.h"
#include "PlatformerGameState.h"
#include "PlatformerGameInstance.h"
#include "PlatformerPlayerState.h"
#include "GameFramework/SpectatorPawn.h"

AMasteringUnrealGameModeBase::AMasteringUnrealGameModeBase()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	TimeSinceStart = 0.0f;
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
		APlatformerGameState* PlatGameState = Cast<APlatformerGameState>(GameState);

 		//get game instance
		UPlatformerGameInstance* GameInstance = Cast<UPlatformerGameInstance>(GetGameInstance());

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
				case EMatchState::EGameInProgress:
				{
					TimeSinceStart += DeltaTime;
					PlatGameState->UpdateMatchTime(TimeSinceStart);
				}
				default: 
				{
					//any other match state will reset timesince start to 0
					if (TimeSinceStart != 0.0f)
					{
						TimeSinceStart = 0.0f;
						PlatGameState->UpdateMatchTime(TimeSinceStart);
					}
				}
				}
			}
		}
	}
}

UClass* AMasteringUnrealGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	APlatformerGameState* PlatGameState = Cast<APlatformerGameState>(GameState);
	UPlatformerGameInstance* GameInstance = Cast<UPlatformerGameInstance>(GetGameInstance());

	if (PlatGameState && GameInstance)
	{
		//check whether we're in game
		if (GameInstance->GetGameState() == EGameState::EMultiplayerInGame)
		{
			//check whether the game is in progress or is starting
			if (PlatGameState->GetMatchState() == EMatchState::EGameInProgress ||
				PlatGameState->GetMatchState() == EMatchState::EStartingGame)
			{
				APlatformerPlayerState* PlayerState = Cast<APlatformerPlayerState>(InController->PlayerState);

				if (PlayerState)
				{
					//check if we're a spectator or that the player is not ready, meaning they joined after the game had started
					if (PlayerState->bSpectator || !PlayerState->bIsReady)
					{
						//return spectator class;
						
						return ASpectatorPawn::StaticClass();
					}

					//otherwise we return the selected character
					int SelectedCharacter = PlayerState->SelectedCharacterIndex;

					//if it is a valid index, return that class
					if (PlatGameState->Characters.IsValidIndex(SelectedCharacter))
					{
						return PlatGameState->Characters[SelectedCharacter].CharacterClass;
					}
					else
					{
						//if the index is invalid, return the first character in the list.
						return PlatGameState->Characters[0].CharacterClass;
					}
				}
			}
		}
	}
	//if the match isnt starting and isnt in progress, we default to spectator
	return ASpectatorPawn::StaticClass();
}