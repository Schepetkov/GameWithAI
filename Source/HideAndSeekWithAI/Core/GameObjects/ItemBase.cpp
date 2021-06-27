#include "ItemBase.h"

void AItemBase::SetLastItemOwner(APawnBase* InLastItemOwned)
{
	LastItemOwned = InLastItemOwned;
}

const APawnBase* AItemBase::GetLastItemOwner()
{
	if (LastItemOwned)
	{
		return LastItemOwned;
	}

	return nullptr;
}