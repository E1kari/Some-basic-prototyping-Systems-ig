
#pragma once

#include <CoreMinimal.h>
#include <SplineMetadataDetailsFactory.h>
#include "SPM/zoneData.h"

#include "SplineMetadataDetails.generated.h"

class FSplineMetadataDetails;
class UCustomSplineMetadata;

UCLASS()
class SPMEDITOR_API UMySplineMetadataDetailsFactory : public USplineMetadataDetailsFactoryBase
{
	GENERATED_BODY()

	virtual ~UMySplineMetadataDetailsFactory() {}
	virtual TSharedPtr<ISplineMetadataDetails> Create() override;
	virtual UClass* GetMetadataClass() const override;
};

class SPMEDITOR_API FSplineMetadataDetails : public ISplineMetadataDetails, public TSharedFromThis<FSplineMetadataDetails>
{
public:
	virtual FName GetName() const override;
	virtual FText GetDisplayName() const override;

private:
	
};
