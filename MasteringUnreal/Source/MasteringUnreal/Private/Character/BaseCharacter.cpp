// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "PlatformerPlayerState.h"
#include "UnrealNetwork.h"
#include "Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Engine/Classes/Components/ArrowComponent.h"
#include "Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Components/InputComponent.h"
#include "PlatformerPlayerController.h"
// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//카메라 고정을 위해 스프링암 셋업
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	//스프링암이 캐릭터와 함께 회전하지 않도록 함
	SpringArm->bUsePawnControlRotation = false;
	//캐릭터의 상대적인 회전값의 영향을 받지 않도록 함
	SpringArm->bAbsoluteRotation = true;
	//스프링암의 길이
	SpringArm->TargetArmLength = 700.0f;
	//루트에 붙임
	SpringArm->SetupAttachment(RootComponent);

	//카메라 셋업
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// 넓은 FOV 셋업
	Camera->FieldOfView = 110.0f;
	//스프링암에 붙임.
	Camera->SetupAttachment(SpringArm);

	//전방방향 셋업
	TraceDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Trace Direction"));
	//루트에 붙임
	TraceDirection->SetupAttachment(RootComponent);

	//기본 네트워크 레플리케이션 셋업
	bReplicates = true; 
	bAlwaysRelevant = true;
	bReplicateMovement = true;

	//캐릭터 무브먼드 변수 셋업

	//움직이는 방향으로 캐릭터를 회전하도록 함
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//컨트롤러 입력에 따른 회전을 하지 않도록 함
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.8f;

	//파워업이 선택되지 않은채로 시작. -1로 초기화시킴
	SelectedPowerupIndex = -1;

	//배열 초기화.
	PowerUps = TArray<APowerup*>();
}


/*
캐릭터가 선택된 파워업기능을 사용할때 애니매이션도 같이 실행되어야할것이다,
파워업을 가지고있다면 버튼을 누르는등 트리거가 켜져서 파워업기능을 사용하게 되었을때 애니메이션 이벤트 트리거가
Firing애니매이션을 시작하게 될것이고, 이를위해 파워업 서버함수가 사용될것이다.
이는 서버에서 미리 결정해야 할것이며 이를 위해서 bIsFiring 변수와 서버함수들을 사용하고 있다.
*/
void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//모든 캐릭터에 레플리케이트
	DOREPLIFETIME(ABaseCharacter, bIsFiring);
}

void ABaseCharacter::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (auto &powerup : PowerUps)
	{
		powerup->Destroy();
	}
}

void ABaseCharacter::UsePowerupStartClient()
{
	APlatformerPlayerController* PC = Cast<APlatformerPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC && PC->bPauseMenuDisplayed) return;
	//아직까지는 로컬 컨디션에 아무것도 체크를 하지 않는다. 이는 이후 화면멈춤상황에서 스킬을 사용하고있거나하는 상황들에서사용할것이다.
	//check local conditions

	//call powerup use on server
	UsePowerupStartServer();
}

void ABaseCharacter::UsePowerupStartServer_Implementation()
{
	//firing이 아니지만 파워업이 있는경우
	if (!bIsFiring && SelectedPowerupIndex >= 0)
	{
		bIsFiring = true;
	}
}
bool ABaseCharacter::UsePowerupStartServer_Validate()
{
	return true;
}
void ABaseCharacter::PowerupUsed_Implementation()
{
	//firing인 경우
	if (bIsFiring)
	{
		//이때의 애니매이션에서 손의 위치를 가져온다
		FVector HandLocation = GetMesh()->GetBoneLocation(TEXT("LeftHandMiddle1"));

		//powerup이 유효한지 다시 한번 확인한다.
		if (SelectedPowerupIndex >= 0)
		{
			APlatformerPlayerState* PS = Cast<APlatformerPlayerState>(GetPlayerState());

			if (PS)
			{
				PS->SelectedPowerup->UsePowerup(this, HandLocation, TraceDirection->GetForwardVector());
			}
		}

		bIsFiring = false;
	}
}
bool ABaseCharacter::PowerupUsed_Validate()
{
	return true;
}
void ABaseCharacter::ReceiveDamage_Implementation(int amount)
{
	APlatformerPlayerState* PS = Cast<APlatformerPlayerState>(GetPlayerState());

	if (PS)
	{
		PS->ReceiveDamage(amount);
	}
}

