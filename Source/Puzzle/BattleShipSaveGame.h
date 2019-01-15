// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BattleShipSaveGame.generated.h"

#define SAVE_SLOT_NAME "BattleShipSaveSlot"
#define USER_INDEX 0

/**
 * Custom class to save the data of our game
 */
UCLASS()
class PUZZLE_API UBattleShipSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UBattleShipSaveGame();
	
	UPROPERTY(VisibleAnywhere, Category = Basic)
		TMap<FString, int> Records;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
};
