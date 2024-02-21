// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class USphereComponent;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Initial UMETA(DisplayName = "Initial"),
	Equipped UMETA(DisplayName = "Equipped"),
	Dropped UMETA(DisplayName = "Dropped"),
	Max UMETA(DisplayName = "Max")
};

UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	AWeapon();

	void ShowPickupWidget(bool bShowWidget) const;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                     const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UFUNCTION()
	void OnRep_WeaponState();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> AreaSphere;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponProperties", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> PickupWidget;

	UPROPERTY(ReplicatedUsing = OnRep_WeaponState, VisibleAnywhere, BlueprintReadOnly, Category = "WeaponProperties",
		meta = (AllowPrivateAccess = "true"))
	EWeaponState WeaponState;

public:
	void SetWeaponState(const EWeaponState NewState);

	FORCEINLINE const TObjectPtr<USphereComponent>& GetAreaSphere() const {	return AreaSphere; }
};
