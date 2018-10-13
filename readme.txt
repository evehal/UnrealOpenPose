UE4 4.19.2&UE4 4.20.2 + VS2015 community update3 + cuda 8.0 + cudnn 5.1 + WIN10

完整工程下载:
1:你需要拷贝下载里的models&ThirdParty到本插件里才能使用此插件.
2:点击ToolBar上的UrealOP按钮,会弹出识别窗口
UE4的内存管理和std容器不兼容,所以需要做一个dll(OpenPoseForUnreal.dll).Dll必须是release版本

ue4
full project download link(include models& ThirdParty):
1:to use this plugin ,you need to copy models&ThirdParty folder from download first
2:click UnrealOP button on toolbar in editor,will show the result.

the memory manager of ue4 is incompability with std container and shared_ptr.so you have to make a dll(OpenPoseForUnreal.dll) as a wrapper for ue4.
u should put codes with std into the dll.dll must be release version.