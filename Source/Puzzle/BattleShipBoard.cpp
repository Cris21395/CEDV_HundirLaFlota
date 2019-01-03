// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipBoard.h"
#include "Block.h"

// Sets default values
ABattleShipBoard::ABattleShipBoard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot.Get();

	// Default values
	Size = 10;
	BlockSpacing = 115.0f;
	SizeOfBlock = FVector(0.3f, 0.3f, 0.15f);
}

// Called when the game starts or when spawned
void ABattleShipBoard::BeginPlay()
{
	Super::BeginPlay();
	
	// Loop to spawn each of the blocks that composes the board
	for (int32 Y_Index = 0; Y_Index < Size; Y_Index++)
	{
		for (int32 X_Index = 0; X_Index < Size; X_Index++)
		{
			// Obtain the index of the block
			int32 Index = CalculateIndex(X_Index, Y_Index);

			// Make position vector
			const FVector location = CalculateLocation(Index) + GetActorLocation();

			// Spawn block
			ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(location, FRotator(0.0f, 0.0f, 0.0f));

			// Modify its scale
			NewBlock->GetBlockMesh()->SetRelativeScale3D(SizeOfBlock);

			// Attach to this actor
			NewBlock->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

			// Tell the block about its owner
			if (NewBlock != nullptr)
			{
				NewBlock->OwningBoard = this;
			}
		}
	}
}

// Called every frame
void ABattleShipBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ABattleShipBoard::CalculateIndex(int32 X, int32 Y)
{
	return X + (Size * Y);
}

FVector ABattleShipBoard::CalculateLocation(int32 Index)
{
	float X_Offset = (Index / Size) * BlockSpacing; // Divide by dimension
	float Y_Offset = (Index % Size) * BlockSpacing; // Modulo gives remainder

	return FVector(X_Offset, Y_Offset, 0.0f);
}

