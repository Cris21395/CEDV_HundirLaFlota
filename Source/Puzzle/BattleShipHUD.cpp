// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipHUD.h"
#include "BattleShipPlayerController.h"
#include "BattleShipTurn.h"
#include "Ship.h"
#include "SlateColor.h"
#include "WidgetAnimation.h"
#include "Blueprint/UserWidget.h"
#include "TextWidgetTypes.h"
#include "TextBlock.h"
#include "EngineUtils.h"

ABattleShipHUD::ABattleShipHUD() : playerDestroyedShips(0), opponentDestroyedShips(0)
{
	struct FConstructorStatics
	{
		ConstructorHelpers::FClassFinder<UUserWidget> PermanentHUDWidgetObject;
		ConstructorHelpers::FClassFinder<UUserWidget> DestroyedShipHUDWidgetObject;
		FConstructorStatics()
			: PermanentHUDWidgetObject(TEXT("/Game/Blueprints/UI/HUD/BP_PermanentHUD"))
			, DestroyedShipHUDWidgetObject(TEXT("/Game/Blueprints/UI/HUD/BP_DestroyedShipWidget"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Save pointer to widgets	
	pPermanentHUDWidgetClass = ConstructorStatics.PermanentHUDWidgetObject.Class;
	pDestroyedShipWidgetClass = ConstructorStatics.DestroyedShipHUDWidgetObject.Class;
}

void ABattleShipHUD::BeginPlay() 
{
	// Add permanent HUD to viewport
	if (pPermanentHUDWidgetClass) 
	{
		pPermanentHUDWidget = CreateWidget<UUserWidget>(this->GetOwningPlayerController(), this->pPermanentHUDWidgetClass);
		pPermanentHUDWidget->AddToViewport();
	}

	// Get textblock
	txtScorePlayer = (UTextBlock*)pPermanentHUDWidget->GetWidgetFromName("txtScorePlayer");
	txtScoreOpponent = (UTextBlock*)pPermanentHUDWidget->GetWidgetFromName("txtScoreOpponent");
	txtTurn = (UTextBlock*)pPermanentHUDWidget->GetWidgetFromName("txtTurn");

	// Get pointers to boards
	for (TActorIterator<ABattleShipBoard> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		// Clickable means that the board belongs to the player
		if (ActorItr->bIsBoardClickable) PlayerBoardPtr = *ActorItr;
		else OpponentBoardPtr = *ActorItr;
	}

	// Bind delegates to handler funtion
	for (TActorIterator<AShip> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->DestroyedShipDelegate.BindUObject(this, &ABattleShipHUD::DestroyedShipDelegateHandler);
	}
}

void ABattleShipHUD::ChangeTurn()
{
	ABattleShipPlayerController* PlayerController = Cast<ABattleShipPlayerController>(GetWorld()->GetFirstPlayerController());

	if (PlayerController->currentTurn == EBattleShipTurn::PLAYER) 
	{
		txtTurn->SetColorAndOpacity(FSlateColor::FSlateColor(FLinearColor::FLinearColor(FVector::FVector(0.0f, 0.7f, 0.0f))));
		txtTurn->SetText(FText::FromString(PLAYER_GAME));
	}
	else if(PlayerController->currentTurn == EBattleShipTurn::IA)
	{
		txtTurn->SetColorAndOpacity(FSlateColor::FSlateColor(FLinearColor::FLinearColor(FVector::FVector(0.7f, 0.0f, 0.0f))));
		txtTurn->SetText(FText::FromString(OPPONENT_GAME));
	}
}

int ABattleShipHUD::GetPlayerScore()
{
	return playerDestroyedShips;
}

int ABattleShipHUD::GetOpponentScore()
{
	return opponentDestroyedShips;
}

void ABattleShipHUD::DestroyedShipDelegateHandler(AShip* Ship) 
{
	// It is the same to use playerBoard as opponent
	int NumberOfShips = PlayerBoardPtr->Ships.Num();

	// Add DestroyedShipWidget
	if (IsShipInBoard(Ship, PlayerBoardPtr.Get())) 
	{
		playerDestroyedShips += 1;
		txtScorePlayer->SetText(FText::FromString(
			FString::FromInt(playerDestroyedShips) + 
			"/" +
			FString::FromInt(NumberOfShips)));
	}
	else if(IsShipInBoard(Ship, OpponentBoardPtr.Get())) 
	{
		opponentDestroyedShips += 1;
		txtScoreOpponent->SetText(FText::FromString(
			FString::FromInt(opponentDestroyedShips) +
			"/" +
			FString::FromInt(NumberOfShips)));
	}

	// Show visual animated notification of destroyed ship
	if (pDestroyedShipWidgetClass) {
		pDestroyedShipWidget = CreateWidget<UUserWidget>(this->GetOwningPlayerController(), this->pDestroyedShipWidgetClass);
		pDestroyedShipWidget->AddToViewport();
	}
}

bool ABattleShipHUD::IsShipInBoard(AShip * Ship, ABattleShipBoard* Board)
{
	bool bShipInBoard = false;
	TArray<AShip*> Ships = Board->Ships;

	for (auto& ShipActor : Ships) 
	{
		if (ShipActor == Ship) bShipInBoard = true;
	}

	return bShipInBoard;
}

