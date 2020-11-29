
rem  Under Windows, starts two instances (nodes) of the default
rem  solution in lab2 of TNM086, using the configuration that sets up
rem  VRPN tracking,

call C:\VRSystem\setenv-gramods.bat

start build\solution.exe --config urn:gramods:config/se.liu.vortex.xml --param SyncNode.localPeerIdx=0
start build\solution.exe --config urn:gramods:config/se.liu.vortex.xml --param SyncNode.localPeerIdx=1
