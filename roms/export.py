import os
def reverse(x):
	y = 0
	for i in range(0,8):
		y = y * 2 + (x & 1)
		x = x >> 1
	return y
#
#		ROM exporter
#
def export(sourceFile,targetFile,patch):
	romImage = [x for x in open(sourceFile,"rb").read(-1)]
	print("Exporting {0} ({1} bytes)".format(sourceFile,len(romImage)))
	if (patch):
		print("Patching")
		amstrad = [x for x in open("cpc6128.rom","rb").read(-1)]
		start = 32 * 8
		end = 127 * 8
		for i in range(start,end):
			romImage[i] = reverse(amstrad[i+0x3800])
		for i in range(0,8):
			romImage[96*8+i] = 0
	h = open(targetFile,"w")
	romImage = ",".join([str(x) for x in romImage])
	romImage = "{ "+romImage+" }"
	arrayName = targetFile.split(os.sep)[-1][:-4]
	h.write("static const BYTE8 {0}[] = {1} ;\n\n".format(arrayName,romImage))
	h.close()

export("basic.rom","../emulator/basic_rom.inc",0)
export("character.rom","../emulator/character_rom.inc",1)
export("c2_cwmhigh.rom","../emulator/monitor_rom.inc",0)
