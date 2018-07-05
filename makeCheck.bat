@echo off
"C:\Progra~1\ARM\RVCT\Programs\3.1\569\win_32-pentium\armcc.exe" ~MMI_features_check.c -o ~MMI_features_check.obj -E --via ~MMI_features_check.def --via ~MMI_features_check.inc
if ERRORLEVEL 1 exit 1
@echo on
