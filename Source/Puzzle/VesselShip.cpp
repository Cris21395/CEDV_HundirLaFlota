// Fill out your copyright notice in the Description page of Project Settings.

#include "VesselShip.h"

AVesselShip::AVesselShip()
{
	// Set Mesh
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);
	ShipMesh->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>
		(TEXT("StaticMesh'/Game/Geometry/ShipMeshes/Ship2/model'")).Object);

	Size = 2;
}

void AVesselShip::DereferenceBlock(ABlock* Block)
{
	// Fire explosion
	FireExplosionIfItIsHit(Block);

	int32 BlockIndex = Block->BlockIndex;

	int32 Length = OccupiedPositions.Num();

	for (int i = 0; i < Length; i++)
	{
		int32 Index = OccupiedPositions[i];
		if (Index == BlockIndex)
		{
			// Remove this position in the array
			OccupiedPositions.Remove(Index);

			if (OccupiedPositions.Num() == 0)
			{
				DestroyedShipDelegate.ExecuteIfBound(this);
			}

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

		// This ship owns Block
		Block->OwningShip = this;

		// Bind the delegate to DereferenceBlock function
		Block->DereferenceBlockDelegate.BindUObject(this, &AVesselShip::DereferenceBlock);
	}
}
