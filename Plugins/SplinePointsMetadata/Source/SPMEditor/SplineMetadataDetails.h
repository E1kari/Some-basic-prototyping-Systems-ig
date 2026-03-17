#pragma once

#include "CoreMinimal.h"
#include "SplineMetadataDetailsFactory.h"
#include "SPM/ZoneData.h"
#include "SplineMetadataDetails.generated.h"

class UCustomSplineMetadata;
class USplineComponent;
class SVerticalBox;

UCLASS()
class SPMEDITOR_API UMySplineMetadataDetailsFactory : public USplineMetadataDetailsFactoryBase
{
	GENERATED_BODY()

public:
	virtual TSharedPtr<ISplineMetadataDetails> Create() override;
	virtual UClass* GetMetadataClass() const override;
};

class SPMEDITOR_API FSplineMetadataDetails
	: public ISplineMetadataDetails
	, public TSharedFromThis<FSplineMetadataDetails>
{
public:
	virtual ~FSplineMetadataDetails() = default;

	virtual FName GetName() const override;
	virtual FText GetDisplayName() const override;
	virtual void Update(USplineComponent* InSplineComponent, const TSet<int32>& InSelectedKeys) override;
	virtual void GenerateChildContent(IDetailGroup& InGroup) override;

private:
	UCustomSplineMetadata* GetMetadata() const;
	FSplinePointParams* GetSelectedPointParams() const;
	const FSplinePointParams* GetSelectedPointParamsConst() const;

	void RebuildZoneArrayWidget();
	void NotifyChanged();

	void OnAddZoneLayer();
	void OnRemoveZoneLayer(int32 LayerIndex);

	TOptional<float> GetLayerDistance(int32 LayerIndex) const;
	void OnLayerDistanceCommitted(float NewValue, ETextCommit::Type CommitType, int32 LayerIndex);

	void OnLayerZoneNameChanged(TSharedPtr<EZoneName> NewSelection, ESelectInfo::Type SelectInfo, int32 LayerIndex);
	FText GetCurrentZoneLabel(int32 LayerIndex) const;

private:
	USplineComponent* SplineComp = nullptr;
	TSet<int32> SelectedKeys;

	TArray<TSharedPtr<EZoneName>> ZoneOptions;
	TSharedPtr<SVerticalBox> ZoneListBox;
};