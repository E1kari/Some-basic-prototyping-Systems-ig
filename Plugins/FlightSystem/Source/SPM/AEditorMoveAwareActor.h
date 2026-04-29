// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SplineForward.h"
#include "GameFramework/Actor.h"
#include "AEditorMoveAwareActor.generated.h"

UCLASS()
class SPM_API AEditorMoveAwareActor : public AActor, public ISplineForward
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
	virtual void OnSplineEventForwarded_Implementation(
		ESplineForwardedEvent EventType,
		const FSplinePayload& Payload) override;
#endif

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Editor")
	void OnMoveFinishedInEditor();

	UFUNCTION(BlueprintImplementableEvent, Category="Spline")
	void BP_OnSplineEventForwarded(
		ESplineForwardedEvent EventType,
		const FSplinePayload& Payload);
	
	virtual void OnMoveFinishedInEditor_Implementation();
};