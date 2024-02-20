// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


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

void ABlasterCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}
