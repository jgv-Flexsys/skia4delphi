#ifndef UnitRADStudioCBuilder
#define UnitRADStudioCBuilder

[Code]
{************************************************************************}
{                                                                        }
{                              Skia4Delphi                               }
{                                                                        }
{ Copyright (c) 2011-2022 Google LLC.                                    }
{ Copyright (c) 2021-2022 Skia4Delphi Project.                           }
{                                                                        }
{ Use of this source code is governed by a BSD-style license that can be }
{ found in the LICENSE file.                                             }
{                                                                        }
{************************************************************************}
// unit RADStudio;

// interface

// uses
  #include "Source\RADStudio.inc"
  #include "Source\RADStudio.Project.inc"

/// <summary> Try to add a path to the browsing path of the C++Builder </summary>
function TryAddCBuilderBrowsingPath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const APath: string): Boolean; forward;
/// <summary> Try to add a path to the include path of the C++Builder </summary>
function TryAddCBuilderIncludePath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const APath: string): Boolean; forward;
/// <summary> Try to add a path to the library path of the C++Builder </summary>
function TryAddCBuilderLibraryPath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const APath: string): Boolean; forward;
/// <summary> Try to remove a path to the browsing path of the C++Builder </summary>
function TryRemoveCBuilderBrowsingPath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const APath: string): Boolean; forward;
/// <summary> Try to remove a path to the include path of the C++Builder </summary>
function TryRemoveCBuilderIncludePath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const APath: string): Boolean; forward;
/// <summary> Try to remove a path to the library path of the C++Builder </summary>
function TryRemoveCBuilderLibraryPath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const APath: string): Boolean; forward;

// implementation

// uses
  #include "Source\Setup.Utils.inc"
  #include "Source\String.Utils.inc"

/// <summary> Try to add a path to the C++Builder registry (in a specific registry name) </summary>
function _TryAddCBuilderPath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const ARegName, APath: string): Boolean; forward;
/// <summary> Try to remove a path to the C++Builder registry (in a specific registry name) </summary>
function _TryRemoveCBuilderPath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const ARegName, APath: string): Boolean; forward;

function TryAddCBuilderBrowsingPath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const APath: string): Boolean;
begin
  Result := _TryAddCBuilderPath(ARADStudioVersion, APlatform, 'BrowsingPath', APath);
  if APlatform = pfWin32 then
    Result := Result and _TryAddCBuilderPath(ARADStudioVersion, APlatform, 'BrowsingPath_Clang32', APath);
end;

function TryAddCBuilderIncludePath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const APath: string): Boolean;
begin
  Result := _TryAddCBuilderPath(ARADStudioVersion, APlatform, 'IncludePath', APath);
  if APlatform = pfWin32 then
    Result := Result and _TryAddCBuilderPath(ARADStudioVersion, APlatform, 'IncludePath_Clang32', APath);
end;

function TryAddCBuilderLibraryPath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const APath: string): Boolean;
begin
  Result := _TryAddCBuilderPath(ARADStudioVersion, APlatform, 'LibraryPath', APath);
  if APlatform = pfWin32 then
    Result := Result and _TryAddCBuilderPath(ARADStudioVersion, APlatform, 'LibraryPath_Clang32', APath);
end;

function _TryAddCBuilderPath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const ARegName, APath: string): Boolean;
var
  LNewValue: string;
  LPathsToInsert: TArrayOfString;
  LCurrentPaths: TArrayOfString;
  I: Integer;
