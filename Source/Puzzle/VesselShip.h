// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "VesselShip.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_API AVesselShip : public AShip
{
	GENERATED_BODY()
	
public:
	AVesselShip();

	void SetOccupiedBlocks(int32 SpawnIndex, ABattleShipBoard* BattleShipBoardPtr) override;

private:
	void DereferenceBlock(ABlock* Block) override;
};
