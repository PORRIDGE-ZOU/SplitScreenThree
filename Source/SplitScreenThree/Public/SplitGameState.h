// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "SplitGameState.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSCREENTHREE_API ASplitGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UPROPERTY(ReplicatedUsing=OnRep_Targets)
	APawn* LeftPawn = nullptr;

	UPROPERTY(ReplicatedUsing=OnRep_Targets)
	APawn* RightPawn = nullptr;

	DECLARE_MULTICAST_DELEGATE(FOnTargetsChanged);
	FOnTargetsChanged OnTargetsChanged;

protected:
	UFUNCTION()
	void OnRep_Targets() { OnTargetsChanged.Broadcast(); }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override
	{
		Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		DOREPLIFETIME(ASplitGameState, LeftPawn);
		DOREPLIFETIME(ASplitGameState, RightPawn);
	}
};
