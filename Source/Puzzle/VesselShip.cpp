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

