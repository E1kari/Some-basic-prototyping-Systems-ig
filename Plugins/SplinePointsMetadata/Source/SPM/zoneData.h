#pragma once

#include "CoreMinimal.h"
#include "ZoneData.generated.h"

UENUM(BlueprintType)
enum class EZoneName : uint8
{
	ZoneDefault UMETA(DisplayName = "Default"),
	ZoneCone    UMETA(DisplayName = "Cone"),
	Zone0       UMETA(DisplayName = "Zone 0"),
	Zone1       UMETA(DisplayName = "Zone 1"),
	Zone2       UMETA(DisplayName = "Zone 2")
};

USTRUCT(BlueprintType)
struct FZoneData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EZoneName ZoneName = EZoneName::ZoneDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rubberband = 0.25f;
};

USTRUCT(BlueprintType)
struct FZoneDistanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FZoneData ZoneData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance = 10.0f;
};