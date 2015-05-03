#include "DigitalForge.h"
#include "DFInventoryItem.h"
#include "DigitalForgeCharacter.h"
#include "UnrealNetwork.h"

ADFInventoryItem::ADFInventoryItem(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

FString ADFInventoryItem::GetItemName()
{
	return ItemName;
}

ADigitalForgeCharacter* ADFInventoryItem::GetPawnOwner() const
{
	return MyPawn;
}

void ADFInventoryItem::GetLifetimeReplicatedProps( TArray< class FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADFInventoryItem, MyPawn);
}

void ADFInventoryItem::SetOwningPawn(ADigitalForgeCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
		// net owner for RPC calls
		SetOwner(NewOwner);
	}	
}

void ADFInventoryItem::OnEnterInventory(ADigitalForgeCharacter* NewOwner)
{
	SetOwningPawn(NewOwner);
}

void ADFInventoryItem::OnLeaveInventory()
{
	if (Role == ROLE_Authority)
	{
		SetOwningPawn(NULL);
	}
}

void ADFInventoryItem::OnRep_MyPawn()
{
	if (MyPawn)
	{
		OnEnterInventory(MyPawn);
	}
	else
	{
		OnLeaveInventory();
	}
}