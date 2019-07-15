#
#		Rebuild emulator
#
pushd ../roms
python export.py
popd
rm cxp
cp  ../core/65816.* ../core/65816core.c ../core/traps.h . 
make -f makefile.linux




