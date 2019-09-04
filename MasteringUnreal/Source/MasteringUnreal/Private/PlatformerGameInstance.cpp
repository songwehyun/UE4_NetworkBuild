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

	/*���� �Ŵ�����Ʈ�� ���� �Լ� ���ε�*/
	
	//Create
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UPlatformerGameInstance::OnCreateSessionComplete);

	//Start
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UPlatformerGameInstance::OnStartOnlineGameComplete);

	//Find
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UPlatformerGameInstance::OnFindSessionComplete);
	
	//Join
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UPlatformerGameInstance::OnJoinSessionComplete);

	//Update
	OnUpdateSessionCompleteDelegate = FOnUpdateSessionCompleteDelegate::CreateUObject(this, &UPlatformerGameInstance::OnUpdateSessionComplete);

	//Destroy
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UPlatformerGameInstance::OnDestroySessionComplete);
}

void UPlatformerGameInstance::Init()
{
	Super::Init();

	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UPlatformerGameInstance::HandleNetworkError);
	}
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

void UPlatformerGameInstance::HostGame(bool bIsLAN, int32 MaxNumPlayers, TArray<FBlueprintSessionSetting> sessionSettings)
{
	//Get the Online Subsystem.
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		//����ũ�÷��̾�id�� ������. 0�� Ŭ���̾�Ʈ���� ��Ƽ�÷��̾ ������ ������ ����Ѵ�. 
		TSharedPtr<const FUniqueNetId> pid = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(0);

		//SpecaiSetting map ����
		TMap<FString, FOnlineSessionSetting> SpecialSettings = TMap<FString, FOnlineSessionSetting>();

		//������ ������ ������ ���� special settings map�� �߰�
		for (auto &setting : sessionSettings)
		{
			FOnlineSessionSetting newSetting;

			newSetting.Data = setting.value;

			newSetting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;

			SpecialSettings.Add(setting.key, newSetting);
		}

		//������ ȣ��Ʈ�� �õ��Ҷ� �ε���ũ���� ������Ʈ ����.
		ChangeState(EGameState::ELoadingScreen);

		//���� ȣ��Ʈ
		HostSession(pid, GameSessionName, bIsLAN, MaxNumPlayers, SpecialSettings);
	}
}

bool UPlatformerGameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, int32 MaxNumPlayers, TMap<FString, FOnlineSessionSetting> SettingsMap)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		//Get SessionInterface, createSession�� ���� ȣ�� �Ҽ� �ִ�.
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			//SessionSettings�� ���������ͷ� ����.
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

			//specialsettings�� ����� �ʿ� �����Ѵ�.
			//��� Map_SandBox�� ����Ұ�����
			//�������̰ų�,ȣ���� ��ũ������ �����ϰ� ���� ���ִ�
			SessionSettings->Set(SETTING_MAPNAME, FString("Map_SandBox"), EOnlineDataAdvertisementType::ViaOnlineService);

			//SpecialSettings�� �߰�.
			for (auto &setting : SettingsMap)
			{
				SessionSettings->Settings.Add(FName(*setting.Key), setting.Value);
			}

			//��������Ʈ�� �����������̽���ϰ� �Բ� �ڵ��� ������.
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			//CreateSession�� ���� ����
			return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	//����ý����� ��ã�Ҵٸ� return false.
	return false;
}

void UPlatformerGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		//���� �������̽� Get.
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			//ȣ���� ������� �������� DelegateHandle�� clear���ش�.
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			//Successful�̿��ٸ� ���� Start.
			if (bWasSuccessful)
			{
				//StartSessionDelegate ����.
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				//StartSession ����.
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
			//��������Ʈ Ŭ����
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	//���۱��� �������̿��ٸ� ���� ���������� ����
	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Map_SandBox", true, "listen");

		ChangeState(EGameState::ETravelling);
	}
}

void UPlatformerGameInstance::FindGames(bool bIsLAN)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	bHasFinishedSearchingForGames = false;
	bSearchingForGames = false;
	searchResults.Empty();

	if (OnlineSub)
	{
		TSharedPtr<const FUniqueNetId> pid = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(0);

		FindSessions(pid, GameSessionName, bIsLAN);
	}
}

void UPlatformerGameInstance::FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		//Get Session Interface
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			//Create the Shared Pointer to search Settings.
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			//Fill settings.
			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 100000000;
			SessionSearch->PingBucketSize = 50;

			//"Presence"�� ������ �¾��ȴ�.
			//Presence�� ��������ʰڴٸ� ���� ���� �ʿ� ����.
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

			//FindSessions �Լ��� session interface�� �����ϱ� ���� �������۷����� �ʿ���/
			TSharedRef<FOnlineSessionSearch> SearchSettingRef = SessionSearch.ToSharedRef();

			//��������Ʈ�� �¶��μ���ý����� ����ϰ� �ڵ�������������.
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			//game instance�� ���� ��Ī�� �����ߴٰ� �˸�.
			bSearchingForGames = true;

			//SessionsInterface�� FindSessionsȣ��.
			Sessions->FindSessions(*UserId, SearchSettingRef);
		}
	}
	else
	{
		//�¶��� ����ý����� ���ٸ�, OnFindSessionComplete�� �����ߴٰ� ��� �˸�.
		OnFindSessionComplete(false);
	}
}

void UPlatformerGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	//����ã�°��� �Ϸ�Ǿ��ٸ� �ϴ� delegate hanlde��clear����� �ȴ�.
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
		}
	}

	//�׸��� ���� search results �� �ٷ�� �ȴ�.

	//������� �������̿��ٸ� searchresults�� �Ľ����ְ� �ƴ϶�� search�� ������.
	if (bWasSuccessful)
	{
		//SearchResults�� ������ ���� �������� �������Ʈ �迭�� �߰��Ѵ�.
		for (auto &result : SessionSearch->SearchResults)
		{
			FBlueprintSearchResult newresult = FBlueprintSearchResult(result);
			searchResults.Add(newresult);
		}
	}

	//��Ī�� �����ٰ� �˸���.
	bHasFinishedSearchingForGames = true;
	bSearchingForGames = false;
}

void UPlatformerGameInstance::JoinGame(FBlueprintSearchResult result)
{
	//Unique Player Id�� �Ǻ��ϱ� ���� OnlineSubsystem�� �����´�.
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		//uniqueid�� �����´�.
		TSharedPtr<const FUniqueNetId> pid = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(0);

		//FBlueprintSearchResult����ü�� �ִ� result�� ����ϸ鼭 JoinSession�� ȣ���Ѵ�.
		JoinSession(pid, GameSessionName, result.result);
	}
}

bool UPlatformerGameInstance::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	//to hold result.
	bool bSuccessful = false;

	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			//��������Ʈ�� ����ϰ� �ڵ��� �������ִ´�.
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			//SessionInterface�� JoinSession�Լ� ���ϰ��� bSuccesful�� �������ش�.
			bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}

	return bSuccessful;
}
void UPlatformerGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			//���ǿ� ���������� ���Դٸ� Delegate�� clear���ش�.
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			//first player controller�� ���������ν� client travel�� ȣ�� �Ҽ� �ְ� �Ѵ�.
			APlayerController* const PlayerController = GetFirstLocalPlayerController();

			FString TravelURL;

			if (PlayerController && Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				//ClinetTravel�� ȣ���Ѵ�.��� ���̳� IP�ּҷ� ���� �����ȴ�.
				PlayerController->ClientTravel(TravelURL, ETravelType::TRAVEL_Absolute);

				//Travelling���� ���¸� �ٲ۴�.
				ChangeState(EGameState::ETravelling);
			}
		}
	}
}

FString UPlatformerGameInstance::GetSessionSpecialSettingString(FString key)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			//get Current session settings
			FOnlineSessionSettings* settings = Sessions->GetSessionSettings(GameSessionName);

			//ensure settings isnt null
			if (settings)
			{
				if (settings->Settings.Contains(FName(*key)))
				{
					FString value;
					settings->Settings[FName(*key)].Data.GetValue(value);

					return value;
				}
			}
			else
			{
				//Ű�� ���°��.
				return FString("INVALID KEY");
			}
		}
	}
	return FString("NO ONLINE SUBSYSTEM");
}

void UPlatformerGameInstance::SetOrUpdateSessionSpecialSettingString(FBlueprintSessionSetting newSetting)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			FOnlineSessionSettings* settings = Sessions->GetSessionSettings(GameSessionName);

			if (settings)
			{
				//check to see if it already contains the key
				if (settings->Settings.Contains(FName(*newSetting.key)))
				{
					//update setting
					settings->Settings[FName(*newSetting.key)].Data = newSetting.value;
				}
				else
				{
					//Ű�� ���ٸ� ���ο� ������ �����.
					FOnlineSessionSetting setting;

					setting.Data = newSetting.value;
					setting.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;

					//��̿� �� ������ �߰�.
					settings->Settings.Add(FName(*newSetting.key), setting);
				}
			}

			//��������Ʈ�� ����ϰ� �ڵ��� ����.
			OnUpdateSessionCompleteDelegateHandle = Sessions->AddOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegate);

			//���� ���� ������Ʈ.
			Sessions->UpdateSession(GameSessionName, *settings, true);
		}
	}
}

void UPlatformerGameInstance::OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			//clear the delegate
			Sessions->ClearOnUpdateSessionCompleteDelegate_Handle(OnUpdateSessionCompleteDelegateHandle);
		}
	}
}

void UPlatformerGameInstance::LeaveGame()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			//��������Ʈ ���
			OnDestroySessionCompleteDelegateHandle = Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			//���� destroy
			Sessions->DestroySession(GameSessionName);
		}
	}
}

void UPlatformerGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
		}
	}

	if (bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), "Map_MainMenu", true);
		ChangeState(EGameState::ETravelling);
	}
}

void UPlatformerGameInstance::HandleNetworkError(UWorld * World, UNetDriver * NetDriver, ENetworkFailure::Type FailureType, const FString & ErrorString)
{
	LeaveGame();
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
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
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
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
		break;
	}
	case EGameState::ETravelling:
	{
		CurrentWidget->RemoveFromViewport();
		CurrentWidget = nullptr;
		break;
	}
	default:
	{
		break;
	}
	
	}

	EnterState(EGameState::ENone);
}
