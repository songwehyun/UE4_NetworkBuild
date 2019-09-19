// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = CollisionBox;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(RootComponent);

	EndPosition = FVector(0, 0, 0);
	TimeOneDirection = 1.0f;
	TimeTravellingInDirection = 0.0f;
	bForward = true;
	DistanceToTravel = FVector(0, 0, 0);

	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	FVector StartPosition = GetActorLocation();

	DistanceToTravel = EndPosition - StartPosition;
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ROLE_Authority)
	{
		if (DistanceToTravel != FVector(0, 0, 0))
		{
			FVector DistanceThisFrame = DistanceToTravel * (DeltaTime / TimeOneDirection);

			FVector CurrentLocation = GetActorLocation();

			if (bForward)
			{
				CurrentLocation += DistanceThisFrame;
			}
			else
			{
				CurrentLocation -= DistanceThisFrame;
			}

			TimeTravellingInDirection += DeltaTime;

			if (TimeTravellingInDirection >= TimeOneDirection)
			{
				bForward = !bForward;

				TimeTravellingInDirection = 0.0f;
			}

			SetActorLocation(CurrentLocation);
		}
	}
}

