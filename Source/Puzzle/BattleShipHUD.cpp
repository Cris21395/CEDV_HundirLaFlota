// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipHUD.h"
#include "Blueprint/UserWidget.h"
#include "TextWidgetTypes.h"
#include "TextBlock.h"
#include "EngineUtils.h"


ABattleShipHUD::ABattleShipHUD() : playerDestroyedShips(0), opponentDestroyedShips(0)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> hudWidgetObj(TEXT("WidgetBlueprint'/Game/Blueprints/UI/HUD/BP_PermanentHUD'"));
	if (hudWidgetObj.Succeeded()) {
		pPermanentHUDWidgetClass = hudWidgetObj.Class;
	} else {
		pPermanentHUDWidgetClass = nullptr;
	}
}

void ABattleShipHUD::BeginPlay() {

	// Add permanent HUD to viewport
	if (pPermanentHUDWidgetClass) {
		pPermanentHUDWidget = CreateWidget<UUserWidget>(this->GetOwningPlayerController(), this->pPermanentHUDWidgetClass);
		pPermanentHUDWidget->AddToViewport();
	}
	// Get textblock
	txtScorePlayer = (UTextBlock*)pPermanentHUDWidget->GetWidgetFromName("txtScorePlayer");
	txtScoreOpponent = (UTextBlock*)pPermanentHUDWidget->GetWidgetFromName("txtScoreOpponent");
	//(LogTemp, Log, TEXT("--------[HUD] >> -----------\n\n\n"));

	// Bind delegates to handler funtion
	for (TActorIterator<AShip> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->DestroyedShipDelegate.BindUObject(this, &ABattleShipHUD::DestroyedShipDelegateHandler);
		UE_LOG(LogTemp, Log, TEXT("--------[HUD] >> BIND SHIP sized %d -----------\n\n\n"), ActorItr->Size);
	}
}

void ABattleShipHUD::DestroyedShipDelegateHandler(AShip* Ship) {
	
	playerDestroyedShips += 1;
	txtScorePlayer->SetText(FText::FromString(FString::FromInt(playerDestroyedShips)));
	UE_LOG(LogTemp, Log, TEXT("--------[HUD] >> %s-----------\n\n\n"), playerDestroyedShips + "/4");
}

