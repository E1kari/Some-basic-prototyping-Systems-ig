// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>

#include "MetadataSplineActor.generated.h"

class UMySplineMetadata;
class UMetadataSplineComponent;

UCLASS()
class SPM_API AMetadataSplineActor : public AActor {
  GENERATED_BODY()

public:
  AMetadataSplineActor();
  UMySplineMetadata *GetSplineMetadata() const;

  UFUNCTION(BlueprintCallable)
  float GetTestFloatAtSplinePoint(int32 PointIndex);

private:
  UPROPERTY(Instanced, Export)
  UMySplineMetadata *MySplineMetadata = nullptr;

  UPROPERTY(VisibleAnywhere)
  UMetadataSplineComponent *MySplineComponent = nullptr;
};
