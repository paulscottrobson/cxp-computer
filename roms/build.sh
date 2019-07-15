#
#		Compile CEGMON/600 and copy ROM images to emulator space.
#
64tass -c -b -o cegmon.rom -L cegmon.lst cegmon.asm
cmp cegmon.rom cegmon.rom.original 

cp cegmon.rom ../emulator/monitor.rom
cp basic.rom ../emulator/basic.rom

#
#		Export font as .h file.
#
python export.py
