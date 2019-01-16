// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "BattleShipTurn.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BattleShipGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_API UBattleShipGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UBattleShipGameInstance();

	/* Score obtained by the winner. Positive means that the player has won
	   negative means that the machine has won*/
	UPROPERTY(BlueprintReadOnly)
	int FinalScore;

	// The winner of the game
	UPROPERTY(BlueprintReadOnly)
	EBattleShipTurn GameWinner;

	// Called after writing in records board, so that the next game begins empty
	UFUNCTION(BlueprintCallable, Category = "CppFunctions")
	void ResetGameAfterAssignRecord();

	// How many time the game last
	UPROPERTY(BlueprintReadOnly)
		float GameTime;

	// Called when the Game Finishes 
	void SetWinnerAndScore(EBattleShipTurn Winner, int Score, float Time);
	
	
};
