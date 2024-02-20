// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverheadWidget::SetDisplayText(const FString& TextToDisplay) const
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UOverheadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	const ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;

	switch (RemoteRole)
	{
	case ROLE_Authority:
		{
			Role = FString { "Authority" };
			break;
		}
	case ROLE_AutonomousProxy:
		{
			Role = FString { "AutonomousProxy" };
			break;
		}
	case ROLE_SimulatedProxy:
		{
			Role = FString { "SimulatedProxy" };
			break;
		}
	default:
		{
			Role = FString { "None" };
			break;
		}
	}

	const FString& RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role);
	SetDisplayText(RemoteRoleString);
}

void UOverheadWidget::ShowPlayerName(APawn* InPawn)
{
	const FString& PlayerName = FString::Printf(TEXT("Player Name: %s"), *InPawn->GetPlayerState()->GetPlayerName());
	SetDisplayText(PlayerName);
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();

	Super::NativeDestruct();
}
