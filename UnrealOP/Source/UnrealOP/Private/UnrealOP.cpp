// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.



#include "UnrealOP.h"
#include "UnrealOPStyle.h"
#include "UnrealOPCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

#include "LevelEditor.h"
#include "LogMacros.h"

#include "MyBlueprintFunctionLibrary.h"
#include <thread>

DEFINE_LOG_CATEGORY(LogOpenPose);

static const FName UnrealOPTabName("UnrealOP");
#define LOCTEXT_NAMESPACE "FUnrealOPModule"

typedef unsigned char* (*_extract_from_image) (	int logging_level, char* image_path, char* model_pose,
										char* model_folder, char* net_resolution,
										char* output_resolution, int num_gpu_start, float scale_gap, int scale_number,
										bool disable_blending, float render_threshold, float alpha_pose, bool show_frame);
_extract_from_image extract_from_image;

void FUnrealOPModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
#pragma region Init0
	FUnrealOPStyle::Initialize();
	FUnrealOPStyle::ReloadTextures();

	FUnrealOPCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUnrealOPCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FUnrealOPModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FUnrealOPModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FUnrealOPModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	TMap<FString, void*> DllsHandles;

	FString RootOpenCVPath = FPaths::ProjectPluginsDir() / FString::Printf(TEXT("UnrealOP/ThirdParty/OpenPose/lib/"));

	DllsHandles.Add("boost_system-vc140-mt-1_61");
	DllsHandles.Add("boost_filesystem-vc140-mt-1_61");
	DllsHandles.Add("boost_chrono-vc140-mt-1_61");
	DllsHandles.Add("boost_python-vc140-mt-1_61");
	DllsHandles.Add("boost_thread-vc140-mt-1_61");
	DllsHandles.Add("cublas64_80");
	DllsHandles.Add("cudart64_80");
	DllsHandles.Add("cudnn64_5");
	DllsHandles.Add("curand64_80");
	DllsHandles.Add("gflags");
	DllsHandles.Add("gflagsd");
	DllsHandles.Add("glog");
	DllsHandles.Add("glogd");
	DllsHandles.Add("libopenblas");
	DllsHandles.Add("libgfortran-3");
	DllsHandles.Add("libgcc_s_seh-1");
	DllsHandles.Add("caffezlib1");
	DllsHandles.Add("caffehdf5");
	DllsHandles.Add("caffehdf5_hl");
	DllsHandles.Add("opencv_ffmpeg310_64");
	//DllsHandles.Add("opencv_world310d");
	DllsHandles.Add("opencv_world310");
	DllsHandles.Add("opencv_core310");
	DllsHandles.Add("opencv_imgproc310");
	DllsHandles.Add("opencv_imgcodecs310");
	DllsHandles.Add("caffe");


	DllsHandles.Add("libquadmath-0.dll");

	DllsHandles.Add("openpose");
	DllsHandles.Add("OpenPoseForUnreal");

	for (auto& Val : DllsHandles)
	{
		FString Filename = FString::Printf(TEXT("%s.dll"), *Val.Key);

		FString LibraryPath = FPaths::Combine(*RootOpenCVPath, Filename);
		Val.Value = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;
		
		if (Val.Value)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s loaded"), *LibraryPath);

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s not loaded"), *LibraryPath);

		}
	}


#pragma endregion
	void* opWrapperDLLHandler = DllsHandles["OpenPoseForUnreal"];
	if (opWrapperDLLHandler) {
		FString funcName = "extract_from_image";
		extract_from_image = (_extract_from_image)FPlatformProcess::GetDllExport(opWrapperDLLHandler, *funcName);
		if (extract_from_image == NULL)
		{
			UE_LOG(LogOpenPose, Warning, TEXT("extract_from_image load fail"));
		}
	}
}

void FUnrealOPModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FUnrealOPStyle::Shutdown();

	FUnrealOPCommands::Unregister();
}


void ScaleAndSizeExtractor_extract()
{

}
void FUnrealOPModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	/*
	FText DialogText = FText::Format(
		LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
		FText::FromString(TEXT("FUnrealOPModule::PluginButtonClicked()")),
		FText::FromString(TEXT("UnrealOP.cpp"))
	);
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
	*/
	FString image_path = FPaths::ProjectPluginsDir() / "UnrealOP/media/COCO_val2014_000000000192.jpg";
	FString model_folder = FPaths::ProjectPluginsDir() / "UnrealOP/models/";
	extract_from_image(0, TCHAR_TO_ANSI(*image_path), "COCO", TCHAR_TO_ANSI(*model_folder),
						"-1x368", "-1x-1", 0, 0.3f, 1, false, 0.05f, 0.6f,true);
	
}

void FUnrealOPModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FUnrealOPCommands::Get().PluginAction);
}

void FUnrealOPModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FUnrealOPCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealOPModule, UnrealOP);
