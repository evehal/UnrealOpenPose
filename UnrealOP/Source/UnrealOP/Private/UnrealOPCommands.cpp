// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UnrealOPCommands.h"


#define LOCTEXT_NAMESPACE "FUnrealOPModule"

void FUnrealOPCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "UnrealOP", "Execute UnrealOP action", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
