// Fill out your copyright notice in the Description page of Project Settings.

#include "VesselShip.h"

AVesselShip::AVesselShip()
{
	// Set Mesh
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);
	ShipMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>
		(TEXT("StaticMesh'/Game/Geometry/ShipMeshes/Ship2/model'")).Object);

	// Set Size
	Size = 2;
}

void AVesselShip::DereferenceBlock(ABlock* Block)
{
	// Fire explosion
	FireExplosionIfItIsHit(Block);

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

void AVesselShip::SetOccupiedBlocks(int32 SpawnIndex, ABattleShipBoard* BattleShipBoardPtr)
{
	for (int i = 0; i < Size; i++) 
	{
		// Mark occupied position of the ship in the board
		OccupiedPositions.Add(SpawnIndex + BattleShipBoardPtr->Size*i);

		// Get the block that holds this ship
		ABlock* Block = BattleShipBoardPtr->GetBlockByIndex(SpawnIndex + BattleShipBoardPtr->Size*i);

		// Bind the delegate to DereferenceBlock function
		Block->DereferenceBlockDelegate.BindUObject(this, &AVesselShip::DereferenceBlock);

		// Mark the block so that it is known that holds a ship
		Block->bHasShip = true;
	}
}