void ABaseCharacter::CollectPowerup(APowerup* powerup)
{
	//파워업을 인벤토리에 추가
	PowerUps.Add(powerup);

	//파워업을 캐릭터에 상속; (후에 자식을위해서)
	powerup->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	//파워업을 가지지 않았다면, 이 파워업을 선택
	if (SelectedPowerupIndex < 0)
			NextPowerup();
}
void ABaseCharacter::NextPowerup()
{
	//배열이 비어있지않다면.
	if (PowerUps.Num() != 0)
	{
		SelectedPowerupIndex++;

		//배열인덱스가 파워업크기보다 크다면 배열에 처음부분으로 초기화.
		if (SelectedPowerupIndex >= PowerUps.Num())
		{
			SelectedPowerupIndex = 0;
		}

		//player state 에게 새 파워업이 선택되었다고 알림.
		APlatformerPlayerState* PS = Cast<APlatformerPlayerState>(GetPlayerState());

		if (PS)
		{
			PS->SelectedPowerup = PowerUps[SelectedPowerupIndex];
		}
	}
}


void ABaseCharacter::MoveForward(float amount)
{
	APlatformerPlayerController* PC = Cast<APlatformerPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC && PC->bPauseMenuDisplayed) return;

	//카메라가 바라보는 방향으로 무브먼트 추가.
	if (Controller && amount)
	{
		AddMovementInput(SpringArm->GetForwardVector(), amount);
	}
}

void ABaseCharacter::MoveRight(float amount)
{
	APlatformerPlayerController* PC = Cast<APlatformerPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC && PC->bPauseMenuDisplayed) return;
	//카메라 우측방향으로 입력 추가.
	if (Controller && amount)
	{
		AddMovementInput(SpringArm->GetRightVector(), amount);
	}
}

void ABaseCharacter::RotateCamera(float amount)
{
	APlatformerPlayerController* PC = Cast<APlatformerPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC && PC->bPauseMenuDisplayed) return;
	//스프링암 z 축 회전
	if (Controller && amount)
	{
		FVector Rot = SpringArm->GetComponentRotation().Euler(); 
		Rot += FVector(0, 0, amount);
		SpringArm->SetRelativeRotation(FQuat::MakeFromEuler(Rot));
	}
}

void ABaseCharacter::ChangeCameraHeight(float amount)
{
	APlatformerPlayerController* PC = Cast<APlatformerPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PC && PC->bPauseMenuDisplayed) return;
	//스프링암의 y 축 회전. Clamp -45 ~ -5. ( 주어진 값이 하한보다 작으면 하한을,상한보다 크면 상한을 돌려주고 그 외는 주어진 값을 돌려준다.)

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

	//코인이 컬렉트될때 레플리케이트된 변수가 서버에서 관리되게된다.
	//서버만 함수를 호출하고 있다는 것을 명심해야된다. 이 과정중 Authority 체크를 여러번 하면서 이 과정이 true임을 확실시 한다.
	//여기까지의 과정은 안전하다.
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
	
	//캐릭터 뒷편으로 스프링암 위치 조정.
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

	//입력 바인드
	InputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("UsePowerUp", EInputEvent::IE_Pressed, this, &ABaseCharacter::UsePowerupStartClient);

	InputComponent->BindAxis("ChangeCameraHeight", this, &ABaseCharacter::ChangeCameraHeight);
	InputComponent->BindAxis("RotateCamera", this, &ABaseCharacter::RotateCamera);
	InputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
}

