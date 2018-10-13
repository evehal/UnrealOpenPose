// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

// Only for WIN10 x64
// https://github.com/CMU-Perceptual-Computing-Lab/openpose/blob/master/doc/installation.md#windows-portable-demo for detail

using UnrealBuildTool;
using System;
using System.IO;

public class UnrealOP : ModuleRules
{
    private string OpenPosePath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../ThirdParty/OpenPose/")); }
    }
    
    private string OpenPose3rdParty
    {
        get { return Path.GetFullPath(Path.Combine(OpenPosePath, "3rdparty")); }
    }

    private string OpenPoseInclude
    {
        get { return Path.GetFullPath(Path.Combine(OpenPosePath, "include")); }
    }
    private string OpenPoseLibPath
    {
        get { return Path.GetFullPath(Path.Combine(OpenPosePath, "lib")); }
    }

    private string OpenCVInclude
    {
        get { return Path.GetFullPath(Path.Combine(OpenPose3rdParty, "windows/opencv/include")); }
    }

    private string OpenCVLibPath
    {
        get { return Path.GetFullPath(Path.Combine(OpenPose3rdParty, "windows/opencv/x64/vc14/lib")); }
    }

    private string OpenCVDllPath
    {
        get { return Path.GetFullPath(Path.Combine(OpenPose3rdParty, "windows/opencv/x64/vc14/bin")); }
    }


    public UnrealOP(ReadOnlyTargetRules Target) : base(Target)
	{
        
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bUseRTTI = true;
        bEnableExceptions = true;


        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
                //Path.Combine(OpenPoseInclude),
                Path.Combine(OpenCVInclude),
                
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
                //Path.Combine(OpenPoseInclude),
                Path.Combine(OpenCVInclude),
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"UnrealEd",
				"LevelEditor",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);


       

        //Add OpenCV lib and load dlls
        PublicLibraryPaths.Add(OpenCVLibPath);
        PublicDelayLoadDLLs.Add("opencv_world310.dll");
        PublicDelayLoadDLLs.Add("opencv_ffmpeg310_64.dll");
        PublicAdditionalLibraries.Add("opencv_world310.lib");

        //Load caffe dll

        //Add OpenPose lib and load dlls
        //PublicLibraryPaths.Add(OpenPoseLibPath);
        //PublicDelayLoadDLLs.Add("openposed.dll");
        //PublicAdditionalLibraries.Add("openposed.lib");
    }
}
