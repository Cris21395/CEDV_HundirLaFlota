// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipGameInstance.h"
#include "BattleShipPlayerController.h"
#include "BattleShipHUD.h"
#include "EngineUtils.h"
#include "EngineMinimal.h"

ABattleShipPlayerController::ABattleShipPlayerController() : DelayToChangeTurn(2), AccumulatedDeltaTime(0), DelayToFinishGame(2)
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
	switch (currentTurn)
	{
	case EBattleShipTurn::PLAYER:

		// Enable events for player
		SetReceiveInput(true);

		break;
	case EBattleShipTurn::IA:

		AccumulatedDeltaTime += DeltaSeconds;

		// Disable events for player
		SetReceiveInput(false);

		if (AccumulatedDeltaTime >= DelayToChangeTurn)
		{
			Robot->PlayTurn();

			AccumulatedDeltaTime = 0.0f;
		}

		break;
	case EBattleShipTurn::NONE:

		AccumulatedDeltaTime += DeltaSeconds;

		// Open new level
		if (AccumulatedDeltaTime >= DelayToFinishGame)
		{
			UGameplayStatics::OpenLevel(GetWorld(), TEXT("/Game/Maps/Records"), TRAVEL_Absolute);

			// Set GameInstance reference
			UBattleShipGameInstance* GameInstance = Cast<UBattleShipGameInstance>(GetWorld()->GetGameInstance());

			// Calculate Score
			int score = 0;
			ABattleShipHUD* HUD = Cast<ABattleShipHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			int NumberOfPlayerShips = HUD->GetPlayerScore();
			int NumberOfMachineShips = HUD->GetOpponentScore();
			score = NumberOfPlayerShips - NumberOfMachineShips;

			// Set winner, score and time
			GameInstance->SetWinnerAndScore(winner, score, AccumulatedDeltaTime);

			AccumulatedDeltaTime = 0.0f;
		}

		break;
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
