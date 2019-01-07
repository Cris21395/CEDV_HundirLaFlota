// Fill out your copyright notice in the Description page of Project Settings.

#include "BoatShip.h"

ABoatShip::ABoatShip()
{
	// Set Mesh
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(TEXT("StaticMesh'/Game/Geometry/ShipMeshes/Ship1/PUSHILIN_Kayak'"));
	ShipMesh->SetStaticMesh(BaseMeshAsset.Object);

	// Set Size
	Size = 1;
}

void ABoatShip::DereferenceBlock(ABlock* Block)
{
	int32 BlockIndex = Block->BlockIndex;

	for (int i = 0; i < Size; i++)
	{
		int32 Index = OccupiedPositions[i];
		if (Index == BlockIndex)
		{
			// Remove this position in the array
			OccupiedPositions.Remove(Index);
		}
	}
}

void ABoatShip::SetOccupiedBlocks(int32 SpawnIndex, ABattleShipBoard* BattleShipBoardPtr)
{	
	// Mark occupied position of the ship in the board
	OccupiedPositions.Add(SpawnIndex);

	// Get the block that holds this ship
	ABlock* Block = BattleShipBoardPtr->GetBlockByIndex(SpawnIndex);

	// Bind the delegate to DereferenceBlock function
	Block->DereferenceBlockDelegate.BindDynamic(this, &ABoatShip::DereferenceBlock);

	// Mark the block so that it is known that holds a ship
	Block->bHasShip = true;
}
