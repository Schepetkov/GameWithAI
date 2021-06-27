#pragma once

#include "CoreMinimal.h"
#include "GameObjectBase.h"

#include "ItemBase.generated.h"

class APawnBase;

UCLASS(Abstract)
class HIDEANDSEEKWITHAI_API AItemBase : public AGameObjectBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	APawnBase* LastItemOwned = nullptr;

public:
	void SetLastItemOwner(APawnBase* InLastItemOwned);
	
	const APawnBase* GetLastItemOwner();
	
};