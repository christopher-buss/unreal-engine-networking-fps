// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/Weapon/Weapon.h"
#include "Components/SphereComponent.h"
#include "Engine/SkeletalMeshSocket.h"


UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	check(BlasterCharacter != nullptr);
	check(WeaponToEquip != nullptr);

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::Equipped);

	const USkeletalMeshSocket* HandSocket = BlasterCharacter->GetMesh()->GetSocketByName(FName { "RightHandSocket" });
	if (!HandSocket)
	{
		return;
	}

	HandSocket->AttachActor(EquippedWeapon, BlasterCharacter->GetMesh());
	EquippedWeapon->SetOwner(BlasterCharacter);
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}
