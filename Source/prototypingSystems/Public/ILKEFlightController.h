// Copyright LKE_Draft1. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LKEFlightTypes.h"
#include "ILKEFlightController.generated.h"

/**
 * ILKEFlightController - Interface fuer FlightPawn Control
 *
 * Ermoeglicht C++ und Blueprint Zugriff auf FlightPawn Funktionalitaet.
 * Wird vom TransitionManager genutzt um Flight-Mode zu aktivieren/deaktivieren.
 *
 * Pattern: BlueprintNativeEvent (C++ Default + BP Override moeglich)
 * Konformitaet: V10.4
 *
 * Implementiert von:
 * - ALKEFlightPawn (C++ Klasse)
 * - BP_LKE_FlightPawn (Blueprint)
 *
 * Genutzt von:
 * - TransitionManager (Activation/Deactivation)
 * - UniversalPlayerPawn (Mode Queries)
 * - UI Systeme (Telemetry Display)
 *
 * Version: 1.0 (Week 3 - Flight System Foundation)
 */

// UInterface Klasse - muss vor UINTERFACE Macro
UINTERFACE(MinimalAPI, Blueprintable)
class ULKEFlightController : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface Definition
 */
class PROTOTYPINGSYSTEMS_API ILKEFlightController
{
	GENERATED_BODY()

public:
	// ============================================================================
	// ACTIVATION / DEACTIVATION
	// ============================================================================

