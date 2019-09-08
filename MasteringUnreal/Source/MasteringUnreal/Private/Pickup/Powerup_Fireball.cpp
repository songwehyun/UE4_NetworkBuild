// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup_Fireball.h"
#include "Engine/World.h"
#include "Fireball.h"

APowerup_Fireball::APowerup_Fireball() : APowerup()
{

}

void APowerup_Fireball::UsePowerup(ABaseCharacter* Character, FVector LaunchLocation, FVector LaunchDirection)
{
	AFireball* newFireBall = GetWorld()->SpawnActor<AFireball>(LaunchLocation, FRotator::ZeroRotator);
	
	newFireBall->TheOwner = (ACharacter*)(Character);
	newFireBall->CollisionSphere->AddForce(LaunchDirection * 5000);
}