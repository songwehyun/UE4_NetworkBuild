// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerGameState.h"

APlatformerGameState::APlatformerGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlatformerGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
