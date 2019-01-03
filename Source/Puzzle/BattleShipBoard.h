// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "GameFramework/Actor.h"
#include "BattleShipBoard.generated.h"

/**
* BattleShipBoard class is an actor to create a board and mantain 
* the logic associated with this element
*/
UCLASS()
class PUZZLE_API ABattleShipBoard : public AActor
{
	GENERATED_BODY()

	// Dummy root component. It is used so that class can be Movable
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<USceneComponent> DummyRoot;
	
public:	
	// Sets default values for this actor's properties
	ABattleShipBoard();

	// Number of elements along each side of the grid
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Board")
		int32 Size;

	// Spacing of blocks
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Board")
		float BlockSpacing;

	// Size of each block of the board
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Board")
		FVector SizeOfBlock;
	
	// Array of indexes that have been selected
	UPROPERTY()
		TArray<int32> SelectedIndexes;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Calculate index from x,y coordinates
	int32 CalculateIndex(int32 X, int32 Y);

	// Calculate location (position X, position Y) from x,y coordinates
	FVector CalculateLocation(int32 Index);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Returns DummyRoot subobject
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot.Get(); }

	void SpawnRandomShips();
};
