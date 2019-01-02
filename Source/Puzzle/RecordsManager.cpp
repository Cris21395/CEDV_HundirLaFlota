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
	
	// Create an instance of a SaveGame object
	BattleShipSaveGamePtr = Cast<UBattleShipSaveGame>(UGameplayStatics::CreateSaveGameObject(UBattleShipSaveGame::StaticClass()));

	// if save game exist
	if (UGameplayStatics::DoesSaveGameExist(BattleShipSaveGamePtr->SaveSlotName, BattleShipSaveGamePtr->UserIndex))
	{
		// Create an instance of a LoadGame object
		BattleShipSaveGamePtr = Cast<UBattleShipSaveGame>(UGameplayStatics::LoadGameFromSlot(BattleShipSaveGamePtr->SaveSlotName,
			BattleShipSaveGamePtr->UserIndex));
	}

	// Load Records
	LoadTopTenRecords(BattleShipSaveGamePtr->Records, Records);
}

TMap<FString, float> ARecordsManager::GetRecords() const
{
	return Records;
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

void ARecordsManager::LoadTopTenRecords(TMap<FString, float>& RecordsFrom, TMap<FString, float>& RecordsTo)
{
	TArray<FString> Keys;
	TArray<float> Values;

	if (RecordsFrom.Num() > 0)
	{
		RecordsFrom.GenerateKeyArray(Keys);
		RecordsFrom.GenerateValueArray(Values);

		int Size = RecordsFrom.Num() < 10 ? RecordsFrom.Num() : 10;

		for (int i = 0; i < Size; i++) 
		{ 
			RecordsTo.Add(Keys[i], Values[i]); 
		}
	}
}

