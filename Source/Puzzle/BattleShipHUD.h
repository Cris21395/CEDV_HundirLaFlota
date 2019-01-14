// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Ship.h"

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BattleShipHUD.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_API ABattleShipHUD : public AHUD
{
	GENERATED_BODY()

public:
	ABattleShipHUD();
	void BeginPlay() override;
	
private:
	int playerDestroyedShips;
	int opponentDestroyedShips;

	TWeakObjectPtr<class UUserWidget> pPermanentHUDWidget;
	TWeakObjectPtr<class UTextBlock> txtScorePlayer;
	TWeakObjectPtr<class UTextBlock> txtScoreOpponent;
	class UClass* pPermanentHUDWidgetClass;

	TWeakObjectPtr<class UUserWidget> pDestroyedShipWidget;
	class UClass* pDestroyedShipWidgetClass;
	
	void DestroyedShipDelegateHandler(AShip* Ship);
};
