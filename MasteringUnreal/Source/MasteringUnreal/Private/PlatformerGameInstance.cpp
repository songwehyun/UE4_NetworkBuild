// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerGameInstance.h"


UPlatformerGameInstance::UPlatformerGameInstance(const FObjectInitializer& ObjectInitializer)
{
	//������Ʈ �ʱ�ȭ.
	CurrentState = EGameState::ENone;

	//������ ������ �����Ƿ� �� �ʱ�ȭ.
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
