// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseCharacter.h"
#include "PlatformerGameState.generated.h"

/* ENUM FOR DEFINING MATCH STATE */
UENUM(BlueprintType)
enum class EMatchState : uint8 
{
	ENotConnected				UMETA(DisplayName = "Not Connected"),
	EWaitingForPlayers			UMETA(DisplayName = "Waiting For Players"),
	EStartingGame				UMETA(DisplayName = "Starting Game"),
	EGameInProgress				UMETA(DisplayName = "Game In Progress"),
	EGameComplete				UMETA(DisplayName = "Game Complete"),
	ERestartingGame				UMETA(DisplayName = "Restarting Game"),

};

/* STRUCT FOR DEFINING A CHARACTER FOR THE CHARACTER SELECT SCREEN */
USTRUCT(BlueprintType)
struct FCharacterSpecification 
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
		TSubclassOf<class ABaseCharacter> CharacterClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
		UTexture2D *CharacterIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
		FString CharacterName;
};
/**
 * 
 */

/*GameState클래스는 GameMode클래스로 부터 스폰이 되며, 
모든 클라이언트의 레플리케이트된 모든 오브젝트들의 변동을 주고받고 제어하며 
매치 정보들 또한 추적한다.
또한 GameStateBase클래스는 기본적으로 플레이어배열을 가지고 있는데, 
자동적으로 GameStateBase에 모든 연결된 플레이어들을 추적하기 위함이다.
GameState클래스는 틱 함수와 생성자만 가지고 있는데 이는 후에 
코드를 덮어씌우는것을 피하기 위함이다.*/
UCLASS()
class MASTERINGUNREAL_API APlatformerGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	APlatformerGameState();

	/* CLASS REFERENCES TO SCREENS TO BE DISPLAYED DURING A MATCH */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
		TSubclassOf<class UUserWidget> cWaitingForPlayers;

	//Character list we'll specify in the editor:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Match State")
		TArray<FCharacterSpecification> Characters;
	//For changing state
	UFUNCTION(NetMulticast, BlueprintCallable, Reliable, Category = "Match State")
		void ChangeState(EMatchState newState);

	//get current state
	UFUNCTION(BlueprintCallable, Category = "Match State")
		EMatchState GetMatchState();

	virtual void Tick(float DeltaTime) override;

private:
	//the currently displayed widget
	UUserWidget *CurrentWidget;
	//the current state, replicated
	UPROPERTY(Replicated, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
		EMatchState CurrentMatchState;
	//for changing state
	void EnterState(EMatchState newState);
	void LeaveState();
};
