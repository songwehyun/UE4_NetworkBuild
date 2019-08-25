// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCharacter.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "Engine/Classes/PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/Classes/Materials/Material.h"
#include "Engine/Classes/Particles/ParticleSystem.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Fireball.generated.h"

UCLASS()
class MASTERINGUNREAL_API AFireball : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireball();

	/* ÄÄÆ÷³ÍÆ® */
	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
		UStaticMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
		USphereComponent* CollisionSphere;
	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
		UPhysicalMaterial* physMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
		UMaterial* theMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
		UParticleSystem* FireParticles;
	UPROPERTY(EditDefaultsOnly, Category = "Fireball")
		UParticleSystemComponent* FireParticleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fireball")
		ACharacter* TheOwner;

	float TotalLifeTime;
	float TimeAlive;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnHitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpulse, const FHitResult& Hit);

};
