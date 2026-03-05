// Fill out your copyright notice in the Description page of Project Settings.

#include "MetadataSplineActor.h"

#include "CustomSplineMetadata.h"

AMetadataSplineActor::AMetadataSplineActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MySplineMetadata = CreateDefaultSubobject<UCustomSplineMetadata>(TEXT("MySplineMetadata"));
	MySplineMetadata->Reset(2);
	MySplineMetadata->AddPoint(0.0f);
	MySplineMetadata->AddPoint(1.0f);

	MySplineComponent = CreateDefaultSubobject<UMetadataSplineComponent>(TEXT("MySplineComponent"));
	SetRootComponent(MySplineComponent);
}

UCustomSplineMetadata* AMetadataSplineActor::GetSplineMetadata() const 
{ 
	return MySplineMetadata; 
}

float AMetadataSplineActor::GetTestFloatAtSplinePoint(int32 PointIndex)
{
	if (ensure(MySplineMetadata))
	{
		if (ensure(MySplineMetadata->PointParams.IsValidIndex(PointIndex)))
		{
			return MySplineMetadata->PointParams[PointIndex].TestFloat;
		}
	}

	return 0.0f;
}

