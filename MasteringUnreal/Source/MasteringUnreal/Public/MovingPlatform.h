// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
UCLASS()
class MASTERINGUNREAL_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

	UPROPERTY(EditDefaultsOnly, Category = "Moving Platform")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Moving Platform")
		UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, Category = "Moving Platform")
		FVector EndPosition;

	UPROPERTY(EditAnywhere, Category = "Moving Platform")
		float TimeOneDirection;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float TimeTravellingInDirection;

	bool bForward;

	FVector DistanceToTravel;
};
