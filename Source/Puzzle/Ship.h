// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ShipType.h"
#include "BattleShipBoard.h"
#include "Block.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ship.generated.h"

UCLASS()
class PUZZLE_API AShip : public AActor 
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShip();
	void SetType(EShipType NewType);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	EShipType Type;
	TWeakObjectPtr<UStaticMeshComponent> ShipMesh;
	int32 IndexOrigin;
	int32 Size;
	TArray<int32> OccupiedPositions;
	FORCEINLINE class UStaticMeshComponent* GetShipMesh() const { return ShipMesh.Get(); }
	
	// Override in children to set occupied blocks for each ship type
	virtual void SetOccupiedBlocks(int32 SpawnIndex, TWeakObjectPtr<ABattleShipBoard> BattleShipBoardPtr) PURE_VIRTUAL(AShip::SetOccupiedBlocks, );
};
