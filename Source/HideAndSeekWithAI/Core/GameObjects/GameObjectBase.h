// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameObjectBase.generated.h"

UCLASS(Abstract)
class HIDEANDSEEKWITHAI_API AGameObjectBase : public AActor
{
	GENERATED_BODY()

public:
	AGameObjectBase(const FObjectInitializer& Obj);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPriveteAccess = "true"))
	UStaticMeshComponent* GameObjectMesh = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	AActor* SpotRef = nullptr;

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	virtual void DestroyActor();

	UStaticMeshComponent* GetMesh();
	
	UPROPERTY(BlueprintReadOnly)
	FVector SpawnLocation;
};
