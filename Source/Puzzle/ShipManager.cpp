// Fill out your copyright notice in the Description page of Project Settings.

#include "ShipManager.h"
#include "EngineUtils.h"
#include "BattleShipBoard.h"
#include "Block.h"

// Sets default values
AShipManager::AShipManager()
{	
	// Store references to ships classes
	ShipClasses.AddUnique(ACruisserShip::StaticClass());
	ShipClasses.AddUnique(ASubmarineShip::StaticClass());
	ShipClasses.AddUnique(AVesselShip::StaticClass());
	ShipClasses.AddUnique(ABoatShip::StaticClass());
}

// Called when the game starts or when spawned
void AShipManager::BeginPlay()
{
	Super::BeginPlay();

	// Array of boards
	TArray<ABattleShipBoard*> BattleShipBoardArray;

	// Get a reference of board to spawn ships
	for (TActorIterator<ABattleShipBoard> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Conversion to smart pointer
		BattleShipBoardArray.Add(*ActorItr);
	}

	// Spawn ships in their specific boards
	for (auto& BattleShipBoardPtr : BattleShipBoardArray)
	{
		for (auto& TypeShip : ShipClasses)
		{
			// If board is clickable, it is for machine
			// If board is not clickable, it is for player
			if (BattleShipBoardPtr->bIsBoardClickable)
			{
				SpawnRandomShip(BattleShipBoardPtr, TypeShip, false);
			}
			else
			{
				SpawnRandomShip(BattleShipBoardPtr, TypeShip, true);
			}
		}
	}

	// Move ship array to board. ShipClasses will be left empty
	// This is made so that ships are not replicated
	for (auto& BattleShipBoardPtr : BattleShipBoardArray)
	{
		BattleShipBoardPtr->Ships = MoveTemp(ShipClasses);
	}
}

void AShipManager::SpawnRandomShip(ABattleShipBoard* BattleShipBoardPtr, TSubclassOf<AShip> ShipType, bool bMustShipsBeVisible)
{
	int32 Size = BattleShipBoardPtr->Size;
	int32 BlockSpacing = BattleShipBoardPtr->BlockSpacing;
	FVector BlockSize = BattleShipBoardPtr->SizeOfBlock;

	// Get random index
	int32 RandomIndex = FMath::Rand() % (Size * Size);

	// Get different index when it is not valid
	while (!IsValidIndex(RandomIndex, BattleShipBoardPtr, ShipType))
	{
		RandomIndex = FMath::Rand() % (Size * Size);
	}

	// Logic to spawn each ship
	ABlock* Block = BattleShipBoardPtr->GetBlockByIndex(RandomIndex);

	// Location of Block
	FVector Blocklocation = Block->GetActorLocation();

	// New ship that will be converted into each boat
	AShip* NewShip = nullptr;

	// IsChildOf compares if Class == OtherClass or Class == Child of Other Class
	if (ShipType->IsChildOf(ABoatShip::StaticClass())) {
		// Spawn boat
		Blocklocation += FVector(0.0f, 0.0f, 10.0f); // Offset due to the scale
		NewShip = GetWorld()->SpawnActor<ABoatShip>(Blocklocation, FRotator(0.0f, 45.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f) * (BlockSize.X*100 + 10));
	}
	else if (ShipType->IsChildOf(AVesselShip::StaticClass())) {
		// Spawn Vessel
		Blocklocation += FVector(30.0f, 0.0f, 40.0f); // Offset due to the scale
		NewShip = GetWorld()->SpawnActor<AVesselShip>(Blocklocation, FRotator(0.0f, 0.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f) * (BlockSize.X * 100 + BlockSpacing*(BlockSize.X+0.1f)));
	}
	else if (ShipType->IsChildOf(ASubmarineShip::StaticClass())) {
		// Spawn Submarine
		Blocklocation += FVector(0.0f, 1.0f, 0.0f) * ((BlockSize.X + BlockSpacing) * 2); // Offset due to the scale
		NewShip = GetWorld()->SpawnActor<ASubmarineShip>(Blocklocation, FRotator(0.0f, 0.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f) * (BlockSize.X * 100));
	}
	else if(ShipType->IsChildOf(ACruisserShip::StaticClass())){
		// Spawn Cruiser
		Blocklocation += FVector(0.0f, 1.0f, 0.0f) * (3489 * BlockSize.X - 7.78 * BlockSpacing); // Offset due to the scale
		NewShip = GetWorld()->SpawnActor<ACruisserShip>(Blocklocation, FRotator(0.0f, 0.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f) * (BlockSpacing / 10));
	}

	if (!bMustShipsBeVisible)
	{
		NewShip->SetActorHiddenInGame(!bMustShipsBeVisible);
		NewShip->SetActorEnableCollision(bMustShipsBeVisible);
	}

	NewShip->SetOccupiedBlocks(RandomIndex, BattleShipBoardPtr);
}

