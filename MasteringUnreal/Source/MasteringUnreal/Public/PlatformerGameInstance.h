// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasteringUnreal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "PlatformerGameInstance.generated.h"

/* ���� ���¸� �����ϱ� ���� �̳ѹ� */

UENUM(BlueprintType)
enum class EGameState : uint8
{
	ENone						UMETA(DisplayName =  "None"),
	ELoadingScreen				UMETA(DisplayName =  "Loading Screen"),
	EStartup					UMETA(DisplayName =  "Startup"),
	EMainMenu					UMETA(DisplayName =  "Main Menu"),
	EMultiplayerHome			UMETA(DisplayName =  "Multiplayer Home"),
	EMultiplayerJoin			UMETA(DisplayName =  "Multiplayer Join"),
	EMultiplayerHost			UMETA(DisplayName =  "Multiplayer Host"),
	EMultiplayerInGame			UMETA(DisplayName =  "Multiplayer In Game"),
	ETravelling					UMETA(DisplayName =  "Travelling"),
};

/*ȣ��Ʈ�� ������ Special Settings�� �ϱ����� Ÿ�� ����*/
USTRUCT(BlueprintType)
struct FBlueprintSessionSetting
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Session Management")
		FString key;
	UPROPERTY(BlueprintReadWrite, Category = "Session Management")
		FString value;
};

/* SerchResult�� ����ֱ� ���� Ÿ��, ���� �ʿ��� �������� �������Ʈ�� �����Ų��.*/
USTRUCT(BlueprintType)
struct FBlueprintSearchResult
{
	GENERATED_BODY()

	FOnlineSessionSearchResult result;

	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		FString ServerName;
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		bool bIsInprogress;
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		FString MapName;
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		int PingInMs;
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		int CurrentPlayers;
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		int MaxPlayers;

	//SearchResult�� ����ִ� ����� ������.
	FBlueprintSearchResult()
	{
		ServerName = FString("No Server Info");
		PingInMs = -1;
		MapName = FString("No Map Info");
		CurrentPlayers = 0;
		MaxPlayers = 0;
	}

	/*SearchResult�� �ִ°���� ������*/
	FBlueprintSearchResult(FOnlineSessionSearchResult theResult)
	{
		//keep result
		result = theResult;

		//serchresult�� SpecialSetting���� ������.
		ServerName = GetSpecialSettingString(FString("ServerName"));
		MapName = GetSpecialSettingString(FString("MAPNAME"));
		FString InProgressString = GetSpecialSettingString(FString("InProgress"));

		if (InProgressString == FString("true"))
		{
			bIsInprogress = true;
		}
		else
		{
			bIsInprogress = false;
		}
		
		
		MaxPlayers = result.Session.SessionSettings.NumPublicConnections;
		CurrentPlayers = MaxPlayers - result.Session.NumOpenPublicConnections;
		PingInMs = result.PingInMs;
	}

	//result�� ������ specialsettings�����͸� ������ �Լ�.
	FString GetSpecialSettingString(FString key)
	{
		FOnlineSessionSettings settings = result.Session.SessionSettings;

		//Ű�� �ִ��� Ȯ��
		if (settings.Settings.Contains(FName(*key)))
		{
			FString value;

			settings.Settings[FName(*key)].Data.GetValue(value);
			return value;
		}
		
		//������ ������ ���� �ʴٸ�.
		return FString("No DATA AT THAT KEY");
	}
};
/* 
ENUM TO TRACK INPUT STATES
�־��� �ð��� �Է¿� ���� � ���� �����ߴ��� �����ϱ����� �̳ѹ�
������ ¥���� Inputmode data�� ������Ȳ����,
�����ϴ� ����� �޴����� ���� �ʴ´ٸ�, 
���� ���°� ����� �����ϴ°��� ������ ����� ���̴�.*/
UENUM(BlueprintType)
enum class EInputMode : uint8
{
	EUIOnly					UMETA(DisplayName = "UI Only"),
	EUIAndGame				UMETA(DisplayName = "UI And Game"),
	EGameOnly				UMETA(DisplayName = "Game Only"),
};

/**
 * 
 */
