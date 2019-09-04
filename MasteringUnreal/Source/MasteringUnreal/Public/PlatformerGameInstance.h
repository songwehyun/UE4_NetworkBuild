// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasteringUnreal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "PlatformerGameInstance.generated.h"

/* 게임 상태를 추적하기 위한 이넘문 */

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

/*호스트된 서버에 Special Settings을 하기위한 타입 세팅*/
USTRUCT(BlueprintType)
struct FBlueprintSessionSetting
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Session Management")
		FString key;
	UPROPERTY(BlueprintReadWrite, Category = "Session Management")
		FString value;
};

/* SerchResult를 들고있기 위한 타입, 또한 필요한 변수들은 블루프린트에 노출시킨다.*/
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

	//SearchResult가 비어있는 경우의 생성자.
	FBlueprintSearchResult()
	{
		ServerName = FString("No Server Info");
		PingInMs = -1;
		MapName = FString("No Map Info");
		CurrentPlayers = 0;
		MaxPlayers = 0;
	}

	/*SearchResult가 있는경우의 생성자*/
	FBlueprintSearchResult(FOnlineSessionSearchResult theResult)
	{
		//keep result
		result = theResult;

		//serchresult의 SpecialSetting값을 가져옴.
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

	//result로 부터의 specialsettings데이터를 가져올 함수.
	FString GetSpecialSettingString(FString key)
	{
		FOnlineSessionSettings settings = result.Session.SessionSettings;

		//키가 있는지 확인
		if (settings.Settings.Contains(FName(*key)))
		{
			FString value;

			settings.Settings[FName(*key)].Data.GetValue(value);
			return value;
		}
		
		//세팅을 가지고 있지 않다면.
		return FString("No DATA AT THAT KEY");
	}
};
/* 
ENUM TO TRACK INPUT STATES
주어진 시간에 입력에 따라 어떤 상태 돌입했는지 추적하기위한 이넘문
구조가 짜여진 Inputmode data를 가진상황에서,
추적하는 방법을 메뉴얼대로 하지 않는다면, 
현재 상태가 어떠한지 추적하는것은 굉장히 어려운 일이다.*/
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
로컬에만 존재하는 클래스이다.
주로 StateMachine, StateManager를 관리하기위해 사용되는 클래스이다.
GameInstance의 주 역할은
게임메뉴 네비게이팅과 
GameMode(서버와) GameState(클라이언트 + 서버)에 제어권을 넘기기 전
커넥션들을 초기화 시켜주는 역할을 가지고 있다.
*/
UCLASS()
class MASTERINGUNREAL_API UPlatformerGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	/*원시 C++ 방식인 UPlatformerGameInstance();로 생성자를 만들 수 없다.*/
	UPlatformerGameInstance(const FObjectInitializer& ObjectInitializer);

	//Init Function
	virtual void Init() override;
	/*위젯 레퍼런스, 이는 스크린에 디스플레이 될것이다.*/
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

	/* 상태 변화 함수 */
	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
		void ChangeState(EGameState newState);

	//현재의 게임 스테이트 겟
	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
		EGameState GetGameState();

	/*INPUT MODE HANDLING*/
	
	//인풋 모드 세팅 함수
	UFUNCTION(BlueprintCallable, Category = "Platformer Game Instance")
		void SetInputMode(EInputMode newInputMode, bool bShowMouseCursor);

	//현재 인풋모드
	EInputMode CurrentInputMode;

	//현재의 마우스커서 디스플레이.
	bool bIsShowingMouseCursor;
	
	/*세션 세팅을 가지고 있을 공유 포인터 */
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	/*세션 제작 */
	//블루프린트로 세션에 호스팅하기 위한 함수.
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void HostGame(bool bIsLAN, int32 MaxNumPlayers, TArray<FBlueprintSessionSetting> sessionSettings);

	/*세션에 호스팅하기 위한 C++함수*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName,
		bool bIsLAN, int32 MaxNumPlayers, TMap<FString, FOnlineSessionSetting> SettingsMap);

	//세션이 만들어졌을때 호출될 델리게이드 함수.
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	//세션이 만들어졌을때 호출될 델리게이트
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

	//델리게이트가 등록되고 나서 들고있을 델리게이트 핸들
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	/*세션 시작 후 */

	//세션이 시작되고 나서 호출할 델리게이트 함수
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	//세션이 시작되고 나서 호출될 델리게이트
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	//델리게이트 등록후 들고있을 델리게이트 핸들
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	/*세션 찾기*/

	//search results를 가지고 있을 array
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		TArray<FBlueprintSearchResult> searchResults;

	//search status를 추적할 booleans
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		bool bHasFinishedSearchingForGames;
	UPROPERTY(BlueprintReadOnly, Category = "Session Management")
		bool bSearchingForGames;

	//c++ Native searchresults 에 공유포인터
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	//blueprint function for finding games
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void FindGames(bool bIsLAN);

	//세션을 찾기위한 c++ 함수
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN);

	//FindSessions이 끝나면 호출할 델리게이트 함수.
	void OnFindSessionComplete(bool bWasSuccessful);
	
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	/*JOIN SESSIONS*/
	//세션에 들어가기위한 Blueprint 함수
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void JoinGame(FBlueprintSearchResult result);

	//세션에 들어가기위한 C++함수.
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	//세션에 들어가졌을때 호출할 델리게이트함수.
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	//함수를 바인딩 할 델리게이트.
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	//OnJoinSessionCompleteDelgate를 제어할 델리게이트 핸들.
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	/* UPDAITNG SESSION */
	//활성화된 세션의 special setting의 현재 값을 가져오는 함수.
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		FString GetSessionSpecialSettingString(FString key);

	//활성화된 세션의 존재하는 special setting을 업데이트하거나 새로 만드는 함수.
	//host only.
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void SetOrUpdateSessionSpecialSettingString(FBlueprintSessionSetting newSetting);

	//세션업데이트가 끝났을때 호출되는 델리게이트 함수.
	void OnUpdateSessionComplete(FName SessionName, bool bWasSuccessful);

	//업데이트가 끝나고 나서 호출할 델리게이트
	FOnUpdateSessionCompleteDelegate OnUpdateSessionCompleteDelegate;

	//OnupdateSessionCompleteDelegate를 제어할 핸들.
	FDelegateHandle OnUpdateSessionCompleteDelegateHandle;

	/* Destroy Session / Leave Game */
	
	UFUNCTION(BlueprintCallable, Category = "Session Management")
		void LeaveGame();

	//세션을 떠나고 나서 호출할 델리게이트 함수.
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	//세션을 Destroy한후 호출할 델리게이트
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	//OnDestroySessionCompleteDelegate를 제어할 핸들.
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	/* Handle Network Error*/
	void HandleNetworkError(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);
private:
	//현재 보이는 위젯
	UUserWidget* CurrentWidget;

	//현재의 게임 스테이트
	EGameState CurrentState;

	//스테이트에 돌입하기 위한 함수.
	void EnterState(EGameState newState);
	//현재 스테이트에 빠져나오기 위한 함수.
	void LeaveState();
};
