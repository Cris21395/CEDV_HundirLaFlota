// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShipType.h"
#include "BattleShipBoard.h"
#include "Block.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ship.generated.h"

UCLASS()
class PUZZLE_API AShip : public AActor 
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShip();

	// Size of ship
	UPROPERTY()
		int32 Size;

	// Pointer to static mesh
	UPROPERTY()
		TWeakObjectPtr<UStaticMeshComponent> ShipMesh;

	// Array of occupied positions (index of blocks)
	UPROPERTY()
		TArray<int32> OccupiedPositions;

private:
	// Override in children to dereference the block which holds a ship
	virtual void DereferenceBlock(ABlock* Block) PURE_VIRTUAL(AShip::DereferenceBlock, );

public:	
	// Override in children to set occupied blocks for each ship type
	virtual void SetOccupiedBlocks(int32 SpawnIndex, ABattleShipBoard* BattleShipBoardPtr) PURE_VIRTUAL(AShip::SetOccupiedBlocks, );

	// Returns ShipMesh subobject
	FORCEINLINE class UStaticMeshComponent* GetShipMesh() const { return ShipMesh.Get(); }
};
