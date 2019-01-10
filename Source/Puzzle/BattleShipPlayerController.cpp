// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipPlayerController.h"
#include "EngineUtils.h"

ABattleShipPlayerController::ABattleShipPlayerController() : DelayToChangeTurn(3), AccumulatedDeltaTime(0)
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ABattleShipPlayerController::BeginPlay()
{
	// Get a reference to the IARobot
	for (TActorIterator<AIARobot> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		Robot = *ActorItr;
	}

	// Turn initialized in player
	currentTurn = EBattleShipTurn::PLAYER;
}

void ABattleShipPlayerController::Tick(float DeltaSeconds)
{
	// If it's the player turn
	if (currentTurn == EBattleShipTurn::PLAYER) 
	{
		// Enable events for player
	} 
	// If it's the machine turn
	else if (currentTurn == EBattleShipTurn::IA)
	{
		AccumulatedDeltaTime += DeltaSeconds;

		// Disable events for player

		if (AccumulatedDeltaTime >= DelayToChangeTurn)
		{
			Robot->PlayTurn();

			AccumulatedDeltaTime = 0.0f;
		}
	}

	// The game have finished
	else {

	}

	
}

void ABattleShipPlayerController::ChangeTurn()
{
	if (currentTurn == EBattleShipTurn::PLAYER)
		currentTurn = EBattleShipTurn::IA;
	else if (currentTurn == EBattleShipTurn::IA)
		currentTurn = EBattleShipTurn::PLAYER;
}