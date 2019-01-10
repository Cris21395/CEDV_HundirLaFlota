// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleShipTurn.h"
#include "BattleShipBoard.h"
#include "IARobot.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattleShipPlayerController.generated.h"

/**
 * Custom class that collects the commands given by the player
 */
UCLASS()
class PUZZLE_API ABattleShipPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABattleShipPlayerController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	// Reference to the IARobot in the world
	UPROPERTY()
		TWeakObjectPtr<AIARobot> Robot;

	// The turn that is playing now is stored
	EBattleShipTurn currentTurn;

	// Function called in block after a click to change the turn
	void ChangeTurn();

private:
	// Countdown for changing the turn
	UPROPERTY()
		float DelayToChangeTurn;

	// Time spent during robot turn
	UPROPERTY()
		float AccumulatedDeltaTime;

	// Pointer to player board to change the clickability
	UPROPERTY()
		TWeakObjectPtr<ABattleShipBoard> BattleShipPlayerBoardPtr;

	// Pointer to machine board to change the clickability
	UPROPERTY()
		TWeakObjectPtr<ABattleShipBoard> BattleShipMechineBoardPtr;

	// Decides if it is allowed to receive input
	void SetReceiveInput(bool bReceiveInput);
};
