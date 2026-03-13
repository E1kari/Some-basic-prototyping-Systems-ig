#include "SplineMetadataDetails.h"

#include "SPM/CustomSplineMetadata.h"

#include "ComponentVisualizer.h"
#include "DetailWidgetRow.h"
#include "Editor.h"
#include "IDetailGroup.h"
#include "ScopedTransaction.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

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
	return LOCTEXT("SplineMetadataDisplayName", "Zones");
}

void FSplineMetadataDetails::Update(USplineComponent* InSplineComponent, const TSet<int32>& InSelectedKeys)
{
	SplineComp = InSplineComponent;
	SelectedKeys = InSelectedKeys;

	ZoneOptions.Reset();

	for (EZoneName Val : TEnumRange<EZoneName>())
	{
		ZoneOptions.Add(MakeShared<EZoneName>(Val));
	}

	RebuildZoneArrayWidget();
}

UCustomSplineMetadata* FSplineMetadataDetails::GetMetadata() const
{
	if (SplineComp)
	{
		return Cast<UCustomSplineMetadata>(SplineComp->GetSplinePointsMetadata());
	}
	
	return nullptr;
}

FSplinePointParams* FSplineMetadataDetails::GetSelectedPointParams() const
{
	UCustomSplineMetadata* Metadata = GetMetadata();
	
	if (!IsValid(Metadata) || SelectedKeys.Num() != 1)
	{
		return nullptr;
	}

	const int32 SelectedIndex = *SelectedKeys.CreateConstIterator();

	return &Metadata->PointParams;
}

const FSplinePointParams* FSplineMetadataDetails::GetSelectedPointParamsConst() const
{
	const UCustomSplineMetadata* Metadata = GetMetadata();
	if (!Metadata || SelectedKeys.Num() != 1)
	{
		return nullptr;
	}

	const int32 SelectedIndex = *SelectedKeys.CreateConstIterator();
	return &Metadata->PointParams;
}

void FSplineMetadataDetails::GenerateChildContent(IDetailGroup& InGroup)
{
	InGroup.AddWidgetRow()
.WholeRowContent()
[
	SNew(STextBlock)
	.Text(FText::FromString(
		FString::Printf(TEXT("Metadata: %s | SelectedKeys: %d"),
			GetMetadata() ? TEXT("yes") : TEXT("no"),
			SelectedKeys.Num())))
];
	
	InGroup.AddWidgetRow()
	.NameContent()
	[
		SNew(STextBlock)
		.Text(LOCTEXT("ZoneLayersLabel", "Zone Layers"))
	]
	.ValueContent()
	.MinDesiredWidth(500.f)
	.MaxDesiredWidth(800.f)
	[
	SNew(SBox)
	.MinDesiredWidth(500.f)
	[
		SAssignNew(ZoneListBox, SVerticalBox)
	]
];

	RebuildZoneArrayWidget();

	InGroup.AddWidgetRow()
	.WholeRowContent()
	[
		SNew(SButton)
		.Text(LOCTEXT("AddZoneLayer", "+ Add Zone Layer"))
		.OnClicked_Lambda([this]()
		{
			OnAddZoneLayer();
			return FReply::Handled();
		})
	];
}

void FSplineMetadataDetails::RebuildZoneArrayWidget()
{
	UE_LOG(LogTemp, Warning, TEXT("desperation"));
	if (!ZoneListBox.IsValid())
	{
		return;
	}

	ZoneListBox->ClearChildren();

	const FSplinePointParams* Params = GetSelectedPointParamsConst();
	if (!Params)
	{
		ZoneListBox->AddSlot()
		.AutoHeight()
		.Padding(0.f, 5.f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("SelectSinglePoint", "Select exactly one spline point to edit zone layers."))
		];
		return;
	}

	for (int32 LayerIndex = 0; LayerIndex < Params->ZoneLayers.Num(); ++LayerIndex)
	{
		ZoneListBox->AddSlot()
		.AutoHeight()
		.Padding(0.f, 4.f)
		[
			SNew(SHorizontalBox)
			
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0.f, 5.0f, 0.f, 5.0f)
			[
				SNew(STextBlock)
				.Text(FText::Format(LOCTEXT("LayerLabelFmt", "Layer {0}"), FText::AsNumber(LayerIndex)))
			]
			
			+ SHorizontalBox::Slot()
			.FillWidth(0.45f)
			.Padding(0.f, 0.f, 0.f, 0.f)
			[
				SNew(SComboBox<TSharedPtr<EZoneName>>)
				.OptionsSource(&ZoneOptions)
				.OnGenerateWidget_Lambda([](TSharedPtr<EZoneName> Item)
				{
					return SNew(STextBlock)
						.Text(StaticEnum<EZoneName>()->GetDisplayNameTextByValue(static_cast<int64>(*Item)));
				})
				.OnSelectionChanged(this, &FSplineMetadataDetails::OnLayerZoneNameChanged, LayerIndex)
				[
					SNew(STextBlock)
					.Text(this, &FSplineMetadataDetails::GetCurrentZoneLabel, LayerIndex)
				]
			]
			
			+ SHorizontalBox::Slot()
			.FillWidth(0.35f)
			.Padding(0.f, 0.f, 8.f, 0.f)
			[
				SNew(SNumericEntryBox<float>)
				.Value(this, &FSplineMetadataDetails::GetLayerDistance, LayerIndex)
				.MinValue(0.f)
				.AllowSpin(true)
				.OnValueCommitted(this, &FSplineMetadataDetails::OnLayerDistanceCommitted, LayerIndex)
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SButton)
				.Text(LOCTEXT("RemoveLayer", "Remove"))
				.OnClicked_Lambda([this, LayerIndex]()
				{
					OnRemoveZoneLayer(LayerIndex);
					return FReply::Handled();
				})
			]
		];
	}
	
	if (ZoneListBox.IsValid())
	{
		ZoneListBox->Invalidate(EInvalidateWidgetReason::Layout);
	}
	
	ZoneListBox->Invalidate(EInvalidateWidgetReason::PaintAndVolatility);
}

