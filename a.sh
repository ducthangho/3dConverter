WORKING_FOLDER="/mnt/c/Users/mylaptop/Google Drive/3DProjects"
if [ $# -gt 0 ]; then
"$WORKING_FOLDER/build/Release/bin/3dConverter" "$@"
else
"$WORKING_FOLDER/build/Release/bin/3dConverter" "$WORKING_FOLDER/AC14-FZK-Haus.ifc"
fi