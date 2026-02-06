// LKEMockEventBus.cpp
// STANDALONE-PACKAGE: Mock-EventBus Implementation
//
// Copyright LKE_Draft1. All Rights Reserved.

#include "LKEMockEventBus.h"

// ============================================================================
// STATIC INSTANCE
// ============================================================================

ULKEMockEventBus* ULKEMockEventBus::Instance = nullptr;

// ============================================================================
// CONSOLE COMMANDS
// ============================================================================

/**
 * Simuliert Memory Pressure (0-4)
 * Usage: mock.memory 3
 */
static FAutoConsoleCommand CmdMockMemory(
	TEXT("mock.memory"),
	TEXT("Simuliert Memory Pressure Level (0=None, 1=Low, 2=Moderate, 3=High, 4=Critical)"),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		if (Args.Num() > 0)
		{
			int32 Level = FCString::Atoi(*Args[0]);
			ULKEMockEventBus::Get()->SimulateMemoryPressure(Level);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Usage: mock.memory <0-4>"));
		}
	})
);

/**
 * Simuliert SubState-Wechsel
 * Usage: mock.substate FlightMode
 */
static FAutoConsoleCommand CmdMockSubState(
	TEXT("mock.substate"),
	TEXT("Simuliert SubState-Wechsel (StoryMode, FlightMode, CombatMode, Transition)"),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		if (Args.Num() > 0)
		{
			ULKEMockEventBus::Get()->SimulateSubStateChange(Args[0]);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Usage: mock.substate <StoryMode|FlightMode|CombatMode|Transition>"));
		}
	})
);

/**
 * Zeigt aktive Subscriptions
 * Usage: mock.subs
 */
static FAutoConsoleCommand CmdMockSubs(
	TEXT("mock.subs"),
	TEXT("Zeigt alle aktiven Event-Subscriptions"),
	FConsoleCommandDelegate::CreateLambda([]()
	{
		ULKEMockEventBus::Get()->PrintSubscriptions();
	})
);

/**
 * Publisht ein beliebiges Event
 * Usage: mock.publish Flight_Activated 1.0
 */
static FAutoConsoleCommand CmdMockPublish(
	TEXT("mock.publish"),
	TEXT("Publisht ein Event (mock.publish EventName [FloatPayload])"),
	FConsoleCommandWithArgsDelegate::CreateLambda([](const TArray<FString>& Args)
	{
		if (Args.Num() > 0)
		{
			FName EventName = FName(*Args[0]);
			float Payload = Args.Num() > 1 ? FCString::Atof(*Args[1]) : 0.0f;
			ULKEMockEventBus::Get()->Publish(EventName, Payload);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Usage: mock.publish <EventName> [FloatPayload]"));
		}
	})
);
