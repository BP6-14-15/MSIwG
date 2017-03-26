#!/bin/bash
instDir="$HOME/Downloads"

if [[ $# -eq 1 ]]; then
  instDir=$1
fi

export LD_LIBRARY_PATH="$instDir/LLVM/build/lib"
./GameEngine
#./GameEngine --fpLibPath=../example/libclientA.so --fpFcnName=updatePlayer

