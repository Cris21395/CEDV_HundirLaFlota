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