void FSplineMetadataDetails::OnAddZoneLayer()
{
	FSplinePointParams* Params = GetSelectedPointParams();
	if (!Params)
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("AddZoneLayerTransaction", "Add spline zone layer"));

	if (UCustomSplineMetadata* Metadata = GetMetadata())
	{
		Metadata->Modify();
	}

	FZoneLayer& NewLayer = Params->ZoneLayers.AddDefaulted_GetRef();
	NewLayer.ZoneName = EZoneName::ZoneDefault;
	NewLayer.Distance = 10.f;

	NotifyChanged();
	RebuildZoneArrayWidget();
}

void FSplineMetadataDetails::OnRemoveZoneLayer(int32 LayerIndex)
{
	FSplinePointParams* Params = GetSelectedPointParams();
	if (!Params || !Params->ZoneLayers.IsValidIndex(LayerIndex))
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("RemoveZoneLayerTransaction", "Remove spline zone layer"));

	if (UCustomSplineMetadata* Metadata = GetMetadata())
	{
		Metadata->Modify();
	}

	Params->ZoneLayers.RemoveAt(LayerIndex);

	NotifyChanged();
	RebuildZoneArrayWidget();
}

TOptional<float> FSplineMetadataDetails::GetLayerDistance(int32 LayerIndex) const
{
	const FSplinePointParams* Params = GetSelectedPointParamsConst();
	if (!Params || !Params->ZoneLayers.IsValidIndex(LayerIndex))
	{
		return TOptional<float>();
	}

	return Params->ZoneLayers[LayerIndex].Distance;
}

void FSplineMetadataDetails::OnLayerDistanceCommitted(float NewValue, ETextCommit::Type CommitType, int32 LayerIndex)
{
	FSplinePointParams* Params = GetSelectedPointParams();
	if (!Params || !Params->ZoneLayers.IsValidIndex(LayerIndex))
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("SetZoneLayerDistanceTransaction", "Set spline zone layer distance"));

	if (UCustomSplineMetadata* Metadata = GetMetadata())
	{
		Metadata->Modify();
	}

	Params->ZoneLayers[LayerIndex].Distance = NewValue;

	NotifyChanged();
}

void FSplineMetadataDetails::OnLayerZoneNameChanged(TSharedPtr<EZoneName> NewSelection, ESelectInfo::Type SelectInfo, int32 LayerIndex)
{
	if (!NewSelection.IsValid())
	{
		return;
	}

	FSplinePointParams* Params = GetSelectedPointParams();
	if (!Params || !Params->ZoneLayers.IsValidIndex(LayerIndex))
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("SetZoneLayerNameTransaction", "Set spline zone layer name"));

	if (UCustomSplineMetadata* Metadata = GetMetadata())
	{
		Metadata->Modify();
	}

	Params->ZoneLayers[LayerIndex].ZoneName = *NewSelection;

	NotifyChanged();
	RebuildZoneArrayWidget();
}

FText FSplineMetadataDetails::GetCurrentZoneLabel(int32 LayerIndex) const
{
	const FSplinePointParams* Params = GetSelectedPointParamsConst();
	if (!Params || !Params->ZoneLayers.IsValidIndex(LayerIndex))
	{
		return LOCTEXT("InvalidZone", "Invalid");
	}

	return StaticEnum<EZoneName>()->GetDisplayNameTextByValue(
		static_cast<int64>(Params->ZoneLayers[LayerIndex].ZoneName));
}

void FSplineMetadataDetails::NotifyChanged()
{
	UE_LOG(LogTemp, Warning, TEXT("make it stop"));
	if (!SplineComp)
	{
		return;
	}

	SplineComp->GetSplinePointsMetadata()->Modify();
	SplineComp->UpdateSpline();
	SplineComp->bSplineHasBeenEdited = true;

	static FProperty* SplineCurvesProperty =
		FindFProperty<FProperty>(USplineComponent::StaticClass(), GET_MEMBER_NAME_CHECKED(USplineComponent, SplineCurves));

	FComponentVisualizer::NotifyPropertyModified(SplineComp, SplineCurvesProperty);
	GEditor->RedrawLevelEditingViewports(true);
}

#undef LOCTEXT_NAMESPACE