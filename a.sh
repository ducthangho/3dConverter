WORKING_FOLDER="/mnt/c/Users/mylaptop/Google Drive/3DConverter/3dConverter"
if [ $# -gt 0 ];
then
"$WORKING_FOLDER/out/build/WSL-Release/3dConverter" "$@"
else
"$WORKING_FOLDER/out/build/WSL-Release/3dConverter" "$WORKING_FOLDER/20160125OTC-Conference Center - IFC4.ifc"
fi
