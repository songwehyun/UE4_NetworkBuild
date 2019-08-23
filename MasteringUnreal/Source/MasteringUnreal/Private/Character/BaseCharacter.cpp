// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "PlatformerPlayerState.h"
#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Components/ArrowComponent.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/Components/InputComponent.h"
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ī�޶� ������ ���� �������� �¾�
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	//���������� ĳ���Ϳ� �Բ� ȸ������ �ʵ��� ��
	SpringArm->bUsePawnControlRotation = false;
	//ĳ������ ������� ȸ������ ������ ���� �ʵ��� ��
	SpringArm->bAbsoluteRotation = true;
	//���������� ����
	SpringArm->TargetArmLength = 700.0f;
	//��Ʈ�� ����
	SpringArm->SetupAttachment(RootComponent);

	//ī�޶� �¾�
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// ���� FOV �¾�
	Camera->FieldOfView = 110.0f;
	//�������Ͽ� ����.
	Camera->SetupAttachment(SpringArm);

	//������� �¾�
	TraceDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Trace Direction"));
	//��Ʈ�� ����
	TraceDirection->SetupAttachment(RootComponent);

	//�⺻ ��Ʈ��ũ ���ø����̼� �¾�
	bReplicates = true; 
	bAlwaysRelevant = true;
	bReplicateMovement = true;

	//ĳ���� ����յ� ���� �¾�

	//�����̴� �������� ĳ���͸� ȸ���ϵ��� ��
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//��Ʈ�ѷ� �Է¿� ���� ȸ���� ���� �ʵ��� ��
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.8f;
}


void ABaseCharacter::MoveForward(float amount)
{
	//ī�޶� �ٶ󺸴� �������� �����Ʈ �߰�.
	if (Controller && amount)
	{
		AddMovementInput(SpringArm->GetForwardVector(), amount);
	}
}

void ABaseCharacter::MoveRight(float amount)
{
	//ī�޶� ������������ �Է� �߰�.
	if (Controller && amount)
	{
		AddMovementInput(SpringArm->GetRightVector(), amount);
	}
}

void ABaseCharacter::RotateCamera(float amount)
{
	//�������� z �� ȸ��
	if (Controller && amount)
	{
		FVector Rot = SpringArm->GetComponentRotation().Euler(); 
		Rot += FVector(0, 0, amount);
		SpringArm->SetRelativeRotation(FQuat::MakeFromEuler(Rot));
	}
}

void ABaseCharacter::ChangeCameraHeight(float amount)
{
	//���������� y �� ȸ��. Clamp -45 ~ -5. ( �־��� ���� ���Ѻ��� ������ ������,���Ѻ��� ũ�� ������ �����ְ� �� �ܴ� �־��� ���� �����ش�.)

	if (Controller && amount)
	{
		FVector rot = SpringArm->GetComponentRotation().Euler();

		float originalHeight = rot.Y;
		originalHeight += amount;
		originalHeight = FMath::Clamp(originalHeight, -45.0f, -5.0f);

		rot = FVector(0, originalHeight, rot.Z);

		SpringArm->SetRelativeRotation(FQuat::MakeFromEuler(rot));
	}
}

void ABaseCharacter::CollectCoin()
{
	APlatformerPlayerState* PS = Cast<APlatformerPlayerState>(GetPlayerState());

	//������ �÷�Ʈ�ɶ� ���ø�����Ʈ�� ������ �������� �����ǰԵȴ�.
	//������ �Լ��� ȣ���ϰ� �ִٴ� ���� ����ؾߵȴ�. �� ������ Authority üũ�� ������ �ϸ鼭 �� ������ true���� Ȯ�ǽ� �Ѵ�.
	//��������� ������ �����ϴ�.
	if (PS)
	{
		PS->CollectCoin();
	}
}
void ABaseCharacter::CollectHeart()
{
	APlatformerPlayerState* PS = Cast<APlatformerPlayerState>(GetPlayerState());

	if (PS)
	{
		PS->CollectHeart();
	}
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//ĳ���� �������� �������� ��ġ ����.
	SpringArm->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, -25, 180)));
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�Է� ���ε�
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	InputComponent->BindAxis("ChangeCameraHeight", this, &ABaseCharacter::ChangeCameraHeight);
	InputComponent->BindAxis("RotateCamera", this, &ABaseCharacter::RotateCamera);
	InputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
}

