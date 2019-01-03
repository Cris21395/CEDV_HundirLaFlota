// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "BattleShipBoard.h"
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
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Geometry/CubeMeshes/Cube.Cube"))
			, Transparency_Material(TEXT("/Game/Geometry/CubeMeshes/Transparency_Material.Transparency_Material"))
			, Transparency_Blue_Material(TEXT("/Game/Geometry/CubeMeshes/Transparency_Blue_Material_Inst.Transparency_Blue_Material_Inst"))
			, Transparency_Yellow_Material(TEXT("/Game/Geometry/CubeMeshes/Transparency_Orange_Material_Inst.Transparency_Orange_Material_Inst"))
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
	BlockMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.15f));
	BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.Transparency_Blue_Material.Get());
	BlockMesh->SetupAttachment(DummyRoot.Get());

	// Add events
	BlockMesh->OnClicked.AddDynamic(this, &ABlock::BlockClicked);
	BlockMesh->OnBeginCursorOver.AddDynamic(this, &ABlock::BlockBeginMouseOver);
	BlockMesh->OnEndCursorOver.AddDynamic(this, &ABlock::BlockEndMouseOver);

	// Save a pointer to the orange material
	Transparency_Material = ConstructorStatics.Transparency_Material.Get();
	Transparency_Blue_Material = ConstructorStatics.Transparency_Blue_Material.Get();
	Transparency_Yellow_Material = ConstructorStatics.Transparency_Yellow_Material.Get();
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
	// Check we are not already active
	if (!bIsActive)
	{
		bIsActive = true;

		// Change material
		BlockMesh->SetMaterial(0, Transparency_Material.Get());
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
