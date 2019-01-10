// Fill out your copyright notice in the Description page of Project Settings.

#include "Ship.h"
#include "EngineMinimal.h"

// Sets default values
AShip::AShip() 
{
	//Initialize as not destroyed
	isDestroyed = false;

	// Save pointer to audio explosion
	ExplosionParticleSystem = ConstructorHelpers::FObjectFinder<UParticleSystem>
		(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion")).Object;

	// Save a pointer to explosion
	AudioExplosion = ConstructorHelpers::FObjectFinder<USoundBase>
		(TEXT("/Game/StarterContent/Audio/Explosion01.Explosion01")).Object;
}

void AShip::FireExplosionIfItIsHit(ABlock* Block)
{
	// Play audio explosion
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), AudioExplosion.Get(), Block->GetActorLocation());

	// Fire explosion particle system
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticleSystem.Get(), Block->GetActorLocation());
}
