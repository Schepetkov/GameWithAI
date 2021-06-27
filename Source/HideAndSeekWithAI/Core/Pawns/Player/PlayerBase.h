#pragma once

#include "CoreMinimal.h"

#include "../PawnBase.h"
#include <Kismet/KismetSystemLibrary.h>
#include "PlayerBase.generated.h"

class UCameraComponent;
class AGameObjectBase;

USTRUCT(BlueprintType)
struct FPlayerSetup : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ItemPickUpRadius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHoldMouseButtonTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ForceCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPlayerSpeed;
};

UCLASS(Abstract)
class HIDEANDSEEKWITHAI_API APlayerBase : public APawnBase
{
	GENERATED_BODY()

public:
	APlayerBase(const FObjectInitializer& Obj);
	
	UFUNCTION(BlueprintCallable)
	const FPlayerSetup& GetPlayerSettingsRowHandle() const;

	UFUNCTION(BlueprintPure)
	float GetHoldMouseButtonTime();

	UFUNCTION(BlueprintPure)
	bool GetCanPickUpState();

	UFUNCTION(BlueprintPure)
	bool GetHasItemState();

	UFUNCTION(BlueprintPure)
	bool GetCaughtBotsState();

	void ChangeCaughtBotsState(bool bNewState);

protected:
	void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void PostInitializeComponents() override;
	virtual void ThrowItem() override;

	UFUNCTION(BlueprintCallable)
	bool MakeTraceToFindItem(float Radius, TArray<FHitResult>& OutHits, EDrawDebugTrace::Type InDrawDebugTrace);

	UFUNCTION(BlueprintCallable)
	void OnLeftMouseButtonPressed();

	UFUNCTION(BlueprintCallable)
	void OnLeftMouseButtonReleased();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPriveteAccess = "true"))
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TArray<FHitResult> ItemsToPickUp;

private:
	UPROPERTY()
	TArray<AActor*> GameObjectForTraceIgnore;
	
	bool bCaughtByBots = false;
	
	bool bCanPickUp = false;
	bool bCanThrow = false;
	bool bHasItem = false;
	
	float HoldMouseButtonTime;
	
	FTimerHandle ThrowItemTimerHandle;
};
