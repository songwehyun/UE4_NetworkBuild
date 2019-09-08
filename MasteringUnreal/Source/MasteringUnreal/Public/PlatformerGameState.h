// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PlatformerGameState.generated.h"

/**
 * 
 */

/*GameStateŬ������ GameModeŬ������ ���� ������ �Ǹ�, 
��� Ŭ���̾�Ʈ�� ���ø�����Ʈ�� ��� ������Ʈ���� ������ �ְ�ް� �����ϸ� 
��ġ ������ ���� �����Ѵ�.
���� GameStateBaseŬ������ �⺻������ �÷��̾�迭�� ������ �ִµ�, 
�ڵ������� GameStateBase�� ��� ����� �÷��̾���� �����ϱ� �����̴�.
GameStateŬ������ ƽ �Լ��� �����ڸ� ������ �ִµ� �̴� �Ŀ� 
�ڵ带 �����°��� ���ϱ� �����̴�.*/
UCLASS()
class MASTERINGUNREAL_API APlatformerGameState : public AGameStateBase
{
	GENERATED_BODY()
public:

	APlatformerGameState();

	virtual void Tick(float DeltaTime) override;
};
