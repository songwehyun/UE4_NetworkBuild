// Fill out your copyright notice in the Description page of Project Settings.


#include "Fireball.h"
#include "CanTakeDamage.h"
// Sets default values
AFireball::AFireball()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollisionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionSphere->SetNotifyRigidBodyCollision(true);
	CollisionSphere->OnComponentHit.AddDynamic(this, &AFireball::OnHitActor);
	CollisionSphere->SetIsReplicated(true);
	CollisionSphere->SetSphereRadius(16.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionSphere->SetSimulatePhysics(true);

	RootComponent = CollisionSphere;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'")).Object);
	Mesh->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(CollisionSphere);
	Mesh->SetRelativeLocation(FVector(0, 0, -16.0f));

	theMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Assets/Materials/M_Fire.M_Fire'")).Object;
	Mesh->SetMaterial(0, theMaterial);

	bReplicates = true;
	bReplicateMovement = true;
	bAlwaysRelevant = true;

	physMaterial = CreateDefaultSubobject<UPhysicalMaterial>(TEXT("Physical Material"));

	FireParticles = ConstructorHelpers::FObjectFinder<UParticleSystem>(TEXT("ParticleSystem'/Game/Assets/Particles/P_Fire.P_Fire'")).Object;

	FireParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System"));

	FireParticleComponent->SetTemplate(FireParticles);

	FireParticleComponent->bAutoActivate = true;

	FireParticleComponent->SetupAttachment(RootComponent);

	TotalLifeTime = 2.0f;
	TimeAlive = 0;
	TheOwner = nullptr;
}

// Called when the game starts or when spawned
void AFireball::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TotalLifeTime이 지나면 파이어볼 해제.

	TimeAlive += DeltaTime;

	if (TimeAlive >= TotalLifeTime)
	{
		this->Destroy();
	}
}

void AFireball::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//바운스등 을 위해 물리 프로퍼티 셋업 
	CollisionSphere->SetMassOverrideInKg(NAME_None, 0.1f, true);

	//항상 같은 높이에서 바운스
	physMaterial->bOverrideRestitutionCombineMode = true;
	physMaterial->Restitution = 1.0f;
	physMaterial->RestitutionCombineMode = EFrictionCombineMode::Max;

	//절대 느려지지 않음.
	physMaterial->bOverrideFrictionCombineMode = true;
	physMaterial->Friction = 0;
	physMaterial->FrictionCombineMode = EFrictionCombineMode::Max;

	//콜리전스피어에 피직스메테리얼 셋업.
	CollisionSphere->SetPhysMaterialOverride(physMaterial);
}


void AFireball::OnHitActor(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	ICanTakeDamage* Character = Cast<ICanTakeDamage>(OtherActor);
	ICanTakeDamage* Owner = Cast<ICanTakeDamage>(TheOwner);


	//파이어볼은 오너가 있고 오너는 히트 캐릭터가 아니란것을 확실시함.
	if (Owner && Character && Owner != Character)
	{
		Character->Execute_ReceiveDamage(OtherActor,1);
		this->Destroy();
	}
}
