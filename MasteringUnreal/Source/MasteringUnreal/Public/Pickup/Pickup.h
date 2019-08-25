// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class ABaseCharacter;
/*
UFUNCTION매크로를 사용하면
블루프린트에서 사용가능하게 하기위해
언리얼 엔진은 FunctionName_Implementaion delegate코드를 자동적으로 생성한다.

이는 Remote Procedure Call, RPC를 만들때도 동일한 작업을 해주어야된다.
따라서 함수를 정의할때 _Implementation을 함수 끝에 붙여줘어야 한다.
그에따라 에디터가 적절하게 의도를 파악하여 함수에 접근한다.

서버함수를 정의하게될때, WithValidation태그를 포함해야한다.
이 태그는 언리얼엔진에게 FunctionName_Validate()함수를 찾아 만든 함수가 호출가능하던지 아닌지간에,
한번 더 확인하는 과정을 거친다.
이는 엔진이 FucntionName_Implementation delegate코드를 자동 생성하는 것과 같이
또 다른 delegate이다.
일반적으로 이 함수들은 return true로 정의된다 */

UCLASS()
class MASTERINGUNREAL_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	/* 컴포넌트 */
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		USphereComponent* CollisionSphere;
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		float SpinsPerSecond;

	//pickup이 다른 액터와 overlap될때 호출
	UFUNCTION()
		void OnComponentOverlap(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor, UPrimitiveComponent* Comp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//픽업이 실질적으로 수집됐을때 호출되는 서버 함수.
	UFUNCTION(Server, WithValidation, Reliable, Category = "Pickup")
		virtual void CollectPickup(ABaseCharacter* Character);

	virtual void CollectPickup_Implementation(ABaseCharacter* Character);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, EditDefaultsOnly ,Category = "Pickup")
		bool bIsPickedUp;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
