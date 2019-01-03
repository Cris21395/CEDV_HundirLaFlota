// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"



// Sets default values
AShip::AShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Set default type
	Type = EShipType::BOAT;

	// Set default mesh
	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(TEXT("StaticMesh'/Game/Geometry/ShipMeshes/Ship1/PUSHILIN_Kayak'"));
	ShipMesh->SetStaticMesh(BaseMeshAsset.Object);
}

void AShip::SetType(EShipType NewType)
{
	Type = NewType;
}

// Called when the game starts or when spawned
void AShip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


