// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"

#include "Blaster/BlasterComponents/CombatComponent.h"
#include "Blaster/Weapon/Weapon.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"


ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetMesh());
	SpringArmComponent->TargetArmLength = 600.f;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidgetComponent"));
	OverheadWidgetComponent->SetupAttachment(RootComponent);

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComponent->SetIsReplicated(true);
}

void ABlasterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OUT OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ABlasterCharacter, OverlappingWeapon, COND_OwnerOnly);
}

void ABlasterCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!ensureAlways(CombatComponent != nullptr))
	{
		return;
	}

	CombatComponent->BlasterCharacter = this;
}

void ABlasterCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon != nullptr)
	{
		OverlappingWeapon->ShowPickupWidget(false);
	}

	OverlappingWeapon = Weapon;

	if (!IsLocallyControlled() || OverlappingWeapon == nullptr)
	{
		return;
	}

	OverlappingWeapon->ShowPickupWidget(true);
}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABlasterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABlasterCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABlasterCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABlasterCharacter::LookUp);

	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABlasterCharacter::EquipButtonPressed);
}

void ABlasterCharacter::MoveForward(const float Value)
{
	ensureAlways(Controller != nullptr);

	if (Value == 0.f)
	{
		return;
	}

	const FRotator YawRotation { 0.f, Controller->GetControlRotation().Yaw, 0.f };
	const FVector Direction { FRotationMatrix { YawRotation }.GetUnitAxis(EAxis::X) };
	AddMovementInput(Direction, Value);
}

void ABlasterCharacter::MoveRight(const float Value)
{
	ensureAlways(Controller != nullptr);

	if (Value == 0.f)
	{
		return;
	}

	const FRotator YawRotation { 0.f, Controller->GetControlRotation().Yaw, 0.f };
	const FVector Direction { FRotationMatrix { YawRotation }.GetUnitAxis(EAxis::Y) };
	AddMovementInput(Direction, Value);
}

void ABlasterCharacter::Turn(const float Value)
{
	AddControllerYawInput(Value);
}

void ABlasterCharacter::LookUp(const float Value)
{
	AddControllerPitchInput(Value);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void ABlasterCharacter::EquipButtonPressed()
{
	if (!ensureAlways(CombatComponent != nullptr))
	{
		return;
	}

	if (!HasAuthority())
	{
		return;
	}

	CombatComponent->EquipWeapon(OverlappingWeapon);
}

void ABlasterCharacter::OnRep_OverlappingWeapon(const AWeapon* LastWeapon) const
{
	if (OverlappingWeapon != nullptr)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}

	if (LastWeapon != nullptr)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

void ABlasterCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}
