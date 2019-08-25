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

	//�ݸ��� ���Ǿ� �¾�
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	//�ݸ��� Ȱ��ȭ ����,��������
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//radius�ȿ� ���ö� overapd�̺�Ʈ Ȱ��ȭ
	CollisionSphere->SetGenerateOverlapEvents(true);
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//replicates
	CollisionSphere->SetIsReplicated(true);
	//OnComponentOverlap�Լ��� OnComponentBeginOverlap delegate�� �߰�
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnComponentOverlap);
	//��Ʈ ������Ʈ�� �¾�
	RootComponent = CollisionSphere;

	//�޽� �¾�
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	//�޽��� ��� �ݸ��� ����
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//�޽ø� �ݸ����� ���
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetIsReplicated(true);

	//���� ���ǵ� �ʱ�ȭ
	SpinsPerSecond = 0.5f;

	//����Ʈ�� �Ⱦ����� ����
	bIsPickedUp = false;

	//���ø�����Ʈ �¾�
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
	//�������� ����ȴٴ°��� Ȯ�ǽ���.
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

	//�Ⱦ� ���°� �ƴ϶�� ȸ��
	if (!bIsPickedUp)
	{
		float SpinThisFrame = (DeltaTime * SpinsPerSecond) * 360;
		
		FVector rotation = RootComponent->GetComponentRotation().Euler();
		rotation += FVector(0, 0, SpinThisFrame);
		RootComponent->SetWorldRotation(FQuat::MakeFromEuler(rotation));
	}
}

