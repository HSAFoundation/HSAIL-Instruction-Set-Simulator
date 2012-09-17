' Create the Installshield Object - Uncomment on Dev machine
' Set oProject = CreateObject ("ISWiAuto12.ISWiProject")
' Create the Installshield Object - Uncomment on Build server
Set oProject = CreateObject ("IswiAuto15.ISWiProject")

' Create the WScript object to read Environment Variables later
Set WshShell = CreateObject("WScript.Shell")

' Read our Environment Variables
vProductVersion = WshShell.ExpandEnvironmentStrings("%PKG_VERSION%")
vProjectFile = WshShell.ExpandEnvironmentStrings("%SCRIPT_NAME%")

' Set Product Code
sProductCode = oProject.GenerateGUID
vProductName = WshShell.ExpandEnvironmentStrings("%PROJECT_NAME%")

'Open the Project
oProject.OpenProject vProjectFile

' Begin Updates

oProject.ProductVersion = vProductVersion
oProject.ProductCode = sProductCode
oProject.ProductName = vProductName

' Save and Close
oProject.SaveProject
oProject.CloseProject

Set oProject = Nothing