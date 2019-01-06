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
	//int32 RandomIndex = FMath::Rand() % (Size * Size);
	int32 RandomIndex = 6;
	while (!IsValidIndex(RandomIndex, ShipType)) {	// Get different index when it is not valid
		UE_LOG(LogTemp, Log, TEXT("nnnnnnnnnnnnnnnnnnnnnnnnREPEAT! %d\n\n\n"), RandomIndex);
		RandomIndex = FMath::Rand() % (Size * Size);	
	}

	// Logic to spawn each ship
	TWeakObjectPtr<ABlock> Block = BattleShipBoardPtr->GetBlockByIndex(RandomIndex);

	// Location of Block
	FVector Blocklocation = Block->GetActorLocation();

	// IsChildOf compares if Class == OtherClass or Class == Child of Other Class
	if (ShipType->IsChildOf(ABoatShip::StaticClass())) {
		// Spawn boat
		Blocklocation += FVector(0.0f, 0.0f, 10.0f); // Offset due to the scale
		TWeakObjectPtr<ABoatShip> NewShip = GetWorld()->SpawnActor<ABoatShip>(Blocklocation, FRotator(0.0f, 45.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(30.0f, 30.0f, 30.0f));
		UE_LOG(LogTemp, Log, TEXT("--------[Boat] >> %d-----------\n\n\n"), RandomIndex);
		NewShip->SetOccupiedBlocks(RandomIndex, BattleShipBoardPtr);
	}
	else if (ShipType->IsChildOf(AVesselShip::StaticClass())) {
		// Spawn Vessel
		Blocklocation += FVector(30.0f , 0.0f, 40.0f); // Offset due to the scale
		TWeakObjectPtr<AVesselShip> NewShip = GetWorld()->SpawnActor<AVesselShip>(Blocklocation, FRotator(0.0f, 0.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(55.0f, 55.0f, 55.0f));
		UE_LOG(LogTemp, Log, TEXT("--------[Vessel] >> %d-----------\n\n\n"), RandomIndex);
		NewShip->SetOccupiedBlocks(RandomIndex, BattleShipBoardPtr);
	}
	else if (ShipType->IsChildOf(ASubmarineShip::StaticClass())) {
		// Spawn Submarine
		Blocklocation += FVector(0.0f, 140.0f, 0.0f); // Offset due to the scale
		TWeakObjectPtr<ASubmarineShip> NewShip = GetWorld()->SpawnActor<ASubmarineShip>(Blocklocation, FRotator(0.0f, 0.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(20.0f, 20.0f, 20.0f));
		UE_LOG(LogTemp, Log, TEXT("--------[Submarine] >> %d-----------\n\n\n"), RandomIndex);
		NewShip->SetOccupiedBlocks(RandomIndex, BattleShipBoardPtr);
	}
	else if(ShipType->IsChildOf(ACruisserShip::StaticClass())){
		// Spawn Cruiser
		Blocklocation += FVector(0.0f, 130.0f, 0.0f); // Offset due to the scale
		TWeakObjectPtr<ACruisserShip> NewShip = GetWorld()->SpawnActor<ACruisserShip>(Blocklocation, FRotator(0.0f, 0.0f, 90.0f));
		NewShip->GetShipMesh()->SetRelativeScale3D(FVector(7.0f, 7.0f, 7.0f));
		UE_LOG(LogTemp, Log, TEXT("--------[Cruisser] >> %d-----------\n\n\n"), RandomIndex);
		NewShip->SetOccupiedBlocks(RandomIndex, BattleShipBoardPtr);
	}
	
}

bool AShipManager::IsValidIndex(int32 IndexToCheck, TSubclassOf<AShip> ShipType)
{
	//int32 Size = GetShipSize(ShipType);
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
	return true;
}
