// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipPlayerController.h"
#include "BattleShipTurn.h"
#include "BattleShipHUD.h"
#include "Blueprint/UserWidget.h"
#include "TextWidgetTypes.h"
#include "TextBlock.h"
#include "EngineUtils.h"


ABattleShipHUD::ABattleShipHUD() : playerDestroyedShips(0), opponentDestroyedShips(0)
{
	// Get the permanent HUD 
	static ConstructorHelpers::FClassFinder<UUserWidget> permanenthudWidgetObj(TEXT("WidgetBlueprint'/Game/Blueprints/UI/HUD/BP_PermanentHUD'"));
	if (permanenthudWidgetObj.Succeeded()) {
		pPermanentHUDWidgetClass = permanenthudWidgetObj.Class;
	}
	else {
		pPermanentHUDWidgetClass = nullptr;
	}
	// get the destroyed ship HUD
	static ConstructorHelpers::FClassFinder<UUserWidget> hudWidgetObj(TEXT("WidgetBlueprint'/Game/Blueprints/UI/HUD/BP_PermanentHUD'"));
	if (hudWidgetObj.Succeeded()) {
		pDestroyedShipWidgetClass = hudWidgetObj.Class;
	}
	else {
		pDestroyedShipWidgetClass = nullptr;
	}
}

void ABattleShipHUD::BeginPlay() {

	// Add permanent HUD to viewport
	if (pPermanentHUDWidgetClass) 
	{
		pPermanentHUDWidget = CreateWidget<UUserWidget>(this->GetOwningPlayerController(), this->pPermanentHUDWidgetClass);
		pPermanentHUDWidget->AddToViewport();
	}

	// Get textblock
	txtScorePlayer = (UTextBlock*)pPermanentHUDWidget->GetWidgetFromName("txtScorePlayer");
	txtScoreOpponent = (UTextBlock*)pPermanentHUDWidget->GetWidgetFromName("txtScoreOpponent");
	txtTurno = (UTextBlock*)pPermanentHUDWidget->GetWidgetFromName("txtTurno");

	// Get pointers to boards
	for (TActorIterator<ABattleShipBoard> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
		// Clickable means that the board belongs to the player
		if (ActorItr->bIsBoardClickable) {
			PlayerBoardPtr = *ActorItr;
		}
		else {
			OpponentBoardPtr = *ActorItr;
		}
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
	if (PlayerController->currentTurn == EBattleShipTurn::PLAYER) {
		txtTurno->SetText(FText::FromString("Player"));
	}
	else if(PlayerController->currentTurn == EBattleShipTurn::IA){
		txtTurno->SetText(FText::FromString("Opponent"));
	}
}

void ABattleShipHUD::DestroyedShipDelegateHandler(AShip* Ship) {

	// Add DestroyedShipWidget
	if (IsShipInBoard(Ship, PlayerBoardPtr)) {
		playerDestroyedShips += 1;
		txtScorePlayer->SetText(FText::FromString(FString::FromInt(playerDestroyedShips) + "/4"));
		UE_LOG(LogTemp, Log, TEXT("--------[HUD] >> InPlayer -----------\n\n\n"));
	}
	else
	if(IsShipInBoard(Ship, OpponentBoardPtr)) {
		playerDestroyedShips += 1;
		txtScorePlayer->SetText(FText::FromString(FString::FromInt(playerDestroyedShips) + "/4"));
		UE_LOG(LogTemp, Log, TEXT("--------[HUD] >> InOpponent -----------\n\n\n"));
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("--------[HUD] >> SHIP NOT FOUND -----------\n\n\n"));
	}
	
}

bool ABattleShipHUD::IsShipInBoard(AShip * Ship, TWeakObjectPtr<ABattleShipBoard> Board)
{
	TArray<TSubclassOf<AShip>> ShipsToCheck = Board->Ships;
	UE_LOG(LogTemp, Log, TEXT("--------[HUD] >> Ehhhhhhh HOLA %d-----------\n\n\n"), ShipsToCheck.Num());
	for (auto& ShipToCheck : ShipsToCheck) {
		AShip* ShipToCheckActor = ShipToCheck.GetDefaultObject();
		UE_LOG(LogTemp, Log, TEXT("--------[HUD] >> %d %d %d -- %d %d %d -----------\n\n\n"), Ship->GetTransform().GetLocation().X, Ship->GetTransform().GetLocation().Y, Ship->GetTransform().GetLocation().Z, ShipToCheckActor->GetTransform().GetLocation().X, ShipToCheckActor->GetTransform().GetLocation().Y, ShipToCheckActor->GetTransform().GetLocation().Z);
		if (ShipToCheckActor == Ship)
		//if(ShipToCheck.Get() == (UObject*)Ship)
			return true;
	}

	return false;
}

