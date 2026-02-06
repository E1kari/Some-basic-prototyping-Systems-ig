// Copyright LKE_Draft1. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LKEFlightTypes.generated.h"

/**
 * LKEFlightTypes - Flight System Type Definitions
 *
 * Enums, Structs und Telemetry-Daten fuer das FlightPawn System.
 *
 * Architektur:
 * - FlightPawn ist SEPARATER Pawn (nicht UniversalPlayerPawn!)
 * - Wird bei Story->Flight Transition gespawnt/aktiviert
 * - Nutzt Physics-basierte Bewegung (Lift, Drag, Thrust)
 * - Memory-Adaptive Physik-Qualitaet
 *
 * Version: 1.0 (Week 3 - Flight System Foundation)
 */

// ============================================================================
// FLIGHT MODE ENUM
// ============================================================================

/**
 * Flight Mode - Bestimmt Steuerungsverhalten
 *
 * Free:     100% Spieler-Kontrolle, normale Physik
 * Rail:     30% Kontrolle, Spline-gefuehrt (WindTunnel)
 * Thermal:  100% Kontrolle + zusaetzlicher Auftrieb
 * Landing:  Automatische Landesequenz (noch nicht impl.)
 * Crashed:  Post-Crash State, keine Kontrolle
 */
UENUM(BlueprintType)
enum class ELKEFlightMode : uint8
{
	Free		UMETA(DisplayName = "Free Flight"),
	Rail		UMETA(DisplayName = "Rail Mode (Wind Tunnel)"),
	Thermal		UMETA(DisplayName = "Thermal Updraft"),
	Landing		UMETA(DisplayName = "Landing Sequence"),
	Crashed		UMETA(DisplayName = "Crashed")
};

// ============================================================================
// PHYSICS QUALITY ENUM
// ============================================================================

/**
 * Physics Quality - Memory-Adaptive Qualitaetsstufen
 *
 * Wird automatisch basierend auf Memory Pressure angepasst:
 * - Level 0-1: High (4 Collision Rays, 60Hz)
 * - Level 2:   Medium (3 Rays, 60Hz)
 * - Level 3:   Low (1 Ray, 30Hz)
 * - Level 4:   Minimal (0 Rays, 20Hz)
 */
UENUM(BlueprintType)
enum class ELKEPhysicsQuality : uint8
{
	High		UMETA(DisplayName = "High (Full Physics)"),
	Medium		UMETA(DisplayName = "Medium (Reduced Rays)"),
	Low			UMETA(DisplayName = "Low (Minimal Physics)"),
	Minimal		UMETA(DisplayName = "Minimal (Emergency)")
};

// ============================================================================
// BOOST STATE ENUM
// ============================================================================

/**
 * Boost State - Fuer Boost-Mechanik
 *
 * Ready:     Boost verfuegbar
 * Active:    Boost laeuft (2x Speed, 3 Sekunden)
 * Cooldown:  Boost auf Cooldown (5 Sekunden)
 */
UENUM(BlueprintType)
enum class ELKEBoostState : uint8
{
	Ready		UMETA(DisplayName = "Ready"),
	Active		UMETA(DisplayName = "Boost Active"),
	Cooldown	UMETA(DisplayName = "Cooldown")
};

// ============================================================================
// FLIGHT TELEMETRY STRUCT
// ============================================================================

/**
 * Flight Telemetry - Echtzeit-Flugdaten
 *
 * Wird alle 30Hz via Flight_TelemetryUpdate Event broadcast.
 * Kann von UI, Audio, Kamera-System genutzt werden.
 */
USTRUCT(BlueprintType)
struct FLKEFlightTelemetry
{
	GENERATED_BODY()

	// === POSITION & BEWEGUNG ===

	/** Aktuelle Geschwindigkeit in Units/Sekunde */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Movement")
	float CurrentSpeed = 0.0f;

