// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattleShipPlayerController.generated.h"

/**
 * Custom class that collects the commands given by the player
 */
UCLASS()
class PUZZLE_API ABattleShipPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABattleShipPlayerController();
};
