// Fill out your copyright notice in the Description page of Project Settings.

#include "RecordsManager.h"
#include "BattleShipSaveGame.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARecordsManager::ARecordsManager() : BattleShipSaveGamePtr(nullptr) {}

// Called when the game starts or when spawned
void ARecordsManager::BeginPlay()
{
	Super::BeginPlay();

	// if save game exist
	if (UGameplayStatics::DoesSaveGameExist(SAVE_SLOT_NAME, USER_INDEX))
	{
		// Create an instance of a LoadGame object
		BattleShipSaveGamePtr = Cast<UBattleShipSaveGame>(UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME, USER_INDEX));
	}
	else
	{
		// Create an instance of a SaveGame object
		BattleShipSaveGamePtr = Cast<UBattleShipSaveGame>(UGameplayStatics::CreateSaveGameObject(UBattleShipSaveGame::StaticClass()));
	}
}

TMap<FString, float> ARecordsManager::GetRecords() const
{
	return BattleShipSaveGamePtr->Records;
}


void ARecordsManager::SaveNewRecord(FString PlayerName, float Duration)
{
	// Add a new record
	BattleShipSaveGamePtr->Records.Add(PlayerName, Duration);

	// Order the TMap regarding the max duration
	BattleShipSaveGamePtr->Records.ValueSort([](const float& A, const float& B)
	{
		return A < B;
	});

	// Save the game
	UGameplayStatics::SaveGameToSlot(BattleShipSaveGamePtr.Get(), BattleShipSaveGamePtr->SaveSlotName, BattleShipSaveGamePtr->UserIndex);
}

