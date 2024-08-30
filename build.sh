TARGET=$1
BINDIR=./bin

# 创建工作目录
if [ ! -d "${BINDIR}" ];then
  mkdir ./bin
fi

echo
make clean && make \
&& echo \
&& echo \
&& echo \
&& echo 
if [ "${TARGET}" != "" ];then
   ./bin/${TARGET}
    exit
fi
