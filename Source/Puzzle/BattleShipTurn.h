// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

enum class EBattleShipTurn : uint8
{
	PLAYER,	// Player turn
	IA,		// Machine turn
	NONE	// Aux turn when the game has finished
};