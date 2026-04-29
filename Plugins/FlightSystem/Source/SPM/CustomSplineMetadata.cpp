// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomSplineMetadata.h"

void UCustomSplineMetadata::ForwardToOwner(ESplineForwardedEvent EventType, FSplinePayload Payload)
{
	AActor* Owner = GetTypedOuter<AActor>();

	if (!Owner)
	{
		return;
	}

	if (Owner->GetClass()->ImplementsInterface(USplineForward::StaticClass()))
	{
		const FString EventNameText = UEnum::GetDisplayValueAsText(EventType).ToString();
		//UE_LOG(LogTemp, Warning, TEXT("trying to forward %s"), *EventNameText);

#if WITH_EDITOR
		FEditorScriptExecutionGuard ScriptGuard;
#endif
		ISplineForward::Execute_OnSplineEventForwarded(Owner, EventType, Payload);
	}
}

void UCustomSplineMetadata::InsertPoint(int32 Index, float t, bool bClosedLoop)
{
	if (Index >= PointParams.ZoneLayers.Num())
	{
		AddPoint(static_cast<float>(Index));
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("adding %d"), Index);
		//PointParams.ZoneLayers.Insert(FZoneLayer{}, Index);
		
		FSplinePayload Payload;
		Payload.Index = Index;
		Payload.t = t;
		Payload.bClosedLoop = bClosedLoop;
		ForwardToOwner(ESplineForwardedEvent::InsertPoint, Payload);
	}

	Modify();
}

void UCustomSplineMetadata::UpdatePoint(int32 Index, float t, bool bClosedLoop)
{
	Modify();
	
	FSplinePayload Payload;
	Payload.Index = Index;
	Payload.t = t;
	Payload.bClosedLoop = bClosedLoop;
	ForwardToOwner(ESplineForwardedEvent::UpdatePoint, Payload);
}

void UCustomSplineMetadata::AddPoint(float InputKey)
{
	//UE_LOG(LogTemp, Warning, TEXT("adding %f"), InputKey);
	//PointParams.ZoneLayers.Add(FZoneLayer{});
	Modify();
	
	FSplinePayload Payload;
	Payload.InputKey = InputKey;
	ForwardToOwner(ESplineForwardedEvent::AddPoint, Payload);
}

void UCustomSplineMetadata::RemovePoint(int32 Index)
{
	//UE_LOG(LogTemp, Warning, TEXT("removing %d/%d"), Index, PointParams.ZoneLayers.Num() - 1);
	//PointParams.ZoneLayers.RemoveAt(Index);
	Modify();
	
	FSplinePayload Payload;
	Payload.Index = Index;
	ForwardToOwner(ESplineForwardedEvent::RemovePoint, Payload);
}


void UCustomSplineMetadata::DuplicatePoint(int32 Index)
{
	//FZoneLayer NewVal = PointParams.ZoneLayers[Index];
	//PointParams.ZoneLayers.Insert(NewVal, Index);
	Modify();
	
	FSplinePayload Payload;
	Payload.Index = Index;
	ForwardToOwner(ESplineForwardedEvent::DuplicatePoint, Payload);
}


void UCustomSplineMetadata::CopyPoint(const USplineMetadata* FromSplineMetadata, int32 FromIndex, int32 ToIndex)
{
	if (const UCustomSplineMetadata* FromMetadata = Cast<UCustomSplineMetadata>(FromSplineMetadata))
	{
		//PointParams.ZoneLayers[ToIndex] = PointParams.ZoneLayers[FromIndex];
		Modify();
		
		FSplinePayload Payload;
		Payload.SplineMetadata = FromSplineMetadata;
		Payload.FromIndex = FromIndex;
		Payload.Index = ToIndex;
		
		ForwardToOwner(ESplineForwardedEvent::CopyPoint, Payload);
	}
}

void UCustomSplineMetadata::Reset(int32 NumPoints)
{
	//PointParams.ZoneLayers.Reset(NumPoints);
	Modify();
	
	FSplinePayload Payload;
	Payload.NumPoints = NumPoints;
	ForwardToOwner(ESplineForwardedEvent::Reset, Payload);
}

void UCustomSplineMetadata::Fixup(int32 NumPoints, USplineComponent* SplineComp)
{
	/*
	UE_LOG(LogTemp, Warning, TEXT("fixup %d to %d"), PointParams.ZoneLayers.Num(), NumPoints);
	
	FSplinePayload Payload;
	Payload.NumPoints = NumPoints;
	Payload.SplineComp = SplineComp;
	
	if (PointParams.ZoneLayers.Num() > NumPoints)
	{
		PointParams.ZoneLayers.RemoveAt(NumPoints, PointParams.ZoneLayers.Num() - NumPoints);
		Modify();
		
		//ForwardToOwner(ESplineForwardedEvent::FixupRemove, Payload);
	}

	while (PointParams.ZoneLayers.Num() < NumPoints)
	{
		PointParams.ZoneLayers.Add(FZoneLayer{});
		Modify();
		
		
		//ForwardToOwner(ESplineForwardedEvent::FixupAdd, Payload);
	}
	*/
}

