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

		//���忡 ���̻� ���� �ʴ� �ݸ����ڽ��� �޽� ����.
		CollisionSphere->DestroyComponent();
		Mesh->DestroyComponent();
	}
}


void APowerup::UsePowerup(ABaseCharacter* Character, FVector LaunchLocation, FVector LaunchDirection)
{

}