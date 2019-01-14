// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Ship.h"

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BattleShipHUD.generated.h"

/**
 * ABattleShipHUD class used to manage the HUD of the game
 */
UCLASS()
class PUZZLE_API ABattleShipHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABattleShipHUD();

	// Called when the game starts or when spawned
	void BeginPlay() override;
	
private:
	// Destroyed ships of the player
	int playerDestroyedShips;

	// Destroyed ships of the opponent
	int opponentDestroyedShips;

	// Pointer to permanent HUD widget
	UPROPERTY()
		TWeakObjectPtr<class UUserWidget> pPermanentHUDWidget;

	// Pointer to destroy widget
	UPROPERTY()
		TWeakObjectPtr<class UUserWidget> pDestroyedShipWidget;

	// Pointer to txtScoreOpponent
	UPROPERTY()
		TWeakObjectPtr<class UTextBlock> txtScoreOpponent;

	// Pointer to txtScorePlayer
	UPROPERTY()
		TWeakObjectPtr<class UTextBlock> txtScorePlayer;

	// Pointer to permanent HUD widget class
	class UClass* pPermanentHUDWidgetClass;

	// Pointer to destroy HUD widget class
	class UClass* pDestroyedShipWidgetClass;
	
	// Called when a ship has been destroyed
	void DestroyedShipDelegateHandler(AShip* Ship);
};
