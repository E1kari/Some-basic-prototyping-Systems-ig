#include "AEditorMoveAwareActor.h"
#include "Engine/Engine.h"
#include "UObject/Script.h" // for FEditorScriptExecutionGuard

void AEditorMoveAwareActor::OnMoveFinishedInEditor_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Native OnMoveFinishedInEditor_Implementation fired"));
}

#if WITH_EDITOR
void AEditorMoveAwareActor::PostEditMove(bool bFinished)
{
	Super::PostEditMove(bFinished);

	if (bFinished)
	{
		UE_LOG(LogTemp, Warning, TEXT("Calling OnMoveFinishedInEditor"));

		FEditorScriptExecutionGuard ScriptGuard;
		OnMoveFinishedInEditor();
	}
}
#endif