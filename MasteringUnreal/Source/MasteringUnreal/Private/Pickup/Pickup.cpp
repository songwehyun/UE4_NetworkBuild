// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "UnrealNetwork.h"
#include "BaseCharacter.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//콜리전 스피어 셋업
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	//콜리젼 활성화 물리,쿼리엔진
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//radius안에 들어올때 overapd이벤트 활성화
	CollisionSphere->SetGenerateOverlapEvents(true);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//replicates
	CollisionSphere->SetIsReplicated(true);
	//OnComponentOverlap함수를 OnComponentBeginOverlap delegate에 추가
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnComponentOverlap);
	//루트 컴포넌트로 셋업
	RootComponent = CollisionSphere;

	//메시 셋업
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	//메시의 모든 콜리전 제거
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//메시를 콜리전에 상속
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetIsReplicated(true);

	//스핀 스피드 초기화
	SpinsPerSecond = 0.5f;

	//디폴트에 픽업되지 않음
	bIsPickedUp = false;

	//레플리케이트 셋업
	bReplicates = true;
	bAlwaysRelevant = true;
}


void APickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APickup, bIsPickedUp);
}

void APickup::OnComponentOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, UPrimitiveComponent* Comp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor);

	if (Character)
	{
		CollectPickup(Character);
	}
}
void APickup::CollectPickup_Implementation(ABaseCharacter* Character)
{
	//서버에서 실행된다는것을 확실시함.
	if (Role != ROLE_Authority) return;

	bIsPickedUp = true;
}

bool APickup::CollectPickup_Validate(ABaseCharacter* Character)
{
	return true;
}
// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//픽업 상태가 아니라면 회전
	if (!bIsPickedUp)
	{
		float SpinThisFrame = (DeltaTime * SpinsPerSecond) * 360;
		
		FVector rotation = RootComponent->GetComponentRotation().Euler();
		rotation += FVector(0, 0, SpinThisFrame);
		RootComponent->SetWorldRotation(FQuat::MakeFromEuler(rotation));
	}
}

