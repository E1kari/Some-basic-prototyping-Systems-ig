// LKEMockEventBus.h
// STANDALONE-PACKAGE: Mock-EventBus fuer lokale Entwicklung ohne Hauptprojekt
//
// USAGE:
// 1. Diese Datei in dein Standalone-Projekt kopieren (Source/Public/)
// 2. In deinem FlightPawn: #include "LKEMockEventBus.h"
// 3. Events publishen mit: MockEventBus->Publish(LKEMockEvents::Flight_Activated);
// 4. Bei Integration: Diesen Mock durch den echten LKEEventBus ersetzen
//
// Copyright LKE_Draft1. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LKEMockEventBus.generated.h"

// ============================================================================
// EVENT NAMEN (aus LKEEventTypes.h - EXAKTE KOPIE!)
// ============================================================================

/**
 * Event-Namen Namespace - MUSS mit Hauptprojekt uebereinstimmen!
 *
 * WICHTIG: Kein "On" Prefix! Format: [Category]_[Action][Target]
 */
namespace LKEMockEvents
{
	// === FLIGHT EVENTS (dein Modul sendet diese) ===
	const FName Flight_Activated = FName(TEXT("Flight_Activated"));
	const FName Flight_Deactivated = FName(TEXT("Flight_Deactivated"));
	const FName Flight_ModeChanged = FName(TEXT("Flight_ModeChanged"));
	const FName Flight_BoostActivated = FName(TEXT("Flight_BoostActivated"));
	const FName Flight_BoostEnded = FName(TEXT("Flight_BoostEnded"));
	const FName Flight_StallWarning = FName(TEXT("Flight_StallWarning"));
	const FName Flight_TelemetryUpdate = FName(TEXT("Flight_TelemetryUpdate"));
	const FName Flight_PhysicsQualityChanged = FName(TEXT("Flight_PhysicsQualityChanged"));
	const FName Flight_EnteredZone = FName(TEXT("Flight_EnteredZone"));
	const FName Flight_ExitedZone = FName(TEXT("Flight_ExitedZone"));

	// === SYSTEM EVENTS (dein Modul empfaengt diese) ===
	const FName Memory_PressureChanged = FName(TEXT("Memory_PressureChanged"));
	const FName State_SubStateChanged = FName(TEXT("State_SubStateChanged"));
	const FName State_MainStateChanged = FName(TEXT("State_MainStateChanged"));
}

// ============================================================================
// MOCK EVENT DATA (vereinfachte Version von FLKEEventData)
// ============================================================================

/**
 * Vereinfachte Event-Daten fuer Standalone-Testing
 */
USTRUCT(BlueprintType)
struct FLKEMockEventData
{
	GENERATED_BODY()

	/** Event Name */
	UPROPERTY(BlueprintReadWrite, Category = "Event")
	FName EventName;

	/** Float Payload (z.B. Speed, Pressure Level) */
	UPROPERTY(BlueprintReadWrite, Category = "Event")
	float FloatPayload = 0.0f;

	/** Int Payload (z.B. Mode Index, Level) */
	UPROPERTY(BlueprintReadWrite, Category = "Event")
	int32 IntPayload = 0;

	/** String Payload (z.B. Zone Name) */
	UPROPERTY(BlueprintReadWrite, Category = "Event")
	FString StringPayload;

	/** Timestamp */
	UPROPERTY(BlueprintReadWrite, Category = "Event")
	float Timestamp = 0.0f;

	FLKEMockEventData()
		: EventName(NAME_None)
		, FloatPayload(0.0f)
		, IntPayload(0)
		, StringPayload(TEXT(""))
		, Timestamp(0.0f)
	{
	}

	FLKEMockEventData(FName InEventName)
		: EventName(InEventName)
		, FloatPayload(0.0f)
		, IntPayload(0)
		, StringPayload(TEXT(""))
		, Timestamp(0.0f)
	{
	}
};

// ============================================================================
// DELEGATE DECLARATIONS
// ============================================================================

