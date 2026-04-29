#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SplineForward.generated.h"

class USplineMetadata;
class USplineComponent;

UENUM(BlueprintType)
enum class ESplineForwardedEvent : uint8
{
	InsertPoint,
	UpdatePoint,
	AddPoint,
	RemovePoint,
	DuplicatePoint,
	CopyPoint,
	Reset,
	FixupRemove,
	FixupAdd,
};

USTRUCT(BlueprintType)
struct FSplinePayload
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	const USplineMetadata* SplineMetadata = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USplineComponent* SplineComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumPoints = INDEX_NONE;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index = INDEX_NONE;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FromIndex = INDEX_NONE;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InputKey = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float t = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bClosedLoop = false;
};


UINTERFACE(BlueprintType)
class SPM_API USplineForward : public UInterface
{
	GENERATED_BODY()
};

class ISplineForward
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Spline")
	void OnSplineEventForwarded(ESplineForwardedEvent EventName, const FSplinePayload& Payload);
};

