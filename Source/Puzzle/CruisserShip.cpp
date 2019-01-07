// Fill out your copyright notice in the Description page of Project Settings.

#include "CruisserShip.h"

ACruisserShip::ACruisserShip()
{
	// Set Mesh
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(TEXT("StaticMesh'/Game/Geometry/ShipMeshes/Ship4/CruiseLiner'"));
	ShipMesh->SetStaticMesh(BaseMeshAsset.Object);

	// Set Size
	Size = 4;
}

void ACruisserShip::DereferenceBlock(ABlock* Block)
{
	int32 BlockIndex = Block->BlockIndex;

	for (int i = 0; i < Size; i++)
	{
		int32 Index = OccupiedPositions[i];
		if (Index == BlockIndex)
		{
			// Remove this position in the array
			OccupiedPositions.Remove(Index);

			break;
		}
	}
}

void ACruisserShip::SetOccupiedBlocks(int32 SpawnIndex, ABattleShipBoard* BattleShipBoardPtr)
{
	for (int i = 0; i < Size; i++) 
	{
		// Mark occupied position of the ship in the board
		OccupiedPositions.Add(SpawnIndex + i);

		// Get the block that holds this ship
		ABlock* Block = BattleShipBoardPtr->GetBlockByIndex(SpawnIndex + i);

		// Bind the delegate to DereferenceBlock function
		Block->DereferenceBlockDelegate.BindDynamic(this, &ACruisserShip::DereferenceBlock);

		// Mark the block so that it is known that holds a ship
		Block->bHasShip = true;
	}
}