begin
  Result := RegKeyExists(HKEY_CURRENT_USER, GetRADStudioRegKey(ARADStudioVersion) + '\C++\Paths\' + GetProjectPlatformName(APlatform));
  if Result then
  begin
    if (not RegQueryStringValue(HKEY_CURRENT_USER, GetRADStudioRegKey(ARADStudioVersion) + '\C++\Paths\' + GetProjectPlatformName(APlatform), ARegName, LNewValue)) or (LNewValue = '') then
      LNewValue := APath
    else
    begin
      LPathsToInsert := SplitString(APath, ';');
      LCurrentPaths := SplitString(LNewValue, ';');
      for I := 0 to GetArrayLength(LPathsToInsert) - 1 do
      begin
        LNewValue := LPathsToInsert[I];
        LCurrentPaths := RemoveString(LCurrentPaths, LNewValue, False);
        LCurrentPaths := RemoveString(LCurrentPaths, LNewValue + '\', False);
        LCurrentPaths := AppendString(LCurrentPaths, LNewValue, False);
      end;
      LNewValue := JoinStrings(LCurrentPaths, ';', False);
    end;
    Result := RegWriteStringValue(HKEY_CURRENT_USER, GetRADStudioRegKey(ARADStudioVersion) + '\C++\Paths\' + GetProjectPlatformName(APlatform), ARegName, LNewValue);
  end;
  if not Result then
    Log(Format('RADStudio.CBuilder._TryAddCBuilderPath: Failed to add a path to the C++Builder registry (RegName: "%s", Path: "%s")', [ARegName, APath]));
end;

function TryRemoveCBuilderBrowsingPath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const APath: string): Boolean;
begin
  Result := _TryRemoveCBuilderPath(ARADStudioVersion, APlatform, 'BrowsingPath', APath);
  if APlatform = pfWin32 then
    Result := _TryRemoveCBuilderPath(ARADStudioVersion, APlatform, 'BrowsingPath_Clang32', APath) and Result;
end;

function TryRemoveCBuilderIncludePath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const APath: string): Boolean;
begin
  Result := _TryRemoveCBuilderPath(ARADStudioVersion, APlatform, 'IncludePath', APath);
  if APlatform = pfWin32 then
    Result := _TryRemoveCBuilderPath(ARADStudioVersion, APlatform, 'IncludePath_Clang32', APath) and Result;
end;

function TryRemoveCBuilderLibraryPath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const APath: string): Boolean;
begin
  Result := _TryRemoveCBuilderPath(ARADStudioVersion, APlatform, 'LibraryPath', APath);
  if APlatform = pfWin32 then
    Result := _TryRemoveCBuilderPath(ARADStudioVersion, APlatform, 'LibraryPath_Clang32', APath) and Result;
end;

function _TryRemoveCBuilderPath(const ARADStudioVersion: TRADStudioVersion; const APlatform: TProjectPlatform; const ARegName, APath: string): Boolean;
var
  LNewValue: string;
  LPathsToRemove: TArrayOfString;
  LCurrentPaths: TArrayOfString;
  I: Integer;
begin
  Result := True;
  if RegQueryStringValue(HKEY_CURRENT_USER, GetRADStudioRegKey(ARADStudioVersion) + '\C++\Paths\' + GetProjectPlatformName(APlatform), ARegName, LNewValue) and (LNewValue <> '') then
  begin
    LPathsToRemove := SplitString(APath, ';');
    LCurrentPaths := SplitString(LNewValue, ';');
    for I := 0 to GetArrayLength(LPathsToRemove) - 1 do
    begin
      LNewValue := LPathsToRemove[I];
      LCurrentPaths := RemoveString(LCurrentPaths, LNewValue, False);
      LCurrentPaths := RemoveString(LCurrentPaths, LNewValue + '\', False);
    end;
    LNewValue := JoinStrings(LCurrentPaths, ';', False);
    Result := RegWriteStringValue(HKEY_CURRENT_USER, GetRADStudioRegKey(ARADStudioVersion) + '\C++\Paths\' + GetProjectPlatformName(APlatform), ARegName, LNewValue);
    if not Result then
      Log(Format('RADStudio.CBuilder._TryRemoveCBuilderPath: Failed to remove a path to the C++Builder registry (RegName: "%s", Path: "%s"). Failed to write the registry "%s"', [ARegName, APath, 'HKEY_CURRENT_USER\' + GetRADStudioRegKey(ARADStudioVersion) + '\C++\Paths\' + GetProjectPlatformName(APlatform)]));
  end
  else
    Log(Format('RADStudio.CBuilder._TryRemoveCBuilderPath: Failed to remove a path to the C++Builder registry (RegName: "%s", Path: "%s"). The registry name, does not exists in registry "%s".', [ARegName, APath, 'HKEY_CURRENT_USER\' + GetRADStudioRegKey(ARADStudioVersion) + '\C++\Paths\' + GetProjectPlatformName(APlatform)]));
end;

// end.
#endif
