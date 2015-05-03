#include "DigitalForge.h"
#include "DFSkill.h"

ADFSkill::ADFSkill(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

/**
 * function called in code for the skill to be activated
 */
void ADFSkill::SkillUsed()
{
	//@todo: check to see if the player owner can perform skills right now

	PerformSkill();
}


float ADFSkill::GetSkillCooldown(){

	return SkillCooldown;
}


void ADFSkill::SetSkillCooldown(float newVal){

	SkillCooldown = newVal;
}