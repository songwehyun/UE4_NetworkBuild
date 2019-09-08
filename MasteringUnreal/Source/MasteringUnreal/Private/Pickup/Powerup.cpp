// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup.h"
#include "BaseCharacter.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"

void APowerup::CollectPickup_Implementation(ABaseCharacter* Character)
{
	if (Role != ROLE_Authority) return;
	
	if (Character)
	{
		Character->CollectPowerup(this);

		bIsPickedUp = true;

		//월드에 더이상 남지 않는 콜리전박스와 메시 해제.
		CollisionSphere->DestroyComponent();
		Mesh->DestroyComponent();
	}
}


void APowerup::UsePowerup(ABaseCharacter* Character, FVector LaunchLocation, FVector LaunchDirection)
{

}