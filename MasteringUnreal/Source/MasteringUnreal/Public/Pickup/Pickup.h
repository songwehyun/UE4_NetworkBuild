// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class ABaseCharacter;
/*
UFUNCTION��ũ�θ� ����ϸ�
�������Ʈ���� ��밡���ϰ� �ϱ�����
�𸮾� ������ FunctionName_Implementaion delegate�ڵ带 �ڵ������� �����Ѵ�.

�̴� Remote Procedure Call, RPC�� ���鶧�� ������ �۾��� ���־�ߵȴ�.
���� �Լ��� �����Ҷ� _Implementation�� �Լ� ���� �ٿ����� �Ѵ�.
�׿����� �����Ͱ� �����ϰ� �ǵ��� �ľ��Ͽ� �Լ��� �����Ѵ�.

�����Լ��� �����ϰԵɶ�, WithValidation�±׸� �����ؾ��Ѵ�.
�� �±״� �𸮾������� FunctionName_Validate()�Լ��� ã�� ���� �Լ��� ȣ�Ⱑ���ϴ��� �ƴ�������,
�ѹ� �� Ȯ���ϴ� ������ ��ģ��.
�̴� ������ FucntionName_Implementation delegate�ڵ带 �ڵ� �����ϴ� �Ͱ� ����
�� �ٸ� delegate�̴�.
�Ϲ������� �� �Լ����� return true�� ���ǵȴ� */

UCLASS()
class MASTERINGUNREAL_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	/* ������Ʈ */
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		USphereComponent* CollisionSphere;
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		float SpinsPerSecond;

	//pickup�� �ٸ� ���Ϳ� overlap�ɶ� ȣ��
	UFUNCTION()
		void OnComponentOverlap(UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor, UPrimitiveComponent* Comp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//�Ⱦ��� ���������� ���������� ȣ��Ǵ� ���� �Լ�.
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
