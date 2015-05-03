#pragma once

#include "DigitalForgeCharacter.h"
#include "DFPlayerCharacter.generated.h"

class ADFNPCharacter;
class ADFPlayerInventory;

UCLASS(Blueprintable)
class ADFPlayerCharacter : public ADigitalForgeCharacter
{
	GENERATED_UCLASS_BODY()

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom;

protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** get the player inventory from the player controller */
	ADFPlayerInventory* GetPlayerInventory() const;

	void PlayerUse();
	void SkillUse();

public:

	UPROPERTY(BlueprintReadOnly, Category=NPC)
	ADFNPCharacter* PlayerUseableNPC;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};