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

	//SearchResult�� ����ִ� ��츦 ���ؼ� �����ڷ� �ʱ�ȭ
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
