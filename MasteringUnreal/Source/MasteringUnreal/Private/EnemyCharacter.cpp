// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxHealth = 3;
	CurrentHelath = MaxHealth;

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemyCharacter::OnHitActor);

	bReplicateMovement = true;
	bAlwaysRelevant = true;
	bReplicates = true;

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::ReceiveDamage_Implementation(int amount)
{
	if (Role == ROLE_Authority)
	{
		CurrentHelath -= amount;

		if (CurrentHelath <= 0)
		{
			Destroy();
		}
	}
}


void AEnemyCharacter::OnHitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (Role == ROLE_Authority)
	{
		ICanTakeDamage* DamageActor = Cast<ICanTakeDamage>(OtherActor);

		if (DamageActor)
		{
			ACharacter* LaunchCharacter = Cast<ACharacter>(OtherActor);

			if (LaunchCharacter)
			{
				FVector LaunchForce = LaunchCharacter->GetActorLocation() - GetActorLocation();

				LaunchForce.Normalize();

				LaunchForce.Z = 1.0f;
				LaunchForce *= 1000;

				LaunchCharacter->LaunchCharacter(LaunchForce, true, true);
			}

			DamageActor->Execute_ReceiveDamage(OtherActor, 1);
		}
	}
}

