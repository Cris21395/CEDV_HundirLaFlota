// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleShipBoard.h"
#include "Ship.h"
#include "Block.h"
#include "EngineMinimal.h"

// Sets default values
ABattleShipBoard::ABattleShipBoard() : Size(10), BlockSpacing(115.0f), SizeOfBlock(FVector(0.3f, 0.3f, 0.1f)), bIsBoardClickable(true)
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot.Get();
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
			FVector location = CalculateLocation(Index) + GetActorLocation();

			// Spawn block
			ABlock* NewBlock = GetWorld()->SpawnActor<ABlock>(location, FRotator(0.0f, 0.0f, 0.0f));

			// Check if block is not null
			if (NewBlock != nullptr)
			{
				// Modify its scale
				NewBlock->GetBlockMesh()->SetRelativeScale3D(SizeOfBlock);

				// Attach to this actor
				NewBlock->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

				// If true, we register events so that board can be clickable
				if (bIsBoardClickable)
				{
					NewBlock->ReceiveInputFromMouse();
				}

				// Set default values
				NewBlock->BlockIndex = Index;
				NewBlock->bIsActive = false;
				NewBlock->OwningBoard = this;
			}
		}
	}
}

ABlock * ABattleShipBoard::GetBlockByIndex(int32 Index) const
{
	ABlock* Block = Cast<ABlock>(RootComponent->GetChildComponent(Index)->GetOwner());

	return Block;
}

int32 ABattleShipBoard::CalculateIndex(int32 X, int32 Y)
{
	// return an index from 0 to (Size * Size)
	return X + (Size * Y);
}

FVector ABattleShipBoard::CalculateLocation(int32 BlockIndex)
{
	float X_Offset = (BlockIndex / Size) * BlockSpacing; // Divide by dimension
	float Y_Offset = (BlockIndex % Size) * BlockSpacing; // Modulo gives remainder

	return FVector(X_Offset, Y_Offset, 0.0f);
}