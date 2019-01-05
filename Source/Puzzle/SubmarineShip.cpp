// Fill out your copyright notice in the Description page of Project Settings.

#include "SubmarineShip.h"


ASubmarineShip::ASubmarineShip()
{
	//Set Mesh
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(TEXT("StaticMesh'/Game/Geometry/ShipMeshes/Ship3/submarine'"));
	ShipMesh->SetStaticMesh(BaseMeshAsset.Object);

	//Set Size
	Size = 3;
}

void ASubmarineShip::SetOccupiedBlocks(int32 SpawnIndex, TWeakObjectPtr<ABattleShipBoard> BattleShipBoardPtr)
{
	for (int i = 0; i < Size; i++) {
		OccupiedPositions.Add(SpawnIndex + i);
		TWeakObjectPtr<ABlock> Block = BattleShipBoardPtr->GetBlockByIndex(SpawnIndex + i);
		Block->bHasShip = true;
	}
}
