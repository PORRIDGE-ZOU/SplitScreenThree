// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SplitGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSCREENTHREE_API ASplitGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	TArray<TWeakObjectPtr<APlayerController>> PCs;
	void TryAssignTargets();
};
