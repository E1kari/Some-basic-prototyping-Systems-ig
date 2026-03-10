// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineMetadataDetails.h"

#include <IDetailGroup.h>
#include <DetailLayoutBuilder.h>
#include <DetailWidgetRow.h>
#include <Widgets/Input/SNumericEntryBox.h>
#include <ScopedTransaction.h>
#include <ComponentVisualizer.h>
#include <Editor.h>

#include "SPM/CustomSplineMetadata.h"

#define LOCTEXT_NAMESPACE "FSplineMetadataDetails"

TSharedPtr<ISplineMetadataDetails> UMySplineMetadataDetailsFactory::Create()
{
	return MakeShared<FSplineMetadataDetails>();
}

UClass* UMySplineMetadataDetailsFactory::GetMetadataClass() const
{
	return UCustomSplineMetadata::StaticClass();
}

FName FSplineMetadataDetails::GetName() const
{
	return FName(TEXT("MySplineMetadataDetails"));
}

FText FSplineMetadataDetails::GetDisplayName() const
{
	return LOCTEXT("MySplineMetadataDetails", "SPM");
}



#undef LOCTEXT_NAMESPACE
