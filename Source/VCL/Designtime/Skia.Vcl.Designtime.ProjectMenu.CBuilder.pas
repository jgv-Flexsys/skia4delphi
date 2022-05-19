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
unit Skia.Vcl.Designtime.ProjectMenu.CBuilder;

interface

{$SCOPEDENUMS ON}

procedure Register;

implementation

uses
  { Delphi }
  Winapi.Windows,
  Winapi.ShLwApi,
  System.SysUtils,
  System.Classes,
  System.IOUtils,
  System.TypInfo,
  System.Generics.Collections,
  Vcl.ActnList,
  Vcl.Dialogs,
  ToolsAPI,
  DeploymentAPI,
  DesignIntf,
  DCCStrs;

{ Register }

procedure Register;
begin
  ForceDemandLoadState(dlDisable);

end;

end.
