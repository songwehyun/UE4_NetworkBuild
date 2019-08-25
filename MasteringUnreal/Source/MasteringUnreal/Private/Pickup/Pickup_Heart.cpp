// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup_Heart.h"
#include "BaseCharacter.h"

APickup_Heart::APickup_Heart()
{

}

void APickup_Heart::CollectPickup_Implementation(ABaseCharacter* Character)
{
	if (Role != ROLE_Authority) return;

	if (Character)
	{
		Character->CollectHeart();

		bIsPickedUp = true;

		this->Destroy();
	}
}