/*
���ÿ��� �����ϴ� Ŭ�����̴�.
�ַ� StateMachine, StateManager�� �����ϱ����� ���Ǵ� Ŭ�����̴�.
GameInstance�� �� ������
���Ӹ޴� �׺�����ð� 
GameMode(������) GameState(Ŭ���̾�Ʈ + ����)�� ������� �ѱ�� ��
Ŀ�ؼǵ��� �ʱ�ȭ �����ִ� ������ ������ �ִ�.
*/
UCLASS()
class MASTERINGUNREAL_API UPlatformerGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	/*���� C++ ����� UPlatformerGameInstance();�� �����ڸ� ���� �� ����.*/
	UPlatformerGameInstance(const FObjectInitializer& ObjectInitializer);

	//Init Function
	virtual void Init() override;
	/*���� ���۷���, �̴� ��ũ���� ���÷��� �ɰ��̴�.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Game Instance")
		TSubclassOf<class UUserWidget> cMainMenu;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Game Instance")
		TSubclassOf<class UUserWidget> cMPHome;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Game Instance")
		TSubclassOf<class UUserWidget> cMPJoin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Game Instance")
		TSubclassOf<class UUserWidget> cMPHost;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Game Instance")
		TSubclassOf<class UUserWidget> cLoadingScreen;

	/* ���� ��ȭ �Լ� */
	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
		void ChangeState(EGameState newState);

	//������ ���� ������Ʈ ��
	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
		EGameState GetGameState();

	/*INPUT MODE HANDLING*/
	
	//��ǲ ��� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
		void SetInputMode(EInputMode newInputMode, bool bShowMouseCursor);

	//���� ��ǲ���
	EInputMode CurrentInputMode;

	//������ ���콺Ŀ�� ���÷���.
	bool bIsShowingMouseCursor;
	
	/*���� ������ ������ ���� ���� ������ */
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	/*���� ���� */
	//�������Ʈ�� ���ǿ� ȣ�����ϱ� ���� �Լ�.
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void HostGame(bool bIsLAN, int32 MaxNumPlayers, TArray<FBlueprintSessionSetting> sessionSettings);

	/*���ǿ� ȣ�����ϱ� ���� C++�Լ�*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName,
		bool bIsLAN, int32 MaxNumPlayers, TMap<FString, FOnlineSessionSetting> SettingsMap);

	//������ ����������� ȣ��� �������̵� �Լ�.
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	//������ ����������� ȣ��� ��������Ʈ
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

	//��������Ʈ�� ��ϵǰ� ���� ������� ��������Ʈ �ڵ�
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	/*���� ���� �� */

	//������ ���۵ǰ� ���� ȣ���� ��������Ʈ �Լ�
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	//������ ���۵ǰ� ���� ȣ��� ��������Ʈ
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	//��������Ʈ ����� ������� ��������Ʈ �ڵ�
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	/*���� ã��*/

	//search results�� ������ ���� array
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		TArray<FBlueprintSearchResult> searchResults;

	//search status�� ������ booleans
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		bool bHasFinishedSearchingForGames;
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		bool bSearchingForGames;

	//c++ Native searchresults �� ����������
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	//blueprint function for finding games
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void FindGames(bool bIsLAN);

	//������ ã������ c++ �Լ�
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN);

	//FindSessions�� ������ ȣ���� ��������Ʈ �Լ�.
	void OnFindSessionComplete(bool bWasSuccessful);
	
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	/*JOIN SESSIONS*/
	//���ǿ� �������� Blueprint �Լ�
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void JoinGame(FBlueprintSearchResult result);

	//���ǿ� �������� C++�Լ�.
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	//���ǿ� �������� ȣ���� ��������Ʈ�Լ�.
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	//�Լ��� ���ε� �� ��������Ʈ.
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	//OnJoinSessionCompleteDelgate�� ������ ��������Ʈ �ڵ�.
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/* UPDAITNG SESSION */
	//Ȱ��ȭ�� ������ special setting�� ���� ���� �������� �Լ�.
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		FString GetSessionSpecialSettingString(FString key);

	//Ȱ��ȭ�� ������ �����ϴ� special setting�� ������Ʈ�ϰų� ���� ����� �Լ�.
	//host only.
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void SetOrUpdateSessionSpecialSettingString(FBlueprintSessionSetting newSetting);

	//���Ǿ�����Ʈ�� �������� ȣ��Ǵ� ��������Ʈ �Լ�.
	void OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful);

	//������Ʈ�� ������ ���� ȣ���� ��������Ʈ
	FOnUpdateSessionCompleteDelegate OnUpdateSessionCompleteDelegate;

	//OnupdateSessionCompleteDelegate�� ������ �ڵ�.
	FDelegateHandle OnUpdateSessionCompleteDelegateHandle;

	/* Destroy Session / Leave Game */
	
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void LeaveGame();

	//������ ������ ���� ȣ���� ��������Ʈ �Լ�.
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	//������ Destroy���� ȣ���� ��������Ʈ
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	//OnDestroySessionCompleteDelegate�� ������ �ڵ�.
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	/* Handle Network Error*/
	void HandleNetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
private:
	//���� ���̴� ����
	UUserWidget* CurrentWidget;

	//������ ���� ������Ʈ
	EGameState CurrentState;

	//������Ʈ�� �����ϱ� ���� �Լ�.
	void EnterState(EGameState newState);
	//���� ������Ʈ�� ���������� ���� �Լ�.
	void LeaveState();
};
