// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipPlayerController.h"
#include "EngineUtils.h"

ABattleShipPlayerController::ABattleShipPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}

void ABattleShipPlayerController::BeginPlay()
{
	FString BattleBoardPlayerString = FString(TEXT("BattleShipBoardPlayer"));
	FString BattleBoardMachineString = FString(TEXT("BattleShipBoardMachine"));

	// Get a reference of board to spawn ships
	for (TActorIterator<ABattleShipBoard> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (BattleBoardPlayerString.Equals(ActorItr->GetName()))
		{
			// Conversion to smart pointer
			BattleShipPlayerBoardPtr = *ActorItr;
			break;
		}
		if (BattleBoardMachineString.Equals(ActorItr->GetName()))
		{
			// Conversion to smart pointer
			BattleShipMechineBoardPtr = *ActorItr;
			break;
		}
	}

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
	// If its the player turn
	if (currentTurn == EBattleShipTurn::PLAYER) {
		// No clickable so cannot keep presing blocks
		BattleShipMechineBoardPtr->bIsBoardClickable = true;
	} 
	// If is the machine turn
	else if (currentTurn == EBattleShipTurn::IA) {
		BattleShipMechineBoardPtr->bIsBoardClickable = false;
		// Introduce a little delay
		Robot->PlayTurn();
		//currentTurn = EBattleShipTurn::PLAYER;
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


