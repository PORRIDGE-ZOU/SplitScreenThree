// Fill out your copyright notice in the Description page of Project Settings.


#include "SplitGameMode.h"
#include "SplitGameState.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

void ASplitGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	PCs.AddUnique(NewPlayer);
	TryAssignTargets();
}

void ASplitGameMode::TryAssignTargets()
{
	if (PCs.Num() < 2) return;
	auto* GS = GetGameState<ASplitGameState>();
	if (!GS) return;

	APawn* P0 = PCs[0].IsValid() ? PCs[0]->GetPawn() : nullptr;
	APawn* P1 = PCs[1].IsValid() ? PCs[1]->GetPawn() : nullptr;
	if (!P0 || !P1) return;

	GS->LeftPawn  = P0;   // deterministic left/right
	GS->RightPawn = P1;
	GS->ForceNetUpdate();
}