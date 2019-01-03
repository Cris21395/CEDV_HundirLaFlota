// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"

class ABattleShipBoard;
class UMaterial;
class UMaterialInstance;

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

	// Are we currently active?
	bool bIsActive;

	// Pointer to white material used on the focused block
	UPROPERTY()
		TWeakObjectPtr<UMaterial> Transparency_Material;

	// Pointer to blue material used on inactive blocks
	UPROPERTY()
		TWeakObjectPtr<UMaterialInstance> Transparency_Blue_Material;

	// Pointer to orange material used on active blocks
	UPROPERTY()
		TWeakObjectPtr<UMaterialInstance> Transparency_Yellow_Material;

	// Board that owns us
	UPROPERTY()
		TWeakObjectPtr<ABattleShipBoard> OwningBoard;

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
