// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipGameInstance.h"


UBattleShipGameInstance::UBattleShipGameInstance() {
	// Initialize variables to default values
	ResetGameAfterAssignRecord();
}

void UBattleShipGameInstance::ResetGameAfterAssignRecord()
{
	// Initialize to 0 as a game can never finish in score 0
	FinalScore = 0;
	// Initialize to NONE as the winner will always be the PLAYER ot the IA
	GameWinner = EBattleShipTurn::NONE;
	// Initialice the time
	GameTime = 0.0f;
}

void UBattleShipGameInstance::SetWinnerAndScore(EBattleShipTurn Winner, int Score, float Time)
{
	GameWinner = Winner;
	FinalScore = Score;
	GameTime = Time;

}
