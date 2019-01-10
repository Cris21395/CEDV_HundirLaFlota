// Fill out your copyright notice in the Description page of Project Settings.

#include "IARobot.h"
#include "EngineUtils.h"
#include "Block.h"


// Sets default values
AIARobot::AIARobot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIARobot::BeginPlay()
{
	Super::BeginPlay();

	FString BattleBoardPlayerString = FString(TEXT("BattleShipBoardPlayer"));

	// Get a reference of board to spawn ships
	for (TActorIterator<ABattleShipBoard> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (BattleBoardPlayerString.Equals(ActorItr->GetName()))
		{
			// Conversion to smart pointer
			BattleShipPlayerBoardPtr = *ActorItr;
			break;
		}
	}
	
}

void AIARobot::PlayTurn()
{
	int32 Size = BattleShipPlayerBoardPtr->Size;
	int32 RandomIndex = FMath::Rand() % (Size * Size);

	ABlock* Block = BattleShipPlayerBoardPtr->GetBlockByIndex(RandomIndex);

	// Check if the block has been clicked yet
	while (Block->bIsActive) {
		RandomIndex = FMath::Rand() % (Size * Size);
		Block = BattleShipPlayerBoardPtr->GetBlockByIndex(RandomIndex);
	}

	Block->HandleClicked();

}



