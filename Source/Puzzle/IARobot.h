// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleShipBoard.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IARobot.generated.h"

UCLASS()
class PUZZLE_API AIARobot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AIARobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Player Board Pointer
	UPROPERTY()
		TWeakObjectPtr<ABattleShipBoard> BattleShipPlayerBoardPtr;

	// Called when robot has the turn
	void PlayTurn();
};
