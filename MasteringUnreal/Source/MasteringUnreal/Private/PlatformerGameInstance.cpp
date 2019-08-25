// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerGameInstance.h"


UPlatformerGameInstance::UPlatformerGameInstance(const FObjectInitializer& ObjectInitializer)
{
	//스테이트 초기화.
	CurrentState = EGameState::ENone;

	//현재의 위젯은 없으므로 널 초기화.
	CurrentWidget = nullptr;

}

void UPlatformerGameInstance::ChangeState(EGameState newState)
{
}

EGameState UPlatformerGameInstance::GetGameState()
{
	return EGameState();
}

void UPlatformerGameInstance::SetInputMode(EInputMode newInputMode, bool bShowMouseCursor)
{
}

void UPlatformerGameInstance::EnterState(EGameState newState)
{
}

void UPlatformerGameInstance::LeaveState()
{
}
