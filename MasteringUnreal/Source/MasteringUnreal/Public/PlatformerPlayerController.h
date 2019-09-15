// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"
#include "PlatformerGameInstance.h"
#include "PlatformerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MASTERINGUNREAL_API APlatformerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APlatformerPlayerController();

	//to hold reference to our UMG HUD widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Platformer Player Controller")
		TSubclassOf<class UUserWidget> cHUD;

	UUserWidget* HUD;

	//to hide or show player list
	UPROPERTY(BlueprintReadOnly, Category = "Platformer Player Controller")
		bool bPlayerListDisplayed;
	//to hide or show pause menu
	UPROPERTY(BlueprintReadOnly, Category = "Platformer Player Controller")
		bool bPauseMenuDisplayed;

	EInputMode modeBeforePause;
	bool bShowCursorBeforePause;

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void TogglePauseMenu();


	void ShowPlayerList();
	void HidePlayerList();

	UFUNCTION(BlueprintCallable, Category = "Platformer Player Controller")
		void HideAllMenus();
};
