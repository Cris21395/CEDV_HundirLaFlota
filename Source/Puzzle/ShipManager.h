// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Ship.h"
#include "BoatShip.h"
#include "VesselShip.h"
#include "SubmarineShip.h"
#include "CruisserShip.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShipManager.generated.h"

class ABattleShipBoard;

/**
* ShipManager class is responsible to manage ships
*/
UCLASS()
class PUZZLE_API AShipManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShipManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Array of ship classes
	UPROPERTY()
		TArray<TSubclassOf<AShip>> ShipClasses;

	// Spawns the ship in a correct place
	void SpawnRandomShip(ABattleShipBoard* BattleShipBoard, TSubclassOf<AShip> ShipType, bool bMustBoardBeVisible);

	// Functions to check positions
	bool IsValidIndex(int32 IndexToCheck, ABattleShipBoard* BattleShipBoard, TSubclassOf<AShip> ShipType);
	int32 GetShipSize(TSubclassOf<AShip> ShipType);

	// A ship cannot be placed out of the board
	bool CheckBoardBoundaries(int32 IndexToCheck, ABattleShipBoard* BattleShipBoard, TSubclassOf<AShip> ShipType);
	
	// Two ships cannot be in the same blocks
	bool CheckCollisions(int32 IndexToCheck, ABattleShipBoard* BattleShipBoard, TSubclassOf<AShip> ShipType);
	
	// There must be at least one block between ships
	bool CheckShipBoundaries(int32 IndexToCheck, ABattleShipBoard* BattleShipBoard, TSubclassOf<AShip> ShipType);
	
	//True if the indexed block if empty
	bool IsEmptyBlock(ABattleShipBoard* BattleShipBoard, int32 IndexToCheck);
};
