// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "Blaster/Character/BlasterCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Net/UnrealNetwork.h"


AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AreaSphere"));
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);
}

void AWeapon::ShowPickupWidget(const bool bShowWidget) const
{
	if (!ensure(PickupWidget != nullptr))
	{
		return;
	}

	PickupWidget->SetVisibility(bShowWidget);
}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, WeaponState);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	PickupWidget->SetVisibility(false);

	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnSphereOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnSphereEndOverlap);
	}
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* /*OverlappedComponent*/, AActor* OtherActor,
                              UPrimitiveComponent* /*OtherComp*/, int32 /*OtherBodyIndex*/, bool /*bFromSweep*/,
                              const FHitResult& /*SweepResult*/)
{
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (!BlasterCharacter)
	{
		return;
	}

	BlasterCharacter->SetOverlappingWeapon(this);
}

// ReSharper disable once CppMemberFunctionMayBeStatic (Needed for dynamic binding)
void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* /*OverlappedComponent*/, AActor* OtherActor,
                                 UPrimitiveComponent* /*OtherComp*/, int32 /*OtherBodyIndex*/)
{
	ABlasterCharacter* BlasterCharacter = Cast<ABlasterCharacter>(OtherActor);
	if (!BlasterCharacter)
	{
		return;
	}

	BlasterCharacter->SetOverlappingWeapon(nullptr);
}

// ReSharper disable once CppMemberFunctionMayBeConst (Needed for dynamic binding)
void AWeapon::OnRep_WeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::Equipped:
		{
			ShowPickupWidget(false);
			AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		}
	default: break;
	}
}

void AWeapon::SetWeaponState(const EWeaponState NewState)
{
	WeaponState = NewState;
	
	switch (WeaponState)
	{
	case EWeaponState::Equipped:
		{
			ShowPickupWidget(false);
			GetAreaSphere()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		}
	default: break;
	}
}
