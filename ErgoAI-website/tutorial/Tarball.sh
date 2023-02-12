#!/bin/sh 


cd ..

EXCLUDEFILE=tutorial/.excludedFiles

tar cvf tutorial/tutorial.tar --exclude-from=$EXCLUDEFILE tutorial

