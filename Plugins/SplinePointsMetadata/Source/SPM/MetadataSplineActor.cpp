// Fill out your copyright notice in the Description page of Project Settings.

#include "MetadataSplineActor.h"

#include "CustomSplineMetadata.h"

AMetadataSplineActor::AMetadataSplineActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SplineMetadata = CreateDefaultSubobject<UCustomSplineMetadata>(TEXT("SplineMetadata"));
	SplineMetadata->Reset(2);
	SplineMetadata->AddPoint(0.0f);
	SplineMetadata->AddPoint(1.0f);

	MetadataSplineComponent = CreateDefaultSubobject<UMetadataSplineComponent>(TEXT("MetadataSplineComponent"));
	SetRootComponent(MetadataSplineComponent);
}

UCustomSplineMetadata* AMetadataSplineActor::GetSplineMetadata() const 
{ 
	return SplineMetadata; 
}

float AMetadataSplineActor::GetTestFloatAtSplinePoint(int32 PointIndex)
{
	if (ensure(SplineMetadata))
	{
		if (ensure(SplineMetadata->PointParams.IsValidIndex(PointIndex)))
		{
			return SplineMetadata->PointParams[PointIndex].TestFloat;
		}
	}

	return 0.0f;
}

