// Fill out your copyright notice in the Description page of Project Settings.

#include "MetadataSplineComponent.h"

#include "CustomSplineMetadata.h"


UMetadataSplineComponent::UMetadataSplineComponent()
{
  SplineMetadata = CreateDefaultSubobject<UCustomSplineMetadata>(TEXT("SplineMetadata"));
}

void UMetadataSplineComponent::EnsureSplineMetadata()
{
  if (!SplineMetadata)
  {
    SplineMetadata = NewObject<UCustomSplineMetadata>(this, TEXT("SplineMetadata"), RF_Transactional);
  }
}

TArray<FZoneLayer> UMetadataSplineComponent::GetTestDataAtSplinePoint(int32 PointIndex)
{
  if (ensure(SplineMetadata))
  {
    return SplineMetadata->PointParams.ZoneLayers;
  }

  return TArray<FZoneLayer>();
}

UCustomSplineMetadata* UMetadataSplineComponent::GetSplineMetadata() const 
{ 
  return SplineMetadata; 
}

USplineMetadata *UMetadataSplineComponent::GetSplinePointsMetadata() {
  EnsureSplineMetadata();
  return SplineMetadata;
}

const USplineMetadata *UMetadataSplineComponent::GetSplinePointsMetadata() const {
  return SplineMetadata;
}

void UMetadataSplineComponent::FixupPoints() {
  EnsureSplineMetadata();
#if WITH_EDITORONLY_DATA
  // Keep metadata in sync
  if (GetSplinePointsMetadata()) {
    const int32 NumPoints = GetNumberOfSplinePoints();
    GetSplinePointsMetadata()->Fixup(NumPoints, this);
  }
#endif
}

void UMetadataSplineComponent::PostLoad() {
  Super::PostLoad();

  EnsureSplineMetadata();
  FixupPoints();
}

void UMetadataSplineComponent::PostDuplicate(bool bDuplicateForPie) {
  Super::PostDuplicate(bDuplicateForPie);

  EnsureSplineMetadata();
  FixupPoints();
}

#if WITH_EDITOR
void UMetadataSplineComponent::PostEditChangeProperty(
    FPropertyChangedEvent &PropertyChangedEvent) {
  Super::PostEditChangeProperty(PropertyChangedEvent);

  FixupPoints();
}

void UMetadataSplineComponent::PostEditImport() {
  Super::PostEditImport();

  FixupPoints();
}
#endif

