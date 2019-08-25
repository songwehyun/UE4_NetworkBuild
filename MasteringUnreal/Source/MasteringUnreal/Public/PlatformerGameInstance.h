// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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
