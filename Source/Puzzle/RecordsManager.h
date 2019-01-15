// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecordsManager.generated.h"

class UBattleShipSaveGame;

/**
* Custom class that defines the logic of records
*/
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
		TMap<FString, int> GetRecords() const;

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
		void SaveNewRecord(FString PlayerName, int HitShips);

private:
	UPROPERTY()
		TWeakObjectPtr<UBattleShipSaveGame> BattleShipSaveGamePtr;
};
