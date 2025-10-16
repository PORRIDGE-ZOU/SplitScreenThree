// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplitLocalViewManager.generated.h"
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class ASplitGameState;

UCLASS()
class SPLITSCREENTHREE_API ASplitLocalViewManager : public AActor
{
	GENERATED_BODY()
public:
	ASplitLocalViewManager();

	UPROPERTY(BlueprintReadWrite)
	USceneCaptureComponent2D* CapA;
	UPROPERTY(BlueprintReadWrite)
	USceneCaptureComponent2D* CapB;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTextureRenderTarget2D* RT_A;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTextureRenderTarget2D* RT_B;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable)
	UTextureRenderTarget2D* GetRTA() const { return RT_A; }
	UFUNCTION(BlueprintCallable)
	UTextureRenderTarget2D* GetRTB() const { return RT_B; }

private:
	TWeakObjectPtr<ASplitGameState> GS;
	TWeakObjectPtr<APawn> LeftPawn;
	TWeakObjectPtr<APawn> RightPawn;

	void RefreshTargets();
	void AlignCaptureToPawn(USceneCaptureComponent2D* Cap, APawn* P);
	void EnsureRTSizes();
};
