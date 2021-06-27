// Copyright Epic Games, Inc. All Rights Reserved.

#include "PawnBase.h"
#include "../GameObjects/ItemBase.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include <Components/CapsuleComponent.h>

APawnBase::APawnBase(const FObjectInitializer& Obj)
	: Super(Obj)
{
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("SM_Body");
	BodyMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	RootComponent = BodyMesh;
	
	HandMesh = CreateDefaultSubobject<UStaticMeshComponent>("SM_Hand");;
	HandMesh->SetupAttachment(BodyMesh);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("PawnMovement_Comp");
	FloatingPawnMovement->bConstrainToPlane = true;
	FloatingPawnMovement->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);
	
	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>("Rotating_Comp");
	RotatingMovementComponent->RotationRate.Yaw = 0.f;

	PrimaryActorTick.bCanEverTick = true;

}

const AItemBase* APawnBase::GetCurrentItem()
{
	return CurrentItem;
}

void APawnBase::PickUpItem(AActor* InItem)
{
	if (InItem && HandMesh)
	{
		if (AItemBase* ItemToPickUp = Cast<AItemBase>(InItem))
		{
			CurrentItem = ItemToPickUp;
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, true);
			
			CurrentItem->AttachToComponent(HandMesh, AttachmentRules);
			CurrentItem->SetLastItemOwner(this);
		}
	}
}

void APawnBase::ThrowItem()
{
	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, EDetachmentRule::KeepWorld, true);
	
	if (CurrentItem && CurrentItem->GetMesh())
	{
		CurrentItem->GetMesh()->SetSimulatePhysics(true);
		CurrentItem->DetachFromActor(DetachmentRules);
		CurrentItem = nullptr;
	}
}
