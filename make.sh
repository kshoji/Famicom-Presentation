#! /bin/sh

which cc65 || echo "Please install cc65 first!\nhttp://www.cc65.org/index.php#Download" && exit
cd src && javac jp/kshoji/famicom/* -d ../bin && cd ..
java -classpath ./bin jp.kshoji.famicom.PresentationSourceProcessor
cd cc65 && sh compile.sh && cd ..
