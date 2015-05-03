#pragma once

#include "DFInventoryItem.h"
#include "DFSkill.generated.h"

UCLASS(Blueprintable)
class ADFSkill : public ADFInventoryItem
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintReadWrite, Category=SkillDetails)
	float SkillCooldown;

	//@todo: instance of stats class that this ability has

	/** function called in code for the skill to be activated */
	void SkillUsed();

public:

	/** event triggered in blueprints that will let each skill behave differently */
	UFUNCTION(BlueprintImplementableEvent, Category=Skill)
	void PerformSkill();
	float GetSkillCooldown();
	void SetSkillCooldown(float newVal);
};