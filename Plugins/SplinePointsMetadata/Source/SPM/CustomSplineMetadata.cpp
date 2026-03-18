// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSplineMetadata.h"

void UCustomSplineMetadata::InsertPoint(int32 Index, float t, bool bClosedLoop)
{
	if (Index >= PointParams.ZoneLayers.Num())
	{
		AddPoint(static_cast<float>(Index));
	}
	else
	{
		PointParams.ZoneLayers.Insert(FZoneLayer{}, Index);
	}

	Modify();
}

void UCustomSplineMetadata::UpdatePoint(int32 Index, float t, bool bClosedLoop)
{
	Modify();
}

void UCustomSplineMetadata::AddPoint(float InputKey)
{
	PointParams.ZoneLayers.Add(FZoneLayer{});
	Modify();
}

void UCustomSplineMetadata::RemovePoint(int32 Index)
{
	PointParams.ZoneLayers.RemoveAt(Index);
	Modify();
}


void UCustomSplineMetadata::DuplicatePoint(int32 Index)
{
	FZoneLayer NewVal = PointParams.ZoneLayers[Index];
	PointParams.ZoneLayers.Insert(NewVal, Index);
	Modify();
}


void UCustomSplineMetadata::CopyPoint(const USplineMetadata* FromSplineMetadata, int32 FromIndex, int32 ToIndex)
{
	if (const UCustomSplineMetadata* FromMetadata = Cast<UCustomSplineMetadata>(FromSplineMetadata))
	{
		PointParams.ZoneLayers[ToIndex] = PointParams.ZoneLayers[FromIndex];
		Modify();
	}
}

void UCustomSplineMetadata::Reset(int32 NumPoints)
{
	PointParams.ZoneLayers.Reset(NumPoints);
	Modify();
}

void UCustomSplineMetadata::Fixup(int32 NumPoints, USplineComponent* SplineComp)
{
	if (PointParams.ZoneLayers.Num() > NumPoints)
	{
		PointParams.ZoneLayers.RemoveAt(NumPoints, PointParams.ZoneLayers.Num() - NumPoints);
		Modify();
	}

	while (PointParams.ZoneLayers.Num() < NumPoints)
	{
		PointParams.ZoneLayers.Add(FZoneLayer{});
		Modify();
	}
}

