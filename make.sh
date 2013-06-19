#! /bin/sh

if [ `which cc65 | wc -c` = 0 ]; then
	echo "Please install cc65 first!\nhttp://www.cc65.org/index.php#Download"
	exit
fi
if [ ! -e ./bin ]; then
mkdir ./bin
fi
cd src && javac jp/kshoji/famicom/* -d ../bin && cd ..
java -classpath ./bin jp.kshoji.famicom.PresentationSourceProcessor
cd cc65 && sh compile.sh && cd ..
