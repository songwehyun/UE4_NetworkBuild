// Fill out your copyright notice in the Description page of Project Settings.


#include "MasteringUnrealGameModeBase.h"
#include "PlatformerGameState.h"

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