// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerGameInstance.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
UPlatformerGameInstance::UPlatformerGameInstance(const FObjectInitializer& ObjectInitializer)
{
	//������Ʈ �ʱ�ȭ.
	CurrentState = EGameState::ENone;

	//������ ������ �����Ƿ� �� �ʱ�ȭ.
	CurrentWidget = nullptr;

}

void UPlatformerGameInstance::ChangeState(EGameState newState)
{
	//������ ������Ʈ�� ���� ������Ʈ�� �ٸ��ٸ�,
	if (CurrentState != newState)
	{
		LeaveState();
		EnterState(newState);
	}
}

EGameState UPlatformerGameInstance::GetGameState()
{
	return CurrentState;
}

/*������ ���� ��Ƽ�÷��̾�� �¾��� ���� �ʾҴٸ�,
�� Ŭ���̾�Ʈ���� �� �ϳ��� �÷��̾� ��Ʈ�ѷ��� �ְ� �Ǵµ�,
�̶� GetFirstPlayerController()�� PlayerContorller�� ��ȯ�ϰ� �ɰ��̴�.
World.Cpp�� ���� PlayerControllerIterator���� ù��° ���Ҹ� ��ȯ�ϴ°��� Ȯ�� �� �� �ִ�.
PlayerContorller�� TArray������ְ�,��Ȳ�� ���� ���忡 �߰��� �ǰ��ִ�.
(typedef TArray<TWeakObjectPtr<AController> >::TConstIterator FConstControllerIterator;)
*/
void UPlatformerGameInstance::SetInputMode(EInputMode newInputMode, bool bShowMouseCursor)
{
	switch (newInputMode)
	{
	case EInputMode::EUIOnly:
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
		break;
	case EInputMode::EUIAndGame:
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		break;
	case EInputMode::EGameOnly:
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		break;
	}

	//���콺Ŀ�� ���̰ų� �����.
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = bShowMouseCursor;

	//������ �����س���.
	CurrentInputMode = newInputMode;
	bIsShowingMouseCursor = bShowMouseCursor;
}

void UPlatformerGameInstance::EnterState(EGameState newState)
{
	//CurrentState�� newState�� �¾�.
	CurrentState = newState;

	switch (CurrentState)
	{
		case EGameState::ELoadingScreen: 
		{
			//���� ����
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cLoadingScreen);

			//����Ʈ�� ���� �߰�.
			CurrentWidget->AddToViewport();

			//������ ��ǲ��� ��
			SetInputMode(EInputMode::EUIOnly, true);
			break;
		}
		case EGameState::EMainMenu:
		{
			//���� ����
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMainMenu);

			//����Ʈ�� ���� �߰�.
			CurrentWidget->AddToViewport();

			//������ ��ǲ��� ��
			SetInputMode(EInputMode::EUIOnly, true);
			break;
		}
		case EGameState::EMultiplayerHome:
		{
			//���� ����
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMPHome);

			//����Ʈ�� ���� �߰�.
			CurrentWidget->AddToViewport();

			//������ ��ǲ��� ��
			SetInputMode(EInputMode::EUIOnly, true);
			break;
		}
		case EGameState::EMultiplayerHost:
		{
			//���� ����
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMPHost);

			//����Ʈ�� ���� �߰�.
			CurrentWidget->AddToViewport();

			//������ ��ǲ��� ��
			SetInputMode(EInputMode::EUIOnly, true);
			break;
		}
		case EGameState::EMultiplayerInGame:
		{
			//���ӿ��� ��ǲ��� ����.
			SetInputMode(EInputMode::EGameOnly, false);
			break;
		}
		case EGameState::EMultiplayerJoin:
		{
			//���� ����
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMPJoin);

			//����Ʈ�� ���� �߰�.
			CurrentWidget->AddToViewport();

			//������ ��ǲ��� ��
			SetInputMode(EInputMode::EUIOnly, true);
			break;
		}
		case EGameState::EStartup:
		{
			break;
		}
		case EGameState::ETravelling:
		{
			//��ǲ��带 UI only�� �Ͽ� ������ǲ�� ����
			//Ŀ���� ǥ����������.
			SetInputMode(EInputMode::EUIOnly, false);
			break;
		}
		default:
		{
			break;
		}
	}
}

void UPlatformerGameInstance::LeaveState()
{
	switch (CurrentState)
	{
	case EGameState::ELoadingScreen:
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
		break;
	}
	case EGameState::EMainMenu:
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
		break;
	}
	case EGameState::EMultiplayerJoin:
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
		break;
	}
	case EGameState::EMultiplayerHome:
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
		break;
	}
	case EGameState::EMultiplayerInGame:
	{
		break;
	}
	case EGameState::EMultiplayerHost:
	{
		if (CurrentWidget)
		{
			CurrentWidget->RemoveFromViewport();
			CurrentWidget = nullptr;
		}
		break;
	}
	case EGameState::EStartup:
	{
		break;
	}
	case EGameState::ETravelling:
	{
		break;
	}
	default:
	{
		break;
	}
	
	}

	EnterState(EGameState::ENone);
}
