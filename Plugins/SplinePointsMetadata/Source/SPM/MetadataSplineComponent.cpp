// Fill out your copyright notice in the Description page of Project Settings.

#include "MetadataSplineComponent.h"

#include "MetadataSplineActor.h"
#include "CustomSplineMetadata.h"

USplineMetadata *UMetadataSplineComponent::GetSplinePointsMetadata() {
  if (AMetadataSplineActor *Actor = Cast<AMetadataSplineActor>(GetOwner())) {
    return Actor->GetSplineMetadata();
  }

  return nullptr;
}

const USplineMetadata *UMetadataSplineComponent::GetSplinePointsMetadata() const {
  if (AMetadataSplineActor *Actor = Cast<AMetadataSplineActor>(GetOwner())) {
    return Actor->GetSplineMetadata();
  }

  return nullptr;
}

void UMetadataSplineComponent::FixupPoints() {
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

  FixupPoints();
}

void UMetadataSplineComponent::PostDuplicate(bool bDuplicateForPie) {
  Super::PostDuplicate(bDuplicateForPie);

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
