// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BattleShipBoard.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleShipGameModeBase.generated.h"

/**
 * Custom class that defines the rules of the game
 */
UCLASS()
class PUZZLE_API ABattleShipGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABattleShipGameModeBase();

	virtual void BeginPlay() override;

	// Called to check whether the player has finished or not
	bool HasWon(ABattleShipBoard* Board);

private:
	int MaxNumberPlayers;
};
