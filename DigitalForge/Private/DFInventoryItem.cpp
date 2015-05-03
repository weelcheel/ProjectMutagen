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

/**
 * set the inventory's owning pawn
 */
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

/**
 * [server] weapon was added to pawn's inventory
 */
void ADFInventoryItem::OnEnterInventory(ADigitalForgeCharacter* NewOwner)
{
	SetOwningPawn(NewOwner);
}

/**
 * [server] weapon was removed from pawn's inventory
 */
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


ADigitalForgeCharacter* ADFInventoryItem::GetMyPawn(){

	return MyPawn;
}


void ADFInventoryItem::SetMyPawn(ADigitalForgeCharacter* newVal){

	MyPawn = newVal;
}