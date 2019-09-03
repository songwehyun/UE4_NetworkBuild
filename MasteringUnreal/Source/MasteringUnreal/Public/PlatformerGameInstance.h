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

	//SearchResult가 비어있는 경우를 위해서 생성자로 초기화
	FBlueprintSearchResult()
	{
		ServerName = FString("No Server Info");
		PingInMs = -1;
		MapName = FString("No Map Info");
		CurrentPlayers = 0;
		MaxPlayers = 0;
	}

	/*FBlueprintSearchResult(FOnlineSessionSearchResult theResult)
	{

	}*/
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
