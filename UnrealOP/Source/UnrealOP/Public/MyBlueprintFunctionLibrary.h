// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UnrealOp.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"




#include "MyBlueprintFunctionLibrary.generated.h"

/**
*
*/
UCLASS()
class UNREALOP_API UMyBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "op")
		static bool openPoseTutorialPose1();
};
