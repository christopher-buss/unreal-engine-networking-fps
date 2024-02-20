// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// GameState.Get()->PlayerArray
	if (NumPlayers < 2)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	bUseSeamlessTravel = true;
	World->ServerTravel(FString { "/Game/Maps/BlasterMap?listen" });
}
