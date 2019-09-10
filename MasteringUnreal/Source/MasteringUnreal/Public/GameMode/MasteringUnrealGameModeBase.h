// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MasteringUnrealGameModeBase.generated.h"

/**
 * 
 */
/* GameMode Ŭ������ �������� �����Ѵ�.
�ַ� ��Ʈ��ũ ���� ��ɵ��� �ٷ�� ���� å������.
����, �÷��̾��� ����� ������, 
GameState������ ��ȭ�� Ž��.(��ġ�� �����ٴ� �� �˸��� ��)*/

/* GameMode Ŭ������ GameModeBaseŬ������ �ٸ�����, 
GameMode�� ���� ��ġ���� ������ ��Ƽ�÷��̾ �ѹ��� ����ɶ� ��� ������ ��ɵ���
��� �غ�Ǿ��ִ�.
�ݸ� GameModeBase�� ����Ѵٸ� ��ġ�� ��Ʈ���ϴ� ��ɵ��� ���� ���� ����ؾߵȴ�.
����, ���� ���������� �� �÷��̾ ������ ��ٷ��� �ǰ�, �� �÷��̾ �غ� �ؾ� ��ġ�� ���� �ȴٸ�,
GameMode�� �װ͵��� �����ϱ� ���� �������� ������ ������ GameState�� �Բ� �� �۵��ȴ�.
�߰�������, �÷��̾ ��Ŀ���õǴ� ��Ȳ��� �ʿ��� ��ɵ鵵 ���߰��ִ�.
(�÷��̾���� ������ �������ִٰ� �� ����� �׵鿡�� �����ִµ�)*/

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
