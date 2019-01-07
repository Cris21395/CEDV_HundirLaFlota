// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "CruisserShip.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_API ACruisserShip : public AShip
{
	GENERATED_BODY()

public:
	ACruisserShip();
	
	void SetOccupiedBlocks(int32 SpawnIndex, ABattleShipBoard* BattleShipBoardPtr) override;

private:
	UFUNCTION()
		void DereferenceBlock(ABlock* Block) override;
};
