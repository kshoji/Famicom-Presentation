#! /bin/sh

cc65 -Oi main.c --add-source || exit;
ca65 main.s || exit;
ca65 crt0.s || exit;
ld65 -C nes.cfg -o main.nes crt0.o main.o runtime.lib || exit;
