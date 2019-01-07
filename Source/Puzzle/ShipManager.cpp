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
	
	FString BattleBoardPlayerString = FString(TEXT("BattleShipBoardPlayer"));

	// Get a reference of board to spawn ships
	for (TActorIterator<ABattleShipBoard> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (BattleBoardPlayerString.Equals(ActorItr->GetName()))
		{
			// Conversion to smart pointer
			BattleShipBoardPtr = *ActorItr;
			break;
		}
	}

	// Spawn ships
	for (auto& TypeShip : ShipClasses) 
	{
		SpawnRandomShip(TypeShip);
	}

	// Move ship array to board. ShipClasses will be left empty
	// This is made so that ships are not replicated
	BattleShipBoardPtr->Ships = MoveTemp(ShipClasses);
}

void AShipManager::SpawnRandomShip(TSubclassOf<AShip> ShipType)
{
	int32 Size = BattleShipBoardPtr->Size;

	// Get random index
	int32 RandomIndex = FMath::Rand() % (Size * Size);
	//int32 RandomIndex = 6;
	while (!IsValidIndex(RandomIndex, ShipType)) {	// Get different index when it is not valid
		UE_LOG(LogTemp, Log, TEXT("nnnnnnnnnnnnnnnnnnnnnnnnREPEAT! %d\n\n\n"), RandomIndex);
		RandomIndex = FMath::Rand() % (Size * Size);	
	}

	// Logic to spawn each ship
	ABlock* Block = BattleShipBoardPtr->GetBlockByIndex(RandomIndex);

	// Location of Block
	FVector Blocklocation = Block->GetActorLocation();

	// IsChildOf compares if Class == OtherClass or Class == Child of Other Class
	if (ShipType->IsChildOf(ABoatShip::StaticClass())) {
		// Spawn boat
		Blocklocation += FVector(0.0f, 0.0f, 10.0f); // Offset due to the scale
		ABoatShip* NewShip = GetWorld()->SpawnActor<ABoatShip>(Blocklocation, FRotator(0.0f, 45.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(30.0f, 30.0f, 30.0f));
		UE_LOG(LogTemp, Log, TEXT("--------[Boat] >> %d-----------\n\n\n"), RandomIndex);
		NewShip->SetOccupiedBlocks(RandomIndex, BattleShipBoardPtr.Get());
	}
	else if (ShipType->IsChildOf(AVesselShip::StaticClass())) {
		// Spawn Vessel
		Blocklocation += FVector(30.0f , 0.0f, 40.0f); // Offset due to the scale
		AVesselShip* NewShip = GetWorld()->SpawnActor<AVesselShip>(Blocklocation, FRotator(0.0f, 0.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(55.0f, 55.0f, 55.0f));
		UE_LOG(LogTemp, Log, TEXT("--------[Vessel] >> %d-----------\n\n\n"), RandomIndex);
		NewShip->SetOccupiedBlocks(RandomIndex, BattleShipBoardPtr.Get());
	}
	else if (ShipType->IsChildOf(ASubmarineShip::StaticClass())) {
		// Spawn Submarine
		Blocklocation += FVector(0.0f, 140.0f, 0.0f); // Offset due to the scale
		ASubmarineShip* NewShip = GetWorld()->SpawnActor<ASubmarineShip>(Blocklocation, FRotator(0.0f, 0.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(20.0f, 20.0f, 20.0f));
		UE_LOG(LogTemp, Log, TEXT("--------[Submarine] >> %d-----------\n\n\n"), RandomIndex);
		NewShip->SetOccupiedBlocks(RandomIndex, BattleShipBoardPtr.Get());
	}
	else if(ShipType->IsChildOf(ACruisserShip::StaticClass())){
		// Spawn Cruiser
		Blocklocation += FVector(0.0f, 130.0f, 0.0f); // Offset due to the scale
		ACruisserShip* NewShip = GetWorld()->SpawnActor<ACruisserShip>(Blocklocation, FRotator(0.0f, 0.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(7.0f, 7.0f, 7.0f));
		UE_LOG(LogTemp, Log, TEXT("--------[Cruisser] >> %d-----------\n\n\n"), RandomIndex);
		NewShip->SetOccupiedBlocks(RandomIndex, BattleShipBoardPtr.Get());
	}
	
}

bool AShipManager::IsValidIndex(int32 IndexToCheck, TSubclassOf<AShip> ShipType)
{
	if (CheckBoardBoundaries(IndexToCheck, ShipType) &&
		CheckCollisions(IndexToCheck, ShipType) &&
		CheckShipBoundaries(IndexToCheck, ShipType))
		return true;
	
	return false;
}

int32 AShipManager::GetShipSize(TSubclassOf<AShip> ShipType)
{
	int32 Size = 0;
	if (ShipType->IsChildOf(ABoatShip::StaticClass()))
		Size = 1;
	else if (ShipType->IsChildOf(AVesselShip::StaticClass()))
		Size = 2;
	else if (ShipType->IsChildOf(ASubmarineShip::StaticClass()))
		Size = 3;
	else if (ShipType->IsChildOf(ACruisserShip::StaticClass()))
		Size = 4;
	return Size;
}

bool AShipManager::CheckBoardBoundaries(int32 IndexToCheck, TSubclassOf<AShip> ShipType)
{
	int32 Size = GetShipSize(ShipType);
	if (ShipType->IsChildOf(ABoatShip::StaticClass())) // Always true for Boat
		return true;
	else if (ShipType->IsChildOf(AVesselShip::StaticClass()) &&
		IndexToCheck + BattleShipBoardPtr->Size < BattleShipBoardPtr->Size*BattleShipBoardPtr->Size) //Vertical Check for Vessel
		return true;
	else if ((ShipType->IsChildOf(ACruisserShip::StaticClass()) || ShipType->IsChildOf(ASubmarineShip::StaticClass())) &&
		((IndexToCheck % BattleShipBoardPtr->Size) + Size) <= BattleShipBoardPtr->Size) // Horizontal Check for Submarine and Cruisser
		return true;
	return false;
}

bool AShipManager::CheckCollisions(int32 IndexToCheck, TSubclassOf<AShip> ShipType)
{
	int32 Size = GetShipSize(ShipType);
	
	if (ShipType->IsChildOf(AVesselShip::StaticClass())) { // Is vertical
		for (int i = 0; i < Size; i++) {
			if (BattleShipBoardPtr->GetBlockByIndex(IndexToCheck + BattleShipBoardPtr->Size*i)->bHasShip)
				return false;	// If the block is already occupied is not valid
		}
	}
	else	// Is Horizontal
	{
		for (int i = 0; i < Size; i++) {
			if (BattleShipBoardPtr->GetBlockByIndex(IndexToCheck + i)->bHasShip)
				return false;	// If the block is already occupied is not valid
		}
	}

	return true; // Is valid
}

bool AShipManager::CheckShipBoundaries(int32 IndexToCheck, TSubclassOf<AShip> ShipType)
{
	int32 ShipSize = GetShipSize(ShipType);
	int32 BoardSize = BattleShipBoardPtr->Size;

	// Used in the diagonal check
	bool left, right, top, bottom = false;

	// Vertical Ships Checks
	if (ShipType->IsChildOf(AVesselShip::StaticClass())) {
		// There are left boxes
		if (IndexToCheck % BoardSize != 0) {
			left = true;
			// If there is a Ship in any of the left
			for (int i = 0; i < ShipSize; i++) {
				if (!IsEmptyBlock(IndexToCheck + BoardSize * i - 1))
					return false;
			}
		}

		// There are bottom boxes
		if (BoardSize < IndexToCheck) {
			bottom = true;
			// If there is a ship in the bottom
				if (!IsEmptyBlock(IndexToCheck - BoardSize))
					return false;
		}

		// There are right boxes
		if (IndexToCheck % BoardSize != 0 || IndexToCheck == 0) {
			right = true;
			// If there is a ship in the right
			for (int i = 0; i < ShipSize; i++) {
				if (!IsEmptyBlock(IndexToCheck + BoardSize * i + 1))
					return false;
			}
		}

		// There are top boxes
		if ((IndexToCheck + BoardSize*ShipSize) < BoardSize*BoardSize) {
			top = true;
			// If there is a ship in any of the top 
				if (!IsEmptyBlock(IndexToCheck + BoardSize * ShipSize))
					return false;
		}

		// Corners Checks
		if (top && left)
			if (!IsEmptyBlock(IndexToCheck + BoardSize * ShipSize - 1))
				return false;
		if (top && right)
			if (!IsEmptyBlock(IndexToCheck + BoardSize * ShipSize + 1))
				return false;
		if (bottom && left)
			if (!IsEmptyBlock(IndexToCheck - BoardSize - 1))
				return false;
		if (bottom && right)
			if (!IsEmptyBlock(IndexToCheck - BoardSize + 1))
				return false;
	}
	//////////////// Horizontal Ships Checks /////////////////
	else{
		// There are left boxes
		if (IndexToCheck % BoardSize != 0) {
			left = true;
			// If there is a Ship on the left
			if (!IsEmptyBlock(IndexToCheck - 1))
				return false;
		}

		// There are bottom boxes
		if (BoardSize < IndexToCheck) {
			bottom = true;
			// If there is a ship in any box of the bottom
			for (int i = 0; i < ShipSize; i++) {
				if (!IsEmptyBlock(IndexToCheck - BoardSize + i))
					return false;
			}
		}

		// There are right boxes
		if ((IndexToCheck + ShipSize) % BoardSize != 0 || IndexToCheck == 0) {
			right = true;
			// If there is a ship in the right
			if (!IsEmptyBlock(IndexToCheck + ShipSize))
				return false;
		}

		// There are top boxes
		if (BoardSize*(BoardSize - 1) > IndexToCheck) {
			top = true;
			// If there is a ship in any of the top 
			for (int i = 0; i < ShipSize; i++) {
				if (!IsEmptyBlock(IndexToCheck + BoardSize * i))
					return false;
			}
		}

		// Corners checks
		if (top && left)
			if (!IsEmptyBlock(IndexToCheck + BoardSize - 1))
				return false;
		if (top && right)
			if (!IsEmptyBlock(IndexToCheck + BoardSize + ShipSize))
				return false;
		if (bottom && left)
			if (!IsEmptyBlock(IndexToCheck - BoardSize - 1))
				return false;
		if (bottom && right)
			if (!IsEmptyBlock(IndexToCheck - BoardSize + ShipSize))
				return false;

	}


	return true;
}

bool AShipManager::IsEmptyBlock(int32 IndexToCheck)
{
	if (IndexToCheck >= 0 && IndexToCheck < 100) {
		ABlock* Block = BattleShipBoardPtr->GetBlockByIndex(IndexToCheck);
		if (Block->bHasShip)
			return false;
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("%%%%%%%%%%%%[EXCEPTION ] >> %d %%%%%%%%%%%%%\n\n\n"), IndexToCheck);
		return false;
	}

	return true;
}