	/** Hoehe ueber Grund in Units */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Movement")
	float Altitude = 0.0f;

	/** Kompass-Richtung (0-360 Grad, 0 = Nord) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Movement")
	float Heading = 0.0f;

	/** Vertikale Geschwindigkeit (positiv = steigend) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Movement")
	float VerticalSpeed = 0.0f;

	// === ROTATION ===

	/** Pitch in Grad (-90 bis +90) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Rotation")
	float Pitch = 0.0f;

	/** Roll in Grad (-180 bis +180) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Rotation")
	float Roll = 0.0f;

	/** Yaw in Grad (0-360) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Rotation")
	float Yaw = 0.0f;

	// === PHYSIK ===

	/** G-Kraefte (1.0 = normal) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Physics")
	float GForce = 1.0f;

	/** Aktuelle Auftriebskraft */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Physics")
	float CurrentLift = 0.0f;

	/** Aktueller Luftwiderstand */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Physics")
	float CurrentDrag = 0.0f;

	/** Thermik-Auftrieb (nur im Thermal Mode) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Physics")
	float ThermalLift = 0.0f;

	// === WARNUNGEN ===

	/** Stall-Warnung aktiv (Speed < StallSpeed) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Warnings")
	bool bStallWarning = false;

	/** Low Altitude Warnung */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Warnings")
	bool bLowAltitudeWarning = false;

	/** Ueberhitzung (nach langem Boost) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Warnings")
	bool bOverheatWarning = false;

	// === STATUS ===

	/** Aktueller Flight Mode */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Status")
	ELKEFlightMode FlightMode = ELKEFlightMode::Free;

	/** Boost State */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Status")
	ELKEBoostState BoostState = ELKEBoostState::Ready;

	/** Verbleibende Boost-Zeit (wenn aktiv) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Status")
	float BoostTimeRemaining = 0.0f;

	/** Verbleibende Cooldown-Zeit */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Status")
	float CooldownTimeRemaining = 0.0f;

	// === TIMESTAMP ===

	/** Zeitstempel dieser Telemetry-Daten */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Meta")
	float Timestamp = 0.0f;

	/** Default Constructor */
	FLKEFlightTelemetry()
	{
	}
};

// ============================================================================
// FLIGHT PHYSICS CONFIG STRUCT
// ============================================================================

/**
 * Flight Physics Config - Konfigurierbare Physik-Parameter
 *
 * Kann pro Ship-Typ unterschiedlich sein.
 * Default-Werte basieren auf Legacy-Dokumentation.
 */
USTRUCT(BlueprintType)
struct FLKEFlightPhysicsConfig
{
	GENERATED_BODY()

	// === AERODYNAMIK ===

	/** Auftriebskoeffizient (hoeher = mehr Lift bei gleicher Speed) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Aerodynamics")
	float LiftCoefficient = 1.2f;

	/** Widerstandskoeffizient (hoeher = mehr Drag) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Aerodynamics")
	float DragCoefficient = 0.3f;

	/** Stall-Geschwindigkeit - unter dieser Speed kein Auftrieb */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Aerodynamics")
	float StallSpeed = 1200.0f;

	/** Minimaler Auftrieb bei Stall (verhindert sofortigen Absturz) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Aerodynamics")
	float MinLiftAtStall = 1000.0f;

	// === GROUND EFFECT ===

	/** Hoehe fuer Ground Effect in Units */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|GroundEffect")
	float GroundEffectHeight = 200.0f;

	/** Bonus-Lift durch Ground Effect (0.2 = +20%) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|GroundEffect")
	float GroundEffectBonus = 0.2f;

	// === THRUST ===

	/** Basis-Schub-Multiplikator */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Thrust")
	float ThrustMultiplier = 50000.0f;

