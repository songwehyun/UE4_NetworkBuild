// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Powerup.h"
#include "BaseCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class TraceDirection;

UCLASS()
class MASTERINGUNREAL_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	/* 게임 원소들*/
	//파워업을 가지고 있을 배열
	TArray<APowerup*> PowerUps;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Base Character")
		bool bIsFiring;
	
	/* 컴포넌트 */
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		UCameraComponent* Camera;

	//전방방향 확인을 위한 화살표
	UPROPERTY(EditDefaultsOnly, Category = "Base Character")
		UArrowComponent* TraceDirection;

	/* 카메라 이동/입력 함수*/
	void MoveForward(float amount);

	void MoveRight(float amount);

	void RotateCamera(float amount);

	void ChangeCameraHeight(float amount);

	/* 픽업 함수 */
	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void CollectCoin();
	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void CollectHeart();

	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void CollectPowerup(APowerup* powerup);

	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void NextPowerup();

	/*데미지 함수*/
	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void ReceiveDamage(int amount);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* 파워업 함수*/
	UFUNCTION(BlueprintCallable, Category = "Base Character")
		void UsePowerupStartClient();

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "Base Character")
		void UsePowerupStartServer();

	UFUNCTION(BlueprintCallable, Server, WithValidation, Reliable, Category = "Base Character")
		void PowerupUsed();
private:
	int SelectedPowerupIndex;
};
