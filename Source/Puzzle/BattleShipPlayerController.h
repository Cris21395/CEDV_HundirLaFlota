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

	// Stored the winner when the game is finished, null otherwise
	EBattleShipTurn winner;

	// Function called in block after a click to change the turn
	void ChangeTurn();

	// Function that set the turn to NONE which means the game has ended
	void FinishGame();

private:
	// Countdown for changing the turn
	float DelayToChangeTurn;

	// Countdown for finishing the game
	float DelayToFinishGame;

	// Time spent during robot turn
	float AccumulatedDeltaTime;

	// Pointer to player board to change the clickability
	UPROPERTY()
		TWeakObjectPtr<ABattleShipBoard> BattleShipPlayerBoardPtr;

	// Pointer to machine board to change the clickability
	UPROPERTY()
		TWeakObjectPtr<ABattleShipBoard> BattleShipMachineBoardPtr;

	// Decides if it is allowed to receive input
	void SetReceiveInput(bool bReceiveInput);
};
