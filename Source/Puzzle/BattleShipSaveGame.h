// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BattleShipSaveGame.generated.h"

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
		TMap<FString, float> Records;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
};
