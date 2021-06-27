// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/DataTable.h"
#include "PawnBase.generated.h"

class AItemBase;
class URotatingMovementComponent;
class UFloatingPawnMovement;
class UCapsuleComponent;

UCLASS(Abstract)
class HIDEANDSEEKWITHAI_API APawnBase : public APawn
{
	GENERATED_BODY()

public:
	APawnBase(const FObjectInitializer& Obj);
		
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPriveteAccess = "true"))
	UStaticMeshComponent* BodyMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPriveteAccess = "true"))
	UStaticMeshComponent* HandMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPriveteAccess = "true"))
	URotatingMovementComponent* RotatingMovementComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPriveteAccess = "true"))
	UFloatingPawnMovement* FloatingPawnMovement = nullptr;

	UPROPERTY(BlueprintReadOnly)
	AItemBase* CurrentItem = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = FirstSetup)
	FDataTableRowHandle SetupDataTable;

public:
	UFUNCTION(BlueprintCallable)
	virtual void PickUpItem(AActor* InItem);

	UFUNCTION(BlueprintCallable)
	virtual void ThrowItem();

	const AItemBase* GetCurrentItem();
	
}; 