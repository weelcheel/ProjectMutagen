#pragma once

#include "DigitalForgeCharacter.h"
#include "DFNPCharacter.generated.h"

class ADFPlayerCharacter;

UCLASS(Blueprintable)
class ADFNPCharacter : public ADigitalForgeCharacter
{
	GENERATED_UCLASS_BODY()

protected:

	UFUNCTION(BlueprintCallable, Category=PlayerUse)
	void OnPlayerCharacterInUseRange(ADFPlayerCharacter* InRangePlayer);

	UFUNCTION(BlueprintCallable, Category=PlayerUse)
	void OnPlayerCharacterOutUseRange(ADFPlayerCharacter* OutRangePlayer);

	/** The camera that the player will view when they 'use' the NPC*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* UseCamera;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:

	void OnPlayerUse(ADFPlayerCharacter* usingPlayer);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLevelNPCUsedDelegate_OnNPCUsed, ADFPlayerCharacter*, UsingPlayer);

	UPROPERTY(BlueprintAssignable, Category="Level NPC Events")
	FLevelNPCUsedDelegate_OnNPCUsed OnNPCUsed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category=NPC)
	FString NPCName;
};