bool AShipManager::IsValidIndex(int32 IndexToCheck, ABattleShipBoard* BattleShipBoardPtr, TSubclassOf<AShip> ShipType)
{
	bool bIsIndexValid = false;

	if (CheckBoardBoundaries(IndexToCheck, BattleShipBoardPtr, ShipType) &&
		CheckCollisions(IndexToCheck, BattleShipBoardPtr, ShipType) &&
		CheckShipBoundaries(IndexToCheck, BattleShipBoardPtr, ShipType))
		return !bIsIndexValid;
	
	return bIsIndexValid;
}

int32 AShipManager::GetShipSize(TSubclassOf<AShip> ShipType)
{
	int32 Size = 0;

	if (ShipType->IsChildOf(ABoatShip::StaticClass())) Size = 1;
	else if (ShipType->IsChildOf(AVesselShip::StaticClass())) Size = 2;
	else if (ShipType->IsChildOf(ASubmarineShip::StaticClass()))Size = 3;
	else if (ShipType->IsChildOf(ACruisserShip::StaticClass())) Size = 4;

	return Size;
}

bool AShipManager::CheckBoardBoundaries(int32 IndexToCheck, ABattleShipBoard* BattleShipBoardPtr, TSubclassOf<AShip> ShipType)
{
	int32 Size = GetShipSize(ShipType);
	bool bIsBoatInBoard = false;

	if (ShipType->IsChildOf(ABoatShip::StaticClass())) // Always true for Boat
		return !bIsBoatInBoard;
	else if (ShipType->IsChildOf(AVesselShip::StaticClass()) &&
		IndexToCheck + BattleShipBoardPtr->Size < BattleShipBoardPtr->Size*BattleShipBoardPtr->Size) //Vertical Check for Vessel
		return !bIsBoatInBoard;
	else if ((ShipType->IsChildOf(ACruisserShip::StaticClass()) || ShipType->IsChildOf(ASubmarineShip::StaticClass())) &&
		((IndexToCheck % BattleShipBoardPtr->Size) + Size) <= BattleShipBoardPtr->Size) // Horizontal Check for Submarine and Cruisser
		return !bIsBoatInBoard;

	return bIsBoatInBoard;
}

bool AShipManager::CheckCollisions(int32 IndexToCheck, ABattleShipBoard* BattleShipBoardPtr, TSubclassOf<AShip> ShipType)
{
	int32 Size = GetShipSize(ShipType);
	bool bIsValid = true;

	if (ShipType->IsChildOf(AVesselShip::StaticClass())) { // Is vertical
		for (int i = 0; i < Size; i++) {
			if (BattleShipBoardPtr->GetBlockByIndex(IndexToCheck + BattleShipBoardPtr->Size*i)->bHasShip)
				return !bIsValid;	// If the block is already occupied is not valid
		}
	}
	else	// Is Horizontal
	{
		for (int i = 0; i < Size; i++) {
			if (BattleShipBoardPtr->GetBlockByIndex(IndexToCheck + i)->bHasShip)
				return !bIsValid;	// If the block is already occupied is not valid
		}
	}

	return bIsValid; // Is valid
}

