#include "PlayerBase.h"

#include "GameFramework/RotatingMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include <Kismet/KismetSystemLibrary.h>
#include "../../GameObjects/ItemBase.h"
#include "../../HideAndSeekGameMode.h"
#include <Kismet/GameplayStatics.h>

APlayerBase::APlayerBase(const FObjectInitializer& Obj)
	: Super(Obj)
{
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(BodyMesh);

	PrimaryActorTick.bCanEverTick = true;
}

float APlayerBase::GetHoldMouseButtonTime()
{
	return HoldMouseButtonTime;
}

bool APlayerBase::GetCanPickUpState()
{
	return bCanPickUp;
}

bool APlayerBase::GetHasItemState()
{
	return bHasItem;
}

bool APlayerBase::GetCaughtBotsState()
{
	return bCaughtByBots;
}

void APlayerBase::ChangeCaughtBotsState(bool bNewState)
{
	bCaughtByBots = bNewState;
}

void APlayerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	FKey LeftMouseButton(FName("LeftMouseButton"));	
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(LeftMouseButton) && CurrentItem)
	{
		HoldMouseButtonTime = GetWorld()->GetFirstPlayerController()->GetInputKeyTimeDown(LeftMouseButton);
	}

	if (CurrentItem == nullptr)
	{
		bHasItem = false;
		MakeTraceToFindItem(GetPlayerSettingsRowHandle().ItemPickUpRadius, ItemsToPickUp, EDrawDebugTrace::None);

		if (ItemsToPickUp.Num() > 0)
		{
			bCanPickUp = true;
		}
		else
		{
			bCanPickUp = false;
		}
	}
	else
	{
		bHasItem = true;
	}
}

void APlayerBase::ThrowItem()
{
	GetWorldTimerManager().ClearTimer(ThrowItemTimerHandle);
	
	FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, EDetachmentRule::KeepRelative, EDetachmentRule::KeepWorld, true);

	if (CurrentItem && CurrentItem->GetMesh())
	{
		CurrentItem->GetMesh()->SetSimulatePhysics(true);
		CurrentItem->DetachFromActor(DetachmentRules);
		
		FVector Impulse = GetActorForwardVector() * (HoldMouseButtonTime * GetPlayerSettingsRowHandle().ForceCoefficient);
		CurrentItem->GetMesh()->AddImpulse(Impulse, NAME_None, true);

		CurrentItem = nullptr;
	}

	bCanThrow = false;
	HoldMouseButtonTime = 0.f;
}

bool APlayerBase::MakeTraceToFindItem(float Radius, TArray<FHitResult>& OutHits, EDrawDebugTrace::Type InDrawDebugTrace)
{
	return UKismetSystemLibrary::SphereTraceMulti(this, GetActorLocation(), GetActorLocation(), Radius, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, GameObjectForTraceIgnore, InDrawDebugTrace, OutHits, true, FLinearColor::Red, FLinearColor::Green, 1.0f);
}

const FPlayerSetup& APlayerBase::GetPlayerSettingsRowHandle() const
{
	const FString Contecxt("APlayerBase::GetPlayerSettingsRowHandle");
	FPlayerSetup* Settings = SetupDataTable.GetRow<FPlayerSetup>(Contecxt);
	check(Settings);
	return *Settings;
}

void APlayerBase::OnLeftMouseButtonPressed()
{
	if (CurrentItem)
	{
		GetWorldTimerManager().SetTimer(ThrowItemTimerHandle, this, &APlayerBase::ThrowItem, GetPlayerSettingsRowHandle().MaxHoldMouseButtonTime, false);
		bCanThrow = true;
		return;
	}
	
	if (CurrentItem == nullptr)
	{
		if (ItemsToPickUp.Num() > 0)
		{
			if (AItemBase* ItemToPickUp = Cast<AItemBase>(ItemsToPickUp[0].GetActor()))
			{
				PickUpItem(ItemToPickUp);
			}
		}
	}
}

void APlayerBase::OnLeftMouseButtonReleased()
{
	if (CurrentItem && bCanThrow)
	{
		GetWorldTimerManager().ClearTimer(ThrowItemTimerHandle);
		ThrowItem();
	}
	else
	{
		HoldMouseButtonTime = 0;
	}
}

void APlayerBase::PostInitializeComponents()
{
	if (SetupDataTable.DataTable && FloatingPawnMovement)
	{
		FloatingPawnMovement->MaxSpeed = GetPlayerSettingsRowHandle().MaxPlayerSpeed;
		FloatingPawnMovement->Acceleration = GetPlayerSettingsRowHandle().MaxPlayerSpeed;
	}

	Super::PostInitializeComponents();
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Obstacle", GameObjectForTraceIgnore);
	}
	
}
