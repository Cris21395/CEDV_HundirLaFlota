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


