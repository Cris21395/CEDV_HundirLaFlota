// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipPlayerController.h"
#include "EngineUtils.h"
#include "EngineMinimal.h"

ABattleShipPlayerController::ABattleShipPlayerController() : DelayToChangeTurn(1), AccumulatedDeltaTime(0)
{
	bShowMouseCursor = true;
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
		SetReceiveInput(true);
	} 
	// If it's the machine turn
	else if (currentTurn == EBattleShipTurn::IA)
	{
		AccumulatedDeltaTime += DeltaSeconds;

		// Disable events for player
		SetReceiveInput(false);

		if (AccumulatedDeltaTime >= DelayToChangeTurn)
		{
			Robot->PlayTurn();

			AccumulatedDeltaTime = 0.0f;
		}
	}

	// The game have finished
	else if(currentTurn == EBattleShipTurn::NONE){
		// Open new level
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/Records"), TRAVEL_Absolute);
	}

	
}

void ABattleShipPlayerController::ChangeTurn()
{
	if (currentTurn == EBattleShipTurn::PLAYER)
		currentTurn = EBattleShipTurn::IA;
	else if (currentTurn == EBattleShipTurn::IA)
		currentTurn = EBattleShipTurn::PLAYER;
}

void ABattleShipPlayerController::FinishGame()
{
	winner = currentTurn;
	currentTurn = EBattleShipTurn::NONE;
}

void ABattleShipPlayerController::SetReceiveInput(bool bReceiveInput)
{
	bEnableClickEvents = bReceiveInput;
	bEnableMouseOverEvents = bReceiveInput;
}
