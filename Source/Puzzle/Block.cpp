// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "BattleShipBoard.h"
#include "EngineMinimal.h"
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
		ConstructorHelpers::FObjectFinderOptional<USoundBase> AudioExplosion;
		ConstructorHelpers::FObjectFinderOptional<UParticleSystem> ExplosionParticleSystem;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Geometry/CubeMeshes/Cube.Cube"))
			, Transparency_Material(TEXT("/Game/Geometry/CubeMeshes/Transparency_Material.Transparency_Material"))
			, Transparency_Blue_Material(TEXT("/Game/Geometry/CubeMeshes/Transparency_Blue_Material_Inst.Transparency_Blue_Material_Inst"))
			, Transparency_Yellow_Material(TEXT("/Game/Geometry/CubeMeshes/Transparency_Orange_Material_Inst.Transparency_Orange_Material_Inst"))
			, AudioExplosion(TEXT("/Game/StarterContent/Audio/Explosion01.Explosion01"))
			, ExplosionParticleSystem(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"))
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

	// Register events
	BlockMesh->OnClicked.AddDynamic(this, &ABlock::BlockClicked);
	BlockMesh->OnBeginCursorOver.AddDynamic(this, &ABlock::BlockBeginMouseOver);
	BlockMesh->OnEndCursorOver.AddDynamic(this, &ABlock::BlockEndMouseOver);

	// Save a pointer to these materials
	Transparency_Material = ConstructorStatics.Transparency_Material.Get();
	Transparency_Blue_Material = ConstructorStatics.Transparency_Blue_Material.Get();
	Transparency_Yellow_Material = ConstructorStatics.Transparency_Yellow_Material.Get();

	// Save pointer to audio explosion
	AudioExplosion = ConstructorStatics.AudioExplosion.Get();

	// Save a pointer to explosion
	ExplosionParticleSystem = ConstructorStatics.ExplosionParticleSystem.Get();
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
	if (!bIsPressed)
	{
		bIsPressed = true;

		// Change material
		BlockMesh->SetMaterial(0, Transparency_Material.Get());

		// If block has a ship, we fire a explosion
		if (bHasShip)
		{
			// Play audio explosion
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), AudioExplosion.Get(), GetActorLocation());

			// Fire explosion particle system
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleSystem.Get(), GetActorLocation());

			// Call delegate if it is bound
			DereferenceBlockDelegate.ExecuteIfBound(this);

			bHasShip = false;
		}
	}
}

void ABlock::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsPressed)
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