bool AShipManager::CheckShipBoundaries(int32 IndexToCheck, ABattleShipBoard* BattleShipBoardPtr, TSubclassOf<AShip> ShipType)
{
	int32 ShipSize = GetShipSize(ShipType);
	int32 BoardSize = BattleShipBoardPtr->Size;

	// Used in the diagonal check
	bool left = false;
	bool right = false;
	bool top = false;
	bool bottom = false;

	// Vertical Ships Checks
	if (ShipType->IsChildOf(AVesselShip::StaticClass())) {
		// There are left boxes
		if (IndexToCheck % BoardSize != 0) {
			left = true;
			// If there is a Ship in any of the left
			for (int i = 0; i < ShipSize; i++) {
				if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck + BoardSize * i - 1))
					return false;
			}
		}

		// There are bottom boxes
		if (BoardSize < IndexToCheck) {
			bottom = true;
			// If there is a ship in the bottom
				if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck - BoardSize))
					return false;
		}

		// There are right boxes
		if ((IndexToCheck +1) % BoardSize != 0 || IndexToCheck == 0) {
			right = true;
			// If there is a ship in the right
			for (int i = 0; i < ShipSize; i++) {
				if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck + BoardSize * i + 1))
					return false;
			}
		}

		// There are top boxes
		if ((IndexToCheck + BoardSize*ShipSize) < BoardSize*BoardSize) {
			top = true;
			// If there is a ship in any of the top 
				if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck + BoardSize * ShipSize))
					return false;
		}

		// Corners Checks
		if (top && left)
			if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck + BoardSize * ShipSize - 1))
				return false;
		if (top && right)
			if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck + BoardSize * ShipSize + 1))
				return false;
		if (bottom && left)
			if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck - BoardSize - 1))
				return false;
		if (bottom && right)
			if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck - BoardSize + 1))
				return false;
	}
	//////////////// Horizontal Ships Checks /////////////////
	else{
		// There are left boxes
		if (IndexToCheck % BoardSize != 0) {
			left = true;
			// If there is a Ship on the left
			if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck - 1))
				return false;
		}

		// There are bottom boxes
		if (BoardSize < IndexToCheck) {
			bottom = true;
			// If there is a ship in any box of the bottom
			for (int i = 0; i < ShipSize; i++) {
				if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck - BoardSize + i))
					return false;
			}
		}

		// There are right boxes
		if ((IndexToCheck + ShipSize) % BoardSize != 0 || IndexToCheck == 0) {
			right = true;
			// If there is a ship in the right
			if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck + ShipSize))
				return false;
		}

		// There are top boxes
		if (BoardSize*(BoardSize - 1) > IndexToCheck) {
			top = true;
			// If there is a ship in any of the top 
			for (int i = 0; i < ShipSize; i++) {
				if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck + BoardSize + i))
					return false;
			}
		}

		// Corners checks
		if (top && left)
			if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck + BoardSize - 1))
				return false;
		if (top && right)
			if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck + BoardSize + ShipSize))
				return false;
		if (bottom && left)
			if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck - BoardSize - 1))
				return false;
		if (bottom && right)
			if (!IsEmptyBlock(BattleShipBoardPtr, IndexToCheck - BoardSize + ShipSize))
				return false;
	}

	return true;
}

bool AShipManager::IsEmptyBlock(ABattleShipBoard* BattleShipBoardPtr, int32 IndexToCheck)
{
	int32 Size = BattleShipBoardPtr->Size;
	bool bIsEmpty = true;

	if (IndexToCheck >= 0 && IndexToCheck < (Size * Size))
	{
		ABlock* Block = BattleShipBoardPtr->GetBlockByIndex(IndexToCheck);
		if (Block->bHasShip) return !bIsEmpty;
	}

	return bIsEmpty;
}
