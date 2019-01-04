// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstance.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

class ABattleShipBoard;

/**
* Block class used to create a board and be clickable
*/
UCLASS()
class PUZZLE_API ABlock : public AActor
{
	GENERATED_BODY()

	// Dummy root component
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TWeakObjectPtr<USceneComponent> DummyRoot;

	// StaticMesh component for the clickable block
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TWeakObjectPtr<UStaticMeshComponent> BlockMesh;

public:
	ABlock();

	// Is it pressed?
	bool bIsPressed;

	// Does it have a ship?
	bool bHasShip;

	// Board that owns this block
	UPROPERTY()
		TWeakObjectPtr<ABattleShipBoard> OwningBoard;

private:
	// Pointer to white material used on the focused block
	UPROPERTY()
		TWeakObjectPtr<UMaterial> Transparency_Material;

	// Pointer to blue material used on inactive blocks
	UPROPERTY()
		TWeakObjectPtr<UMaterialInstance> Transparency_Blue_Material;

	// Pointer to orange material used on active blocks
	UPROPERTY()
		TWeakObjectPtr<UMaterialInstance> Transparency_Yellow_Material;

	// Pointer to explosion used to be fired when a ship holded by this block has been hit
	UPROPERTY()
		TWeakObjectPtr<UParticleSystem> ExplosionParticleSystem;

	// Pointer to audio explosion used to be played along ExplosionParticleSystem
	UPROPERTY()
		TWeakObjectPtr<USoundBase> AudioExplosion;

	// Handle the block being clicked
	UFUNCTION()
		void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	// Handle the block when cursor begins to be over it
	UFUNCTION()
		void BlockBeginMouseOver(UPrimitiveComponent* MouseOverComp);

	// Handle the block when cursor ends to be over it
	UFUNCTION()
		void BlockEndMouseOver(UPrimitiveComponent* MouseOverComp);

		void HandleClicked();

		void Highlight(bool bOn);

public:
	// Returns DummyRoot subobject
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot.Get(); }

	// Returns BlockMesh subobject
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh.Get(); }
};
