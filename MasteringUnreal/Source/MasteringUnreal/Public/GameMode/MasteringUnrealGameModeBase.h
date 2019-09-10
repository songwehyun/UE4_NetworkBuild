// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MasteringUnrealGameModeBase.generated.h"

/**
 * 
 */
/* GameMode 클래스는 서버에만 존재한다.
주로 네트워크 게임 기능들을 다루는 것을 책임진다.
예로, 플레이어의 사망과 리스폰, 
GameState에서의 변화를 탐지.(매치가 끝났다는 걸 알리는 등)*/

/* GameMode 클래스와 GameModeBase클래스가 다른점은, 
GameMode는 같은 매치에서 수많은 멀티플레이어가 한번에 연결될때 사용 가능한 기능들이
모두 준비되어있다.
반면 GameModeBase를 사용한다면 매치를 컨트롤하는 기능들을 직접 만들어서 사용해야된다.
예로, 나의 협동게임이 두 플레이어가 들어오길 기다려야 되고, 두 플레이어가 준비를 해야 매치가 시작 된다면,
GameMode는 그것들을 구현하기 위한 정보들을 가지고 있으며 GameState와 함께 잘 작동된다.
추가적으로, 플레이어가 디스커넥팅되는 상황등에서 필요한 기능들도 갖추고있다.
(플레이어들의 정보들 가지고있다가 재 연결시 그들에게 돌려주는등)*/

UCLASS()
class MASTERINGUNREAL_API AMasteringUnrealGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMasteringUnrealGameModeBase();
	virtual void Logout(AController* exiting) override;
	virtual void Tick(float DeltaTime) override;

	//for spawning appropriate character
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
};
