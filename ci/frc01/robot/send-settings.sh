#!/bin/bash
HOST='10.35.12.2'
USER='lvuser'

# Add file names here to send them
FILESRC=('GUISettings.txt')
PATHDEST=('/home/lvuser')
FILEDEST=('GUISettings.txt')

# script: send files in list
for (( i=0 ; i < ${#FILESRC[@]} ; i++ ));
do
    scp ${FILEDEST[$i]} $USER@$HOST:${PATHDEST[$i]}
done
