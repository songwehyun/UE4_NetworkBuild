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

	/*세션 매니지먼트를 위해 함수 바인딩*/
	
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UPlatformerGameInstance::OnCreateSessionComplete);

	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UPlatformerGameInstance::OnStartOnlineGameComplete);



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

void UPlatformerGameInstance::HostGame(bool bIsLAN, int32 MaxNumPlayers, TArray<FBlueprintSessionSetting> sessionSettings)
{
	//Get the Online Subsystem.
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		//유니크플레이어id를 가진다. 0은 클라이언트마다 멀티플레이어가 허용되지 않을때 사용한다. 
		TSharedPtr<const FUniqueNetId> pid = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(0);

		//SpecaiSetting map 제작
		TMap<FString, FOnlineSessionSetting> SpecialSettings = TMap<FString, FOnlineSessionSetting>();

		//제공된 세팅을 루프로 돌아 special settings map을 추가
		for (auto &setting : sessionSettings)
		{
			FOnlineSessionSetting newSetting;

			newSetting.Data = setting.value;

			newSetting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;

			SpecialSettings.Add(setting.key, newSetting);
		}

		//게임을 호스트를 시도할때 로딩스크린의 스테이트 변경.
		ChangeState(EGameState::ELoadingScreen);

		//세션 호스트
		HostSession(pid, GameSessionName, bIsLAN, MaxNumPlayers, SpecialSettings);
	}
}

bool UPlatformerGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, int32 MaxNumPlayers, TMap<FString, FOnlineSessionSetting> SettingsMap)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		//Get SessionInterface, createSession을 위에 호출 할수 있다.
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			//SessionSettings을 공유포인터로 만듬.
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = true;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			//specialsettings을 사용할 맵에 세팅한다.
			//계속 Map_SandBox를 사용할거지만
			//게임중이거나,호스팅 스크린에서 변경하게 해줄 수있다
			SessionSettings->Set(SETTING_MAPNAME, FString("Map_SandBox"), EOnlineDataAdvertisementType::ViaOnlineService);

			//SpecialSettings을 추가.
			for (auto &setting : SettingsMap)
			{
				SessionSettings->Settings.Add(FName(*setting.Key), setting.Value);
			}

			//델리게이트와 세션인터페이스등록과 함께 핸들을 가져옴.
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			//CreateSession에 값을 리턴
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	//서브시스템을 못찾았다면 return false.
	return false;
}

void UPlatformerGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		//세션 인터페이스 Get.
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			//호출을 여기까지 마쳤으니 DelegateHandle을 clear해준다.
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			//Successful이였다면 게임 Start.
			if (bWasSuccessful)
			{
				//StartSessionDelegate 세팅.
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				//StartSession 실행.
				Sessions->StartSession(SessionName);
			}
		}
	}
}

void UPlatformerGameInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			//델리게이트 클리어
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	//시작까지 성공적이였다면 맵을 리슨서버로 연다
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Map_SandBox", true, "listen");

		ChangeState(EGameState::ETravelling);
	}
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
