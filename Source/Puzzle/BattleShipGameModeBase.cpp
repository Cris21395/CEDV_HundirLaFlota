// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipGameModeBase.h"
#include "BattleShipPlayerController.h"
#include "BattleShipGameStateBase.h"
#include "BattleShipHUD.h"
#include "EngineMinimal.h"

ABattleShipGameModeBase::ABattleShipGameModeBase() : MaxNumberPlayers(2)
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ABattleShipPlayerController::StaticClass();
	GameStateClass = ABattleShipGameStateBase::StaticClass();
	HUDClass = ABattleShipHUD::StaticClass();
}

void ABattleShipGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

bool ABattleShipGameModeBase::HasWon(ABattleShipBoard * Board)
{
	for (auto& Ship : Board->Ships) {
		AShip* ShipActor = Ship.GetDefaultObject();
		UE_LOG(LogTemp, Log, TEXT("--------[GAMEMODE] >> %d -----------\n\n\n"), ShipActor->OccupiedPositions.Num());
		if (ShipActor->OccupiedPositions.Num() != 0)
			return false;
	}
	return true;
}

