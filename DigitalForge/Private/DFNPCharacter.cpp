#include "DigitalForge.h"
#include "DFNPCharacter.h"
#include "DFPlayerCharacter.h"

ADFNPCharacter::ADFNPCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	CameraBoom->AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 95.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	UseCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("UseCamera"));
	UseCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);
	UseCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ADFNPCharacter::OnPlayerCharacterInUseRange(ADFPlayerCharacter* InRangePlayer)
{
	if (InRangePlayer != NULL)
	{
		if (InRangePlayer->PlayerUseableNPC && InRangePlayer->PlayerUseableNPC != this)
		{
			FVector dv = InRangePlayer->GetActorLocation() - InRangePlayer->PlayerUseableNPC->GetActorLocation();
			FVector ndv = InRangePlayer->GetActorLocation() - GetActorLocation();

			if (ndv.SizeSquared() < dv.SizeSquared())
				InRangePlayer->PlayerUseableNPC = this;
		}
		else
			InRangePlayer->PlayerUseableNPC = this;
		
	}
}

void ADFNPCharacter::OnPlayerCharacterOutUseRange(ADFPlayerCharacter* OutRangePlayer)
{
	if (OutRangePlayer)
	{
		if (OutRangePlayer->PlayerUseableNPC == this)
			OutRangePlayer->PlayerUseableNPC = NULL;
	}
}

void ADFNPCharacter::OnPlayerUse(ADFPlayerCharacter* usingPlayer)
{
	OnNPCUsed.Broadcast(usingPlayer);
}


/**
 * The camera that the player will view when they 'use' the NPC
 */
UCameraComponent* ADFNPCharacter::GetUseCamera(){

	return UseCamera;
}


/**
 * The camera that the player will view when they 'use' the NPC
 */
void ADFNPCharacter::SetUseCamera(UCameraComponent* newVal){

	UseCamera = newVal;
}


FString ADFNPCharacter::GetNPCName(){

	return NPCName;
}


void ADFNPCharacter::SetNPCName(FString newVal){

	NPCName = newVal;
}


/**
 * Camera boom positioning the camera behind the character
 */
USpringArmComponent* ADFNPCharacter::GetCameraBoom(){

	return CameraBoom;
}


/**
 * Camera boom positioning the camera behind the character
 */
void ADFNPCharacter::SetCameraBoom(USpringArmComponent* newVal){

	CameraBoom = newVal;
}