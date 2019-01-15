// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipGameModeBase.h"
#include "BattleShipPlayerController.h"
#include "BattleShipGameStateBase.h"
#include "Ship.h"
#include "BattleShipHUD.h"
#include "EngineMinimal.h"

ABattleShipGameModeBase::ABattleShipGameModeBase() : MaxNumberPlayers(2)
{
	PlayerControllerClass = ABattleShipPlayerController::StaticClass();
	GameStateClass = ABattleShipGameStateBase::StaticClass();
	HUDClass = ABattleShipHUD::StaticClass();
	DefaultPawnClass = nullptr;
}

void ABattleShipGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

bool ABattleShipGameModeBase::HasWon(ABattleShipBoard * Board)
{
	bool bHasWon = true;

	for (auto& ShipActor : Board->Ships) 
	{
		if (ShipActor->OccupiedPositions.Num() != 0) bHasWon = false;
	}
	return bHasWon;
}

