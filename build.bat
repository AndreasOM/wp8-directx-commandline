rem @echo off
set PROJECT_BASE=%cd%

pushd PhoneDirect3DApp2

rem set PROJECT_BASE=c:\customers\Other\samples\wp8-directx-commandline
rem goto xap

rem Compile shaders

"C:\Program Files (x86)\Windows Kits\8.0\bin\x86\fxc.exe" /nologo /E"main" /T ps_4_0_level_9_3 /Fo "%PROJECT_BASE%\Debug\PhoneDirect3DApp2\SimplePixelShader.hlslo" /Zi SimplePixelShader.hlsl

"C:\Program Files (x86)\Windows Kits\8.0\bin\x86\fxc.exe" /nologo /E"main" /T vs_4_0_level_9_3 /Fo "%PROJECT_BASE%\Debug\PhoneDirect3DApp2\SimpleVertexShader.hlslo" /Zi SimpleVertexShader.hlsl

rem Compile code

"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\WPSDK\WP80\bin\CL.exe" -DCOMMAND_LINE_BUILD=1 /c /I"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\WPSDK\WP80\include" /I"C:\Program Files (x86)\Windows Phone Kits\8.0\Include" /I"C:\Program Files (x86)\Windows Phone Kits\8.0\Include\minwin" /I"C:\Program Files (x86)\Windows Phone Kits\8.0\Include\mincore" /I"%PROJECT_BASE%\PhoneDirect3DApp2\\" /IDebug\ /I"%PROJECT_BASE%\PhoneDirect3DApp2\\" /I"Generated Files\\" /IDebug\ /ZI /ZW /ZW:nostdlib /nologo /W3 /WX- /sdl /MP /Od /Oy- /D PSAPI_VERSION=2 /D _UNICODE /D UNICODE /D _DEBUG /D WINAPI_FAMILY=WINAPI_FAMILY_PHONE_APP /Gm- /EHsc /RTC1 /MDd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Fo"Debug\\" /Fd"Debug\vc110.pdb" /Gd /TP /FU"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\WPSDK\WP80\lib\platform.winmd" /FU"C:\Program Files (x86)\Windows Phone Kits\8.0\Windows Metadata\Windows.winmd" /analyze- /errorReport:queue CheckerMesh.cpp CubeMesh.cpp Mesh.cpp PhoneDirect3DApp2.cpp CubeRenderer.cpp Direct3DBase.cpp

:link
rem Link

"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\WPSDK\WP80\bin\link.exe" /ERRORREPORT:QUEUE /OUT:"%PROJECT_BASE%\Debug\PhoneDirect3DApp2\wp8-directx.exe" /INCREMENTAL /NOLOGO /LIBPATH:"C:\Program Files (x86)\Windows Phone Kits\8.0\lib\x86" /LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\WPSDK\WP80\lib" d3d11.lib WindowsPhoneCore.lib RuntimeObject.lib PhoneAppModelHost.lib /NODEFAULTLIB:ole32.lib /MANIFEST:NO /DEBUG /PDB:"%PROJECT_BASE%\Debug\PhoneDirect3DApp2\PhoneDirect3DApp2.pdb" /SUBSYSTEM:WINDOWS /TLBID:1 /WINMD:NO /WINMDFILE:"%PROJECT_BASE%\Debug\PhoneDirect3DApp2\PhoneDirect3DApp2.winmd" /DYNAMICBASE /NXCOMPAT /MACHINE:X86 Debug\CheckerMesh.obj Debug\CubeMesh.obj Debug\Mesh.obj Debug\PhoneDirect3DApp2.obj Debug\CubeRenderer.obj Debug\Direct3DBase.obj
  
:xap
rem Package

set XAP=Test-XAP
md %XAP%
copy "%PROJECT_BASE%\Debug\PhoneDirect3DApp2\wp8-directx.exe" %XAP%
copy "%PROJECT_BASE%\Debug\PhoneDirect3DApp2\*.hlslo" %XAP%

copy "%PROJECT_BASE%\PhoneDirect3DApp2\WMAppManifest.xml" %XAP%

md %XAP%\Assets

md %XAP%\Assets\Tiles

pushd %XAP%

zip -u -v ..\test.xap *

popd

popd


