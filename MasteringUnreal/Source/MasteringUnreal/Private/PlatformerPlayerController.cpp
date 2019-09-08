// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerPlayerController.h"
#include "Engine/World.h"

APlatformerPlayerController::APlatformerPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	bPauseMenuDisplayed = false;
	bPlayerListDisplayed = false;


}

void APlatformerPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", EInputEvent::IE_Pressed, this, &APlatformerPlayerController::TogglePauseMenu);
	InputComponent->BindAction("ShowPlayerScreen", EInputEvent::IE_Pressed, this, &APlatformerPlayerController::ShowPlayerList);
	InputComponent->BindAction("ShowPlayerScreen", EInputEvent::IE_Released, this, &APlatformerPlayerController::HidePlayerList);
}

void APlatformerPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//to ensure this is locally owned playercontroller
	if (IsLocalController())
	{
		UPlatformerGameInstance* GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

		if (GameInstance)
		{
			if (GameInstance->GetGameState() == EGameState::EMultiplayerInGame)
			{
				if (HUD == nullptr)
				{
					if (cHUD)
					{
						HUD = CreateWidget<UUserWidget>(this, cHUD);
						HUD->AddToViewport();
					}
				}
			}
			else if (HUD != nullptr)
			{
				HUD->RemoveFromViewport();
				HUD = nullptr;
			}
		}
	}
}
void APlatformerPlayerController::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (HUD)
	{
		HUD->RemoveFromViewport();
		HUD = nullptr;
	}
}

void APlatformerPlayerController::TogglePauseMenu()
{
	bPauseMenuDisplayed = !bPauseMenuDisplayed;

	UPlatformerGameInstance* GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

	if (GameInstance)
	{
		if (bPauseMenuDisplayed)
		{
			if (bPlayerListDisplayed)
			{
				bPlayerListDisplayed = false;
			}
			//retain the old input mode
			modeBeforePause = GameInstance->CurrentInputMode;
			bShowCursorBeforePause = GameInstance->bIsShowingMouseCursor;

			//set input mode to UI and Game
			GameInstance->SetInputMode(EInputMode::EUIAndGame, true);
		}
		else
		{
			//restore old input mode
			GameInstance->SetInputMode(modeBeforePause, bShowCursorBeforePause);
		}
	}

}

void APlatformerPlayerController::ShowPlayerList()
{
	//PauseMenu일때 나오지 않도록 함.
	if (!bPauseMenuDisplayed)
	{
		bPlayerListDisplayed = true;

		//InputMode를 UI and Game으로 하여 플레이어들을 스크롤링 할 수 있게 함.
		UPlatformerGameInstance* GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

		if (GameInstance)
		{
			GameInstance->SetInputMode(EInputMode::EUIAndGame, true);
		}
	}
}
void APlatformerPlayerController::HidePlayerList()
{
	if (!bPauseMenuDisplayed)
	{
		bPlayerListDisplayed = false;

		UPlatformerGameInstance* GameInstance = Cast<UPlatformerGameInstance>(GetWorld()->GetGameInstance());

		if (GameInstance)
		{
			GameInstance->SetInputMode(EInputMode::EGameOnly, false);
		}
	}
}