// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "EngineMinimal.h"
#include "BattleShipHUD.h"
#include "BattleShipBoard.h"
#include "BattleShipGameModeBase.h"
#include "BattleShipPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

ABlock::ABlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> Transparency_Material;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Transparency_Blue_Material;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Transparency_Yellow_Material;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> Transparency_Red_Material;
		ConstructorHelpers::FObjectFinderOptional<USoundBase> Audio_Splash_Water;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Geometry/CubeMeshes/Cube.Cube"))
			, Transparency_Material(TEXT("/Game/Geometry/CubeMeshes/Transparency_Material.Transparency_Material"))
			, Transparency_Blue_Material(TEXT("/Game/Geometry/CubeMeshes/Transparency_Blue_Material_Inst.Transparency_Blue_Material_Inst"))
			, Transparency_Yellow_Material(TEXT("/Game/Geometry/CubeMeshes/Transparency_Orange_Material_Inst.Transparency_Orange_Material_Inst"))
			, Transparency_Red_Material(TEXT("/Game/Geometry/CubeMeshes/Transparency_Red_Material_Inst.Transparency_Red_Material_Inst"))
			, Audio_Splash_Water(TEXT("/Game/Audio/Splash_Water.Splash_Water"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot.Get();

	// Create default static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.1f));
	BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.Transparency_Blue_Material.Get());
	BlockMesh->SetupAttachment(DummyRoot.Get());

	// Save a pointer to these materials
	Transparency_Material = ConstructorStatics.Transparency_Material.Get();
	Transparency_Blue_Material = ConstructorStatics.Transparency_Blue_Material.Get();
	Transparency_Yellow_Material = ConstructorStatics.Transparency_Yellow_Material.Get();
	Transparency_Red_Material = ConstructorStatics.Transparency_Red_Material.Get();

	// Save pointer to this audio
	AudioSplashWater = ConstructorStatics.Audio_Splash_Water.Get();
}

void ABlock::ReceiveInputFromMouse()
{
	// Register events
	BlockMesh->OnClicked.AddDynamic(this, &ABlock::BlockClicked);
	BlockMesh->OnBeginCursorOver.AddDynamic(this, &ABlock::BlockBeginMouseOver);
	BlockMesh->OnEndCursorOver.AddDynamic(this, &ABlock::BlockEndMouseOver);
}

void ABlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}

void ABlock::BlockBeginMouseOver(UPrimitiveComponent * MouseOverComp)
{
	Highlight(true);
}

void ABlock::BlockEndMouseOver(UPrimitiveComponent * MouseOverComp)
{
	Highlight(false);
}

void ABlock::HandleClicked()
{
	ABattleShipPlayerController* PlayerController = Cast<ABattleShipPlayerController>(GetWorld()->GetFirstPlayerController());
	ABattleShipHUD* HUD = Cast<ABattleShipHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// Check we are not already active
	if (!bIsActive)
	{
		bIsActive = true;

		// If block has a ship, we fire a explosion
		if (OwningShip != nullptr)
		{
			// Change material
			BlockMesh->SetMaterial(0, Transparency_Red_Material.Get());

			// Call delegate if it is bound
			DereferenceBlockDelegate.ExecuteIfBound(this);
		}
		else
		{
			// Change material
			BlockMesh->SetMaterial(0, Transparency_Material.Get());

			// Play splash water audio
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), AudioSplashWater.Get(), GetActorLocation());

			// Checks win conditions
			ABattleShipGameModeBase* GameMode = Cast<ABattleShipGameModeBase>(GetWorld()->GetAuthGameMode());
			// If you win
			if (GameMode->HasWon(OwningBoard.Get())) {
				UE_LOG(LogTemp, Log, TEXT("--------[BLOCK] >> WINNER FOUND -----------\n\n\n"));
				PlayerController->FinishGame();
			}
			else
			{
				// Change turn when player has not hit a ship
				PlayerController->ChangeTurn();
				HUD->ChangeTurn();
			}

			
		}
	}
}

void ABlock::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsActive)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, Transparency_Yellow_Material.Get());
	}
	else
	{
		BlockMesh->SetMaterial(0, Transparency_Blue_Material.Get());
	}
}
