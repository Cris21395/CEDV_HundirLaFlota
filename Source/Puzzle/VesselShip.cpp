// Fill out your copyright notice in the Description page of Project Settings.

#include "VesselShip.h"


AVesselShip::AVesselShip()
{
	// Set Mesh
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(TEXT("StaticMesh'/Game/Geometry/ShipMeshes/Ship2/model'"));
	ShipMesh->SetStaticMesh(BaseMeshAsset.Object);

	// Set Size
	Size = 2;
}

void AVesselShip::SetOccupiedBlocks(int32 SpawnIndex, TWeakObjectPtr<ABattleShipBoard> BattleShipBoardPtr)
{
	for (int i = 0; i < Size; i++) {
		OccupiedPositions.Add(SpawnIndex + BattleShipBoardPtr->Size*i);
		TWeakObjectPtr<ABlock> Block = BattleShipBoardPtr->GetBlockByIndex(SpawnIndex + BattleShipBoardPtr->Size*i);
		Block->bHasShip = true;
	}
}
