@echo off
SET VCBIN=C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin
SET CSBIN=C:\Program Files (x86)\MSBuild\14.0\Bin
SET SDKBIN=C:\Program Files (x86)\Microsoft SDKs\Windows\v8.1A\bin\NETFX 4.5.1 Tools
set NSPACE=goombasav_cs

@rem Set up environment variables
call "%VCBIN%\vcvars32"

@rem Compile C++
"%VCBIN%\cl" /clr /LN /MD /TP ..\goombasav_net\expose.cpp ..\goombasav.c ..\minilzo\minilzo.c

@rem Handle C# resx files
"%SDKBIN%\resgen" Form1.resx %NSPACE%.Form1.resources /r:"C:/Program Files (x86)/Reference Assemblies/Microsoft/Framework/.NETFramework/v4.0/System.Drawing.dll"
"%SDKBIN%\resgen" AboutForm.resx %NSPACE%.AboutForm.resources

@rem Compile C#, with reference to C++ obj file
"%CSBIN%\csc" /target:module /addmodule:expose.obj Form1.cs Form1.Designer.cs AboutForm.cs AboutForm.Designer.cs

@rem Linker
"%VCBIN%\link" /LTCG /CLRIMAGETYPE:IJW /ENTRY:goombasav_cs.Form1.Main /SUBSYSTEM:WINDOWS,5.01 /ASSEMBLYRESOURCE:%NSPACE%.Form1.resources /ASSEMBLYRESOURCE:%NSPACE%.AboutForm.resources /OUT:goombasav_cs.exe goombasav.obj minilzo.obj Form1.netmodule expose.obj

@rem Cleanup
@echo on
del *.obj *.netmodule *.resources

@pause
