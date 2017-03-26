#!/bin/bash
export DEBIAN_FRONTEND=noninteractive
sudo apt-get update
sudo apt-get -y install vim git

function checkExit {
    if [[ $? != 0 ]]; then
        echo "installation error"
        exit $?;
    fi
}
checkExit

wget -O - http://apt.llvm.org/llvm-snapshot.gpg.key|sudo apt-key add -

checkExit
runDir=$PWD
git clone https://github.com/BP6-14-15/MSIwG MSIwG
cd MSIwG/engine
./clangSetup.sh $runDir 
checkExit

cd example
echo "CXX=clang++-4.0 cmake ./ -DLIBCXXPATH=$runDir" > buildScript.sh
echo "make" >> buildScript.sh
chmod +x buildScript.sh
cp buildScript.sh ../source/
cd ..
./engineDependencies.sh

cd source
echo "#!/bin/bash" > run.sh
echo "export LD_LIBRARY_PATH=\"$runDir/LLVM/build/lib\"
./GameEngine
#./GameEngine --fpLibPath=../example/libclientA.so --fpFcnName=updatePlayer
" >> run.sh
chmod +x run.sh