/** Delegate fuer Event-Subscriptions */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLKEMockEventDelegate, const FLKEMockEventData&, EventData);

/** Delegate fuer spezifische Event-Subscriptions (gefiltert nach EventName) */
DECLARE_DYNAMIC_DELEGATE_OneParam(FLKEMockEventCallback, const FLKEMockEventData&, EventData);

// ============================================================================
// MOCK EVENT BUS
// ============================================================================

/**
 * ULKEMockEventBus - Standalone EventBus fuer lokale Entwicklung
 *
 * Simuliert den echten LKEEventBus ohne Abhaengigkeiten zum Hauptprojekt.
 *
 * FEATURES:
 * - Publish Events (mit Logging)
 * - Subscribe zu allen Events
 * - Subscribe zu spezifischen Events (gefiltert)
 * - Simuliere eingehende Events (fuer Testing)
 *
 * BEI INTEGRATION:
 * - Ersetze ULKEMockEventBus durch ULKEEventBus
 * - Ersetze FLKEMockEventData durch FLKEEventData
 * - Ersetze LKEMockEvents:: durch LKEEvents::
 */
UCLASS(BlueprintType, Blueprintable)
class ULKEMockEventBus : public UObject
{
	GENERATED_BODY()

public:
	// ============================================================================
	// SINGLETON ACCESS (fuer einfache Nutzung)
	// ============================================================================

	/**
	 * Gibt die globale MockEventBus Instanz zurueck
	 * Erstellt sie bei Bedarf
	 */
	UFUNCTION(BlueprintCallable, Category = "MockEventBus")
	static ULKEMockEventBus* Get()
	{
		if (!Instance)
		{
			Instance = NewObject<ULKEMockEventBus>();
			Instance->AddToRoot(); // Prevent GC
			UE_LOG(LogTemp, Log, TEXT("[MockEventBus] Instance created"));
		}
		return Instance;
	}

	// ============================================================================
	// PUBLISH (Events senden)
	// ============================================================================

	/**
	 * Publisht ein Event (einfache Version)
	 *
	 * @param EventName - Name des Events (z.B. LKEMockEvents::Flight_Activated)
	 * @param FloatPayload - Optionaler Float-Wert
	 */
	UFUNCTION(BlueprintCallable, Category = "MockEventBus")
	void Publish(FName EventName, float FloatPayload = 0.0f)
	{
		FLKEMockEventData EventData(EventName);
		EventData.FloatPayload = FloatPayload;
		EventData.Timestamp = FPlatformTime::Seconds();

		PublishEventData(EventData);
	}

	/**
	 * Publisht ein Event (vollstaendige Version)
	 *
	 * @param EventData - Vollstaendige Event-Daten
	 */
	UFUNCTION(BlueprintCallable, Category = "MockEventBus")
	void PublishEventData(const FLKEMockEventData& EventData)
	{
		// Logging (sehr hilfreich beim Debugging!)
		UE_LOG(LogTemp, Log, TEXT("[MockEventBus] PUBLISH: %s (Float: %.2f, Int: %d)"),
			*EventData.EventName.ToString(),
			EventData.FloatPayload,
			EventData.IntPayload);

		// An alle Subscriber broadcasten
		OnEventReceived.Broadcast(EventData);

		// An spezifische Subscriber (gefiltert nach EventName)
		if (FLKEMockEventCallback* Callback = SpecificSubscriptions.Find(EventData.EventName))
		{
			Callback->ExecuteIfBound(EventData);
		}
	}

	// ============================================================================
	// SUBSCRIBE (Events empfangen)
	// ============================================================================

	/**
	 * Delegate fuer ALLE Events
	 * Binde hieran um jeden Event zu empfangen
	 */
	UPROPERTY(BlueprintAssignable, Category = "MockEventBus")
	FLKEMockEventDelegate OnEventReceived;

