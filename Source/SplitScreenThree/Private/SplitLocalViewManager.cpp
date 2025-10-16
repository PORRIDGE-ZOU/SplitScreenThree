#include "SplitLocalViewManager.h"
#include "SplitGameState.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/World.h"
#include "Engine/GameViewportClient.h"

ASplitLocalViewManager::ASplitLocalViewManager()
{
    PrimaryActorTick.bCanEverTick = true;

    CapA = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CapA"));
    CapB = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("CapB"));
    CapA->SetupAttachment(RootComponent);
    CapB->SetupAttachment(RootComponent);

    CapA->bCaptureEveryFrame = true;
    CapA->bCaptureOnMovement = true;
    CapB->bCaptureEveryFrame = true;
    CapB->bCaptureOnMovement = true;
    CapA->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
    CapB->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;

    // lock exposure
    auto Lock = [](USceneCaptureComponent2D* C){
        C->PostProcessSettings.bOverride_AutoExposureMethod = 1;
        C->PostProcessSettings.AutoExposureMethod = EAutoExposureMethod::AEM_Manual;
        C->PostProcessSettings.bOverride_AutoExposureBias = 1;
        C->PostProcessSettings.AutoExposureBias = 0.f;
    };
    Lock(CapA);
    Lock(CapB);
}

void ASplitLocalViewManager::BeginPlay()
{
    Super::BeginPlay();

    // Create RTs
    if (!RT_A || !RT_B)
    {
        RT_A = UKismetRenderingLibrary::CreateRenderTarget2D(this, 1920, 1080, RTF_RGBA16f);
        RT_B = UKismetRenderingLibrary::CreateRenderTarget2D(this, 1920, 1080, RTF_RGBA16f);
    }

    RT_A->bAutoGenerateMips = false; RT_A->SRGB = false;
    RT_B->bAutoGenerateMips = false; RT_B->SRGB = false;

    CapA->TextureTarget = RT_A;
    CapB->TextureTarget = RT_B;
    
    GS = GetWorld() ? GetWorld()->GetGameState<ASplitGameState>() : nullptr;
    if (GS.IsValid())
    {
        GS->OnTargetsChanged.AddLambda([this]()
        {
            RefreshTargets();
        });
        RefreshTargets();
    }
}

void ASplitLocalViewManager::RefreshTargets()
{
    if (!GS.IsValid()) return;
    LeftPawn  = GS->LeftPawn;
    RightPawn = GS->RightPawn;
}

void ASplitLocalViewManager::AlignCaptureToPawn(USceneCaptureComponent2D* Cap, APawn* P)
{
    if (!Cap || !P) return;

    if (auto Cam = P->FindComponentByClass<UCameraComponent>())
    {
        Cap->SetWorldLocationAndRotation(Cam->GetComponentLocation(), Cam->GetComponentRotation());
        Cap->FOVAngle = Cam->FieldOfView;
    }
    else
    {
        Cap->SetWorldLocation(P->GetPawnViewLocation());
        Cap->SetWorldRotation(P->GetViewRotation());
    }
}

void ASplitLocalViewManager::EnsureRTSizes()
{
    if (!GetWorld() || !GetWorld()->GetGameViewport()) return;
    const FIntPoint Sz = GetWorld()->GetGameViewport()->Viewport->GetSizeXY();
    auto Res = [&](UTextureRenderTarget2D* RT){
        if (!RT) return;
        if (RT->SizeX != Sz.X || RT->SizeY != Sz.Y)
            UKismetRenderingLibrary::ResizeRenderTarget2D( RT, Sz.X, Sz.Y);
    };
    Res(RT_A); Res(RT_B);
}

void ASplitLocalViewManager::Tick(float dt)
{
    Super::Tick(dt);
    EnsureRTSizes();

    AlignCaptureToPawn(CapA, LeftPawn.Get());
    AlignCaptureToPawn(CapB, RightPawn.Get());

    //if (CapA->TextureTarget) CapA->CaptureScene();
    //if (CapB->TextureTarget) CapB->CaptureScene();
}