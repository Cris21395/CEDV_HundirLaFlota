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


