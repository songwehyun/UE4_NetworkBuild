// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerGameInstance.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Classes/GameFramework/PlayerController.h"
UPlatformerGameInstance::UPlatformerGameInstance(const FObjectInitializer& ObjectInitializer)
{
	//스테이트 초기화.
	CurrentState = EGameState::ENone;

	//현재의 위젯은 없으므로 널 초기화.
	CurrentWidget = nullptr;

}

void UPlatformerGameInstance::ChangeState(EGameState newState)
{
	//현재의 스테이트가 들어온 스테이트와 다르다면,
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

/*게임이 아직 멀티플레이어로 셋업이 되지 않았다면,
각 클라이언트에는 단 하나의 플레이어 컨트롤러만 있게 되는데,
이때 GetFirstPlayerController()는 PlayerContorller만 반환하게 될것이다.
World.Cpp에 보면 PlayerControllerIterator에서 첫번째 원소를 반환하는것을 확인 할 수 있다.
PlayerContorller가 TArray에담겨있고,상황에 따라 월드에 추가가 되고있다.
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

	//마우스커서 보이거나 숨기기.
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = bShowMouseCursor;

	//변수에 저장해놓음.
	CurrentInputMode = newInputMode;
	bIsShowingMouseCursor = bShowMouseCursor;
}

void UPlatformerGameInstance::EnterState(EGameState newState)
{
	//CurrentState를 newState로 셋업.
	CurrentState = newState;

	switch (CurrentState)
	{
		case EGameState::ELoadingScreen: 
		{
			//위젯 생성
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cLoadingScreen);

			//뷰포트에 위젯 추가.
			CurrentWidget->AddToViewport();

			//적절한 인풋모드 셋
			SetInputMode(EInputMode::EUIOnly, true);
			break;
		}
		case EGameState::EMainMenu:
		{
			//위젯 생성
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMainMenu);

			//뷰포트에 위젯 추가.
			CurrentWidget->AddToViewport();

			//적절한 인풋모드 셋
			SetInputMode(EInputMode::EUIOnly, true);
			break;
		}
		case EGameState::EMultiplayerHome:
		{
			//위젯 생성
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMPHome);

			//뷰포트에 위젯 추가.
			CurrentWidget->AddToViewport();

			//적절한 인풋모드 셋
			SetInputMode(EInputMode::EUIOnly, true);
			break;
		}
		case EGameState::EMultiplayerHost:
		{
			//위젯 생성
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMPHost);

			//뷰포트에 위젯 추가.
			CurrentWidget->AddToViewport();

			//적절한 인풋모드 셋
			SetInputMode(EInputMode::EUIOnly, true);
			break;
		}
		case EGameState::EMultiplayerInGame:
		{
			//게임에만 인풋모드 셋팅.
			SetInputMode(EInputMode::EGameOnly, false);
			break;
		}
		case EGameState::EMultiplayerJoin:
		{
			//위젯 생성
			CurrentWidget = CreateWidget<UUserWidget>(GetWorld()->GetFirstPlayerController(), cMPJoin);

			//뷰포트에 위젯 추가.
			CurrentWidget->AddToViewport();

			//적절한 인풋모드 셋
			SetInputMode(EInputMode::EUIOnly, true);
			break;
		}
		case EGameState::EStartup:
		{
			break;
		}
		case EGameState::ETravelling:
		{
			//인풋모드를 UI only로 하여 게임인풋을 막고
			//커서를 표시하지않음.
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
