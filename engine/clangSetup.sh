#!bin/bash
errLogPath=".err.log"
exec 3>&1
exec > /dev/null 2> $errLogPath
echo "" > $errLogPath

function echoStd {
    while (( "$#" )); do
        echo $1 >&3
        shift
    done
}

function close {
    exec 3>&-
}

function checkExit {
    if [[ $? != 0 ]]; then
        echoStd "clang installation failed, check $PWD/$errLogPath"
        exit $?;
    fi
}

export DEBIAN_FRONTEND=noninteractive

echoStd "Installing clang, this might take a while..."
sudo apt-get -y install clang-4.0 lldb-4.0 lld-4.0 libc++-dev subversion cmake

checkExit

instDir="$HOME/Downloads"

if [[ $# -eq 1 ]]; then
  instDir=$1
fi

echoStd "Installing libc++ at $instDir"

cd $instDir 
mkdir -p LLVM
cd LLVM

svn co http://llvm.org/svn/llvm-project/llvm/branches/release_40 llvm 
checkExit

cd llvm/projects

svn co http://llvm.org/svn/llvm-project/libcxx/branches/release_40 libcxx

checkExit

svn co http://llvm.org/svn/llvm-project/libcxxabi/branches/release_40 libcxxabi 

checkExit

cd ../..

echoStd "Building libc++..."

mkdir -p build 

cd build
CC=clang-4.0 CXX=clang++-4.0 cmake ../llvm 
checkExit
make cxx 

echoStd "Done installing clang"
