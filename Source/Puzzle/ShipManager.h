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
	// Number of ships to be spawned
	UPROPERTY()
		int NumberOfShips;

	
	// Array of ship classes
	UPROPERTY()
		//TArray<AShip> ShipClasses;
		TArray<TSubclassOf<AShip>> ShipClasses;
	
	// Pointer to the board
	UPROPERTY()
		TWeakObjectPtr<ABattleShipBoard> BattleShipBoardPtr;

	// Spawns the ship in a correct place
	void SpawnRandomShip(TSubclassOf<AShip> ShipType);
	
};
