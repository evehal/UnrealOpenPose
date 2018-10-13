// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "UnrealOp.h"
#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "UnrealOPStyle.h"

class FUnrealOPCommands : public TCommands<FUnrealOPCommands>
{
public:

	FUnrealOPCommands()
		: TCommands<FUnrealOPCommands>(TEXT("UnrealOP"), NSLOCTEXT("Contexts", "UnrealOP", "UnrealOP Plugin"), NAME_None, FUnrealOPStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
