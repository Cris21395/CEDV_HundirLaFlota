// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipManager.h"
#include "EngineUtils.h"
#include "BattleShipBoard.h"
#include "Block.h"

// Sets default values
AShipManager::AShipManager()
{	
	// Store references to ships classes
	ShipClasses.AddUnique(ACruisserShip::StaticClass());
	ShipClasses.AddUnique(ASubmarineShip::StaticClass());
	ShipClasses.AddUnique(AVesselShip::StaticClass());
	ShipClasses.AddUnique(ABoatShip::StaticClass());
}

// Called when the game starts or when spawned
void AShipManager::BeginPlay()
{
	Super::BeginPlay();
	
	FString BattleBoardPlayerString = FString(TEXT("BattleShipBoardPlayer"));

	// Get a reference of board to spawn ships
	for (TActorIterator<ABattleShipBoard> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (BattleBoardPlayerString.Equals(ActorItr->GetName()))
		{
			// Conversion to smart pointer
			BattleShipBoardPtr = *ActorItr;
			break;
		}
	}

	// Spawn ships
	for (auto& TypeShip : ShipClasses) 
	{
		SpawnRandomShip(TypeShip);
	}

}

void AShipManager::SpawnRandomShip(TSubclassOf<AShip> ShipType)
{
	int32 Size = BattleShipBoardPtr->Size;

	// Get random index
	int32 RandomIndex = FMath::Rand() % (Size * Size);

	// Logic to spawn each ship
	TWeakObjectPtr<ABlock> Block = BattleShipBoardPtr->GetBlockByIndex(RandomIndex);

	// Location of Block
	FVector Blocklocation = Block->GetActorLocation();

	// IsChildOf compares if Class == OtherClass or Class == Child of Other Class
	if (ShipType->IsChildOf(ABoatShip::StaticClass())) {
		// Spawn boat
		TWeakObjectPtr<ABoatShip> NewShip = GetWorld()->SpawnActor<ABoatShip>(Blocklocation, FRotator(0.0f, 45.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(50.0f, 50.0f, 50.0f));
	}
	else if (ShipType->IsChildOf(AVesselShip::StaticClass())) {
		// Spawn Vessel
		Blocklocation += FVector(60.0f , 0.0f, 100.0f); // Offset due to the scale
		TWeakObjectPtr<AVesselShip> NewShip = GetWorld()->SpawnActor<AVesselShip>(Blocklocation, FRotator(0.0f, 0.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(75.0f, 75.0f, 75.0f));
	}
	else if (ShipType->IsChildOf(ASubmarineShip::StaticClass())) {
		// Spawn Submarine
		Blocklocation += FVector(0.0f, 100.0f, 0.0f); // Offset due to the scale
		TWeakObjectPtr<ASubmarineShip> NewShip = GetWorld()->SpawnActor<ASubmarineShip>(Blocklocation, FRotator(0.0f, 0.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(30.0f, 30.0f, 30.0f));
	}
	else if(ShipType->IsChildOf(ACruisserShip::StaticClass())){
		// Spawn Cruiser
		Blocklocation += FVector(0.0f, 50.0f, 0.0f); // Offset due to the scale
		TWeakObjectPtr<ACruisserShip> NewShip = GetWorld()->SpawnActor<ACruisserShip>(Blocklocation, FRotator(0.0f, 0.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(12.0f, 12.0f, 12.0f));
	}
}