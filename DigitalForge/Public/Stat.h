#pragma once

#include "Stat.generated.h"

UCLASS()
class UStat : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	float Value;
	FString Name;

	FORCEINLINE		UStat&		operator+( UStat &astat )
	{
		UStat* nstat = ConstructObject<UStat>(UStat::StaticClass());
		if (nstat)
		{
			nstat->Value = astat.Value + Value;
		}

		return *nstat;
	}

	FORCEINLINE		UStat&		operator+=( UStat &astat )
	{
		Value += astat.Value;

		return *this;
	}
	FORCEINLINE		UStat&		operator-( UStat &astat )
	{
		UStat* nstat = ConstructObject<UStat>(UStat::StaticClass());
		if (nstat)
		{
			nstat->Value = astat.Value - Value;
		}

		return *nstat;
	}

	FORCEINLINE		UStat&		operator-=( UStat &astat )
	{
		Value -= astat.Value;

		return *this;
	}
	FORCEINLINE		UStat&		operator*( UStat &astat )
	{
		UStat* nstat = ConstructObject<UStat>(UStat::StaticClass());
		if (nstat)
		{
			nstat->Value = astat.Value * Value;
		}

		return *nstat;
	}

	FORCEINLINE		UStat&		operator*=( UStat &astat )
	{
		Value *= astat.Value;

		return *this;
	}
	FORCEINLINE		UStat&		operator/( UStat &astat )
	{
		UStat* nstat = ConstructObject<UStat>(UStat::StaticClass());
		if (nstat)
		{
			nstat->Value = astat.Value / Value;
		}

		return *nstat;
	}

	FORCEINLINE		UStat&		operator/=( UStat &astat )
	{
		Value /= astat.Value;

		return *this;
	}
};