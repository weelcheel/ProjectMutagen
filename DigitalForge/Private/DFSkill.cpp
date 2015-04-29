#include "DigitalForge.h"
#include "DFSkill.h"

ADFSkill::ADFSkill(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void ADFSkill::SkillUsed()
{
	//@todo: check to see if the player owner can perform skills right now

	PerformSkill();
}