	/**
	 * Subscribed zu einem spezifischen Event
	 *
	 * @param EventName - Name des Events
	 * @param Callback - Callback-Funktion
	 */
	UFUNCTION(BlueprintCallable, Category = "MockEventBus")
	void SubscribeTo(FName EventName, FLKEMockEventCallback Callback)
	{
		SpecificSubscriptions.Add(EventName, Callback);
		UE_LOG(LogTemp, Log, TEXT("[MockEventBus] SUBSCRIBE: %s"), *EventName.ToString());
	}

	/**
	 * Unsubscribed von einem spezifischen Event
	 *
	 * @param EventName - Name des Events
	 */
	UFUNCTION(BlueprintCallable, Category = "MockEventBus")
	void UnsubscribeFrom(FName EventName)
	{
		SpecificSubscriptions.Remove(EventName);
		UE_LOG(LogTemp, Log, TEXT("[MockEventBus] UNSUBSCRIBE: %s"), *EventName.ToString());
	}

	// ============================================================================
	// SIMULATION (fuer Testing - simuliere eingehende Events)
	// ============================================================================

	/**
	 * Simuliert ein Memory Pressure Event
	 * Nutze dies um deine Reaktion auf Memory-Druck zu testen!
	 *
	 * @param Level - Memory Pressure Level (0=None, 1=Low, 2=Moderate, 3=High, 4=Critical)
	 */
	UFUNCTION(BlueprintCallable, Category = "MockEventBus|Simulation")
	void SimulateMemoryPressure(int32 Level)
	{
		FLKEMockEventData EventData(LKEMockEvents::Memory_PressureChanged);
		EventData.IntPayload = FMath::Clamp(Level, 0, 4);
		EventData.FloatPayload = Level * 0.25f; // 0=0%, 1=25%, 2=50%, 3=75%, 4=100%

		UE_LOG(LogTemp, Warning, TEXT("[MockEventBus] SIMULATING Memory Pressure: Level %d"), Level);
		PublishEventData(EventData);
	}

	/**
	 * Simuliert einen SubState-Wechsel
	 * Nutze dies um Aktivierung/Deaktivierung zu testen!
	 *
	 * @param NewSubState - SubState als String ("StoryMode", "FlightMode", "CombatMode")
	 */
	UFUNCTION(BlueprintCallable, Category = "MockEventBus|Simulation")
	void SimulateSubStateChange(const FString& NewSubState)
	{
		FLKEMockEventData EventData(LKEMockEvents::State_SubStateChanged);
		EventData.StringPayload = NewSubState;

		UE_LOG(LogTemp, Warning, TEXT("[MockEventBus] SIMULATING SubState Change: %s"), *NewSubState);
		PublishEventData(EventData);
	}

	// ============================================================================
	// DEBUG
	// ============================================================================

	/**
	 * Zeigt alle aktiven Subscriptions
	 */
	UFUNCTION(BlueprintCallable, Category = "MockEventBus|Debug")
	void PrintSubscriptions()
	{
		UE_LOG(LogTemp, Log, TEXT("[MockEventBus] === ACTIVE SUBSCRIPTIONS ==="));
		for (const auto& Pair : SpecificSubscriptions)
		{
			UE_LOG(LogTemp, Log, TEXT("  - %s"), *Pair.Key.ToString());
		}
		UE_LOG(LogTemp, Log, TEXT("[MockEventBus] === END ==="));
	}

private:
	/** Singleton Instance */
	static ULKEMockEventBus* Instance;

	/** Spezifische Subscriptions (EventName -> Callback) */
	TMap<FName, FLKEMockEventCallback> SpecificSubscriptions;
};

// ============================================================================
// CONSOLE COMMANDS (fuer einfaches Testing)
// ============================================================================

/**
 * Console Commands fuer MockEventBus Testing
 *
 * Verfuegbar in der UE5 Console (~):
 *   mock.memory 0-4     - Simuliert Memory Pressure
 *   mock.substate X     - Simuliert SubState-Wechsel
 *   mock.subs           - Zeigt aktive Subscriptions
 */

// Hinweis: Console Commands muessen in einer .cpp Datei definiert werden
// Siehe LKEMockEventBus.cpp fuer Implementation
