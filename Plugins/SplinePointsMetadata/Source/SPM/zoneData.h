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
	Zone2       UMETA(DisplayName = "Zone 2"),
	Count		UMETA(Hidden)
	
};
ENUM_RANGE_BY_COUNT(EZoneName, EZoneName::Count)


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

UCLASS(BlueprintType)
class UZoneLayer : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EZoneName ZoneName = EZoneName::ZoneDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Distance = 10.0f;
	
	
	bool operator==(const UZoneLayer& Other) const
	{
		return ZoneName == Other.ZoneName
			&& Distance == Other.Distance;
	}

	bool operator!=(const UZoneLayer& Other) const
	{
		return !(*this == Other);
	}
};

UCLASS(BlueprintType)
class USplinePointParams : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UZoneLayer*> ZoneLayers;
};

UCLASS(BlueprintType)
class UZoneLibraryAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FZoneData> Zones;
};