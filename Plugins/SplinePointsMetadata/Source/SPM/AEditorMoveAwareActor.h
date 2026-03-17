// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AEditorMoveAwareActor.generated.h"

UCLASS()
class SPM_API AEditorMoveAwareActor : public AActor
{
	GENERATED_BODY()

public:
#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
#endif

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Editor")
	void OnMoveFinishedInEditor();

	virtual void OnMoveFinishedInEditor_Implementation();
};