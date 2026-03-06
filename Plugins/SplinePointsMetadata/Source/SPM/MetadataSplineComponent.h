// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/SplineComponent.h>
#include <CoreMinimal.h>


#include "MetadataSplineComponent.generated.h"

class UCustomSplineMetadata;

USTRUCT()

struct FSplinePointParams {
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  float TestFloat = 1.0f;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class SPM_API UMetadataSplineComponent : public USplineComponent {
  GENERATED_BODY()

public:
  UMetadataSplineComponent();

  UFUNCTION(BlueprintCallable)
  float GetTestFloatAtSplinePoint(int32 PointIndex);
  
  UCustomSplineMetadata *GetSplineMetadata() const;
  
  virtual USplineMetadata *GetSplinePointsMetadata() override;
  virtual const USplineMetadata *GetSplinePointsMetadata() const override;
  virtual void PostLoad() override;
  virtual void PostDuplicate(bool bDuplicateForPie) override;
#if WITH_EDITOR
  virtual void
  PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
  virtual void PostEditImport() override;
#endif
  void FixupPoints();
  
  private:
  void EnsureSplineMetadata();

  UPROPERTY(Instanced, Export)
  UCustomSplineMetadata *SplineMetadata = nullptr;
};
