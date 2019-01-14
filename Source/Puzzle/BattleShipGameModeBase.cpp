// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipGameModeBase.h"
#include "BattleShipPlayerController.h"
#include "BattleShipGameStateBase.h"
#include "BattleShipHUD.h"

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