	/** Maximale Geschwindigkeit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Thrust")
	float MaxSpeed = 5000.0f;

	/** Minimale Geschwindigkeit (negativ = rueckwaerts) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Thrust")
	float MinSpeed = -500.0f;

	// === BOOST ===

	/** Boost Speed-Multiplikator */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Boost")
	float BoostMultiplier = 2.0f;

	/** Boost-Dauer in Sekunden */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Boost")
	float BoostDuration = 3.0f;

	/** Boost-Cooldown in Sekunden */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Boost")
	float BoostCooldown = 5.0f;

	// === ROTATION ===

	/** Pitch-Rate in Grad/Sekunde */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Rotation")
	float PitchRate = 60.0f;

	/** Roll-Rate in Grad/Sekunde */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Rotation")
	float RollRate = 90.0f;

	/** Yaw-Rate in Grad/Sekunde */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Rotation")
	float YawRate = 45.0f;

	/** Maximaler Pitch-Winkel */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Rotation")
	float MaxPitch = 75.0f;

	// === COLLISION ===

	/** Anzahl Collision Rays bei High Quality */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Collision")
	int32 CollisionRayCountHigh = 4;

	/** Anzahl Collision Rays bei Medium Quality */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Collision")
	int32 CollisionRayCountMedium = 3;

	/** Anzahl Collision Rays bei Low Quality */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight|Collision")
	int32 CollisionRayCountLow = 1;

	/** Default Constructor */
	FLKEFlightPhysicsConfig()
	{
	}
};

// ============================================================================
// FLIGHT INPUT STRUCT
// ============================================================================

/**
 * Flight Input - Aufbereitete Input-Daten
 *
 * Wird vom Input-System gefuellt und von Physics verarbeitet.
 */
USTRUCT(BlueprintType)
struct FLKEFlightInput
{
	GENERATED_BODY()

	/** Throttle (-1 bis +1, negativ = bremsen) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Input")
	float Throttle = 0.0f;

	/** Pitch Input (-1 bis +1) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Input")
	float PitchInput = 0.0f;

	/** Roll Input (-1 bis +1) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Input")
	float RollInput = 0.0f;

	/** Yaw Input (-1 bis +1) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Input")
	float YawInput = 0.0f;

	/** Boost angefordert */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Input")
	bool bBoostRequested = false;

	/** Bremse angefordert */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Input")
	bool bBrakeRequested = false;

	/** Default Constructor */
	FLKEFlightInput()
	{
	}

	/** Reset alle Inputs auf neutral */
	void Reset()
	{
		Throttle = 0.0f;
		PitchInput = 0.0f;
		RollInput = 0.0f;
		YawInput = 0.0f;
		bBoostRequested = false;
		bBrakeRequested = false;
	}
};

// ============================================================================
// FLIGHT ACTIVATION REQUEST STRUCT
// ============================================================================

/**
 * Flight Activation Request - Fuer Transition-System
 *
 * Wird vom TransitionManager verwendet um FlightPawn zu aktivieren.
 */
USTRUCT(BlueprintType)
struct FLKEFlightActivationRequest
{
	GENERATED_BODY()

	/** Spawn-Location fuer FlightPawn */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Activation")
	FVector SpawnLocation = FVector::ZeroVector;

	/** Spawn-Rotation */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Activation")
	FRotator SpawnRotation = FRotator::ZeroRotator;

	/** Initiale Geschwindigkeit */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Activation")
	float InitialSpeed = 1500.0f;

	/** Initialer Flight Mode */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Activation")
	ELKEFlightMode InitialFlightMode = ELKEFlightMode::Free;

	/** Physics Config (optional, nullptr = Default) */
	UPROPERTY(BlueprintReadWrite, Category = "Flight|Activation")
	FLKEFlightPhysicsConfig PhysicsConfig;

	/** Request Timestamp */
	UPROPERTY(BlueprintReadOnly, Category = "Flight|Activation")
	float RequestTimestamp = 0.0f;

	/** Default Constructor */
	FLKEFlightActivationRequest()
	{
	}
};
