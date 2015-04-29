#pragma once

#include "DigitalForgeCharacter.h"
#include "DFPlayerCharacter.generated.h"

class ADFNPCharacter;

UCLASS(Blueprintable)
class ADFPlayerCharacter : public ADigitalForgeCharacter
{
	GENERATED_UCLASS_BODY()

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

protected:

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	void PlayerUse();

public:

	UPROPERTY(BlueprintReadOnly, Category=NPC)
	ADFNPCharacter* PlayerUseableNPC;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};