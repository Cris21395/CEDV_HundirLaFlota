// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipManager.h"
#include "EngineUtils.h"
#include "BattleShipBoard.h"
#include "Block.h"

// Sets default values
AShipManager::AShipManager() : NumberOfShips(4)
{	
	// Store references to ships classes
	// ShipClasses.AddUnique(::StaticClass());
	// ShipClasses.AddUnique(::StaticClass());
	// ShipClasses.AddUnique(::StaticClass());
	// ShipClasses.AddUnique(::StaticClass());
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
	/* for (auto& TypeShip : ShipClasses) 
	{
		SpawnRandomShip(TypeShip);
	}*/

}

/* void AShipManager::SpawnShip(ABaseShip ShipType)
{
	int32 Size = BattleShipBoardPtr->Size;

	// Get random index
	int32 RandomIndex = FMath::Rand() % (Size * Size);

	// Logic to spawn each ship
	TWeakObjectPtr<ABlock> Block = BattleShipBoardPtr->GetBlockByIndex(RandomIndex);

	switch (// Depending on the type of ship)
	{
	case // Type of ship:
		// Spawn ship depending on the block it occupies
		break;
	case // Type of ship:
		// Spawn ship depending on the block it occupies
		break;
	case // Type of ship:
		// Spawn ship depending on the block it occupies
		break;
	case // Type of ship:
		// Spawn ship depending on the block it occupies
		break;
	}
}*/