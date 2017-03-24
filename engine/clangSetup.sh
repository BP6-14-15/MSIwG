#/bin/bash
sudo apt-get install clang-4.0 lldb-4.0 lld-4.0 libc++-dev subversion cmake

if [[ $? != 0 ]]; then
  echo "couldn't install dependencies... aborting"
  exit $?;
fi

instDir="$HOME/Downloads"

if [[ $# -eq 1 ]]; then
  instDir=$1
fi

cd $instDir 
mkdir -p LLVM
cd LLVM

svn co http://llvm.org/svn/llvm-project/llvm/branches/release_40 llvm 
cd llvm/projects
svn co http://llvm.org/svn/llvm-project/libcxx/branches/release_40 libcxx
svn co http://llvm.org/svn/llvm-project/libcxxabi/branches/release_40 libcxxabi
cd ../.. 
mkdir -p build 
cd build
CC=clang-4.0 CXX=clang++-4.0 cmake ../llvm
make cxx
