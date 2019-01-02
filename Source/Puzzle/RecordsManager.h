// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecordsManager.generated.h"

class UBattleShipSaveGame;

UCLASS()
class PUZZLE_API ARecordsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARecordsManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = MapsAndSets)
		TMap<FString, float> GetRecords() const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
		void SaveNewRecord(FString PlayerName, float Duration);

private:
	UPROPERTY()
		TWeakObjectPtr<UBattleShipSaveGame> BattleShipSaveGamePtr;
		TMap<FString, float> Records;

		void LoadTopTenRecords(TMap<FString, float>& RecordsFrom, TMap<FString, float>& RecordsTo);
};