	/**
	 * Aktiviert den FlightPawn nach Transition
	 *
	 * Aufgerufen vom TransitionManager wenn Story->Flight Transition abgeschlossen.
	 * FlightPawn wird sichtbar, Input wird aktiviert.
	 *
	 * @param Request - Activation Request mit Spawn-Location, Initial Speed, etc.
	 * @return true wenn Aktivierung erfolgreich
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Lifecycle")
	bool ActivateFlight(const FLKEFlightActivationRequest& Request);

	/**
	 * Deaktiviert den FlightPawn vor Transition
	 *
	 * Aufgerufen vom TransitionManager wenn Flight->Story Transition beginnt.
	 * FlightPawn wird unsichtbar, Input wird deaktiviert.
	 *
	 * @return true wenn Deaktivierung erfolgreich
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Lifecycle")
	bool DeactivateFlight();

	/**
	 * Prueft ob FlightPawn aktuell aktiv ist
	 *
	 * @return true wenn Flight-Mode aktiv
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Lifecycle")
	bool IsFlightActive() const;

	// ============================================================================
	// FLIGHT MODE
	// ============================================================================

	/**
	 * Gibt aktuellen Flight Mode zurueck
	 *
	 * @return Aktueller Flight Mode (Free, Rail, Thermal, Landing, Crashed)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Mode")
	ELKEFlightMode GetFlightMode() const;

	/**
	 * Setzt Flight Mode
	 *
	 * Wechselt zwischen Free, Rail, Thermal Modi.
	 * Broadcastet Flight_ModeChanged Event.
	 *
	 * @param NewMode - Neuer Flight Mode
	 * @return true wenn Mode-Wechsel erfolgreich
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Mode")
	bool SetFlightMode(ELKEFlightMode NewMode);

	// ============================================================================
	// TELEMETRY
	// ============================================================================

	/**
	 * Gibt aktuelle Telemetry-Daten zurueck
	 *
	 * Echtzeit-Flugdaten fuer UI/Audio/Kamera:
	 * - Speed, Altitude, Heading
	 * - Pitch, Roll, Yaw
	 * - GForce, Lift, Drag
	 * - Warnings (Stall, LowAltitude, Overheat)
	 *
	 * @return Aktuelle FLKEFlightTelemetry Struktur
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Telemetry")
	FLKEFlightTelemetry GetTelemetry() const;

	// ============================================================================
	// BOOST CONTROL
	// ============================================================================

	/**
	 * Aktiviert Boost wenn verfuegbar
	 *
	 * Boost erhoeht Speed fuer BoostDuration Sekunden.
	 * Broadcastet Flight_BoostActivated Event.
	 *
	 * @return true wenn Boost aktiviert wurde (false wenn Cooldown)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Boost")
	bool ActivateBoost();

	/**
	 * Gibt Boost State zurueck
	 *
	 * @return Aktueller Boost State (Ready, Active, Cooldown)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Boost")
	ELKEBoostState GetBoostState() const;

	// ============================================================================
	// INPUT PROCESSING
	// ============================================================================

	/**
	 * Verarbeitet Flight Input
	 *
	 * Aufgerufen vom Input-System (Enhanced Input oder direkt).
	 * Input wird im naechsten Physics-Tick verarbeitet.
	 *
	 * @param Input - Aufbereitete Input-Daten (Throttle, Pitch, Roll, Yaw)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Input")
	void ProcessFlightInput(const FLKEFlightInput& Input);

	// ============================================================================
	// PHYSICS CONFIG
	// ============================================================================

	/**
	 * Gibt aktuelle Physics Config zurueck
	 *
	 * @return Aktuelle FLKEFlightPhysicsConfig
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Physics")
	FLKEFlightPhysicsConfig GetPhysicsConfig() const;

	/**
	 * Setzt Physics Config (fuer Ship-Varianten)
	 *
	 * Erlaubt verschiedene Schiffs-Typen mit unterschiedlicher Aerodynamik.
	 *
	 * @param NewConfig - Neue Physics Konfiguration
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Physics")
	void SetPhysicsConfig(const FLKEFlightPhysicsConfig& NewConfig);

	/**
	 * Gibt aktuelle Physics Quality zurueck
	 *
	 * Quality wird automatisch basierend auf Memory Pressure angepasst.
	 *
	 * @return Aktuelle Physics Quality (High, Medium, Low, Minimal)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Physics")
	ELKEPhysicsQuality GetPhysicsQuality() const;

	// ============================================================================
	// ZONE INTERACTION
	// ============================================================================

	/**
	 * Benachrichtigt FlightPawn dass eine Zone betreten wurde
	 *
	 * Aufgerufen von Wind/Thermal Zone Actors.
	 * Broadcastet Flight_EnteredZone Event.
	 *
	 * @param ZoneType - Typ der Zone (z.B. "Thermal", "WindTunnel")
	 * @param ZoneStrength - Staerke des Effekts (0.0 - 1.0)
	 * @param ZoneDirection - Richtung des Zone-Effekts (fuer Wind)
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Zones")
	void EnterZone(FName ZoneType, float ZoneStrength, FVector ZoneDirection);

	/**
	 * Benachrichtigt FlightPawn dass eine Zone verlassen wurde
	 *
	 * Broadcastet Flight_ExitedZone Event.
	 *
	 * @param ZoneType - Typ der verlassenen Zone
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Flight|Zones")
	void ExitZone(FName ZoneType);
};

/**
 * USAGE EXAMPLE:
 *
 * // Im TransitionManager:
 * if (FlightPawn && FlightPawn->GetClass()->ImplementsInterface(ULKEFlightController::StaticClass()))
 * {
 *     ILKEFlightController* FlightController = Cast<ILKEFlightController>(FlightPawn);
 *
 *     // Activation Request erstellen
 *     FLKEFlightActivationRequest Request;
 *     Request.SpawnLocation = TransitionSpawnPoint->GetActorLocation();
 *     Request.SpawnRotation = TransitionSpawnPoint->GetActorRotation();
 *     Request.InitialSpeed = 1500.0f;
 *     Request.InitialFlightMode = ELKEFlightMode::Free;
 *
 *     // FlightPawn aktivieren
 *     if (ILKEFlightController::Execute_ActivateFlight(FlightPawn, Request))
 *     {
 *         UE_LOG(LogLKEFlight, Log, TEXT("FlightPawn activated successfully"));
 *     }
 * }
 *
 * // In UI (Telemetry Display):
 * FLKEFlightTelemetry Telemetry = ILKEFlightController::Execute_GetTelemetry(FlightPawn);
 * SpeedText->SetText(FText::AsNumber(Telemetry.CurrentSpeed));
 * AltitudeText->SetText(FText::AsNumber(Telemetry.Altitude));
 *
 * // In Audio (Stall Warning):
 * if (Telemetry.bStallWarning)
 * {
 *     PlayStallWarningSound();
 * }
 */
