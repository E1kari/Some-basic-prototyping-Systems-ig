// Fill out your copyright notice in the Description page of Project Settings.
/*

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


template<class T>
bool UpdateMultipleValue(TOptional<T>& CurrentValue, T InValue)
{
	if (!CurrentValue.IsSet())
	{
		CurrentValue = InValue;
	}
	else if (CurrentValue.IsSet() && CurrentValue.GetValue() != InValue)
	{
		CurrentValue.Reset();
		return false;
	}

	return true;
}


void FSplineMetadataDetails::Update(USplineComponent* InSplineComponent, const TSet<int32>& InSelectedKeys)
{
	SplineComp = InSplineComponent;
	SelectedKeys = InSelectedKeys;
	TestValue.Reset();

	if (InSplineComponent)
	{
		bool bUpdateTestFloat = true;

		UCustomSplineMetadata* Metadata = Cast<UCustomSplineMetadata>(InSplineComponent->GetSplinePointsMetadata());
		if (Metadata)
		{
			for (int32 Index : InSelectedKeys)
			{
				if (Metadata->PointParams.IsValidIndex(Index))
				{
					if (bUpdateTestFloat)
					{
						bUpdateTestFloat = UpdateMultipleValue(TestValue, Metadata->PointParams[Index].TestData);
					}
				}
			}
		}
	}
}


void FSplineMetadataDetails::GenerateChildContent(IDetailGroup& DetailGroup)
{
	UCustomSplineMetadata* Metadata = GetMetadata();
	if (!Metadata)
		return;

	IDetailLayoutBuilder& Layout = DetailGroup.GetParentLayout();

	TSharedRef<IPropertyHandle> ParamsHandle =
		Layout.GetProperty(GET_MEMBER_NAME_CHECKED(UCustomSplineMetadata, PointParams));

	DetailGroup.AddPropertyRow(ParamsHandle);
}


void FSplineMetadataDetails::OnSetValues(FSplineMetadataDetails& Details)
{
	Details.SplineComp->GetSplinePointsMetadata()->Modify();
	Details.SplineComp->UpdateSpline();
	Details.SplineComp->bSplineHasBeenEdited = true;
	static FProperty* SplineCurvesProperty = FindFProperty<FProperty>(USplineComponent::StaticClass(), GET_MEMBER_NAME_CHECKED(USplineComponent, SplineCurves));
	FComponentVisualizer::NotifyPropertyModified(Details.SplineComp, SplineCurvesProperty);
	Details.Update(Details.SplineComp, Details.SelectedKeys);

	GEditor->RedrawLevelEditingViewports(true);
}

void FSplineMetadataDetails::OnSetTestFloat(FZoneLayer NewValue, ETextCommit::Type CommitInfo)
{
	if (UCustomSplineMetadata* Metadata = GetMetadata())
	{
		const FScopedTransaction Transaction(LOCTEXT("SetTestFloat", "Set spline point test float"));

		for (int32 Index : SelectedKeys)
		{
			Metadata->PointParams[Index].TestData = NewValue;
		}

		OnSetValues(*this);
	}
}

UCustomSplineMetadata* FSplineMetadataDetails::GetMetadata() const
{
	UCustomSplineMetadata* Metadata = SplineComp ? Cast<UCustomSplineMetadata>(SplineComp->GetSplinePointsMetadata()) : nullptr;
	return Metadata;
}

TOptional<FZoneLayer> FSplineMetadataDetails::GetTestFloat() const 
{ 
	return TestValue; 
}

#undef LOCTEXT_NAMESPACE
*/