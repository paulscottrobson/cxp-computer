// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		hardware.c
//		Purpose:	Hardware Emulation
//		Created:	12th July 2019
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#include "sys_processor.h"
#include "hardware.h"

#if defined(WINDOWS) || defined(LINUX)

#include "gfx.h"
#include <stdlib.h>

// *******************************************************************************************************************************
//												Reset Hardware
// *******************************************************************************************************************************

void HWReset(void) {
}

// *******************************************************************************************************************************
//												  Reset CPU
// *******************************************************************************************************************************

void HWSync(void) {
}

// *******************************************************************************************************************************
//									  Write to display/colour RAM
// *******************************************************************************************************************************

void HWWriteDisplay(WORD16 address,BYTE8 data) {	
}

// *******************************************************************************************************************************
//											Access keyboard
// *******************************************************************************************************************************

static const BYTE8 keyboardMap[] = {
	'1','2','3','4','5','6','7',0,
	'8','9','0','@','-',GFXKEY_BACKSPACE,0,0,
	'.','L','O',0,GFXKEY_RETURN,0,0,0,
	'W','E','R','T','Y','U','I',0,
	'S','D','F','G','H','J','K',0,
	'X','C','V','B','N','M',',',0,
	'Q','A','Z',' ','/',';','P',0,
	GFXKEY_F11,GFXKEY_CONTROL,GFXKEY_F12,0,0,GFXKEY_LSHIFT,GFXKEY_RSHIFT,0	
};

BYTE8 HWWriteKeyboard(BYTE8 pattern) {
	BYTE8 outPattern = 0x00;
	for (BYTE8 row = 0;row < 8;row++) {
		if ((pattern & (0x80 >> row)) != 0) {
			for (BYTE8 col = 0;col < 8;col++) {
				if (GFXIsKeyPressed(keyboardMap[row*8+col])) {
					outPattern = outPattern | (0x80 >> col);
				}
			}
		}
	}
	if ((pattern & 0x01) && (!GFXIsKeyPressed(GFXKEY_LSHIFT))) {
		outPattern |= 1;
	}
	//outPattern = outPattern ^ 0xFF;
	return outPattern;
}

#endif

#ifdef ESP32

// *******************************************************************************************************************************
//												Reset Hardware
// *******************************************************************************************************************************

void HWReset(void) {
}

// *******************************************************************************************************************************
//												  Reset CPU
// *******************************************************************************************************************************

static BYTE8 keyStatus[256] = {0};

void HWSync(void) {
	static int shift = 0;
	static int release = 0;
	int scanCode = HWGetScanCode();
	if (scanCode > 0) {
		if (scanCode == 0xE0) {
			shift = 0x80;
		} 
		if (scanCode == 0xF0) {
			release = 1;
		}
		if (scanCode != 0xE0 && scanCode != 0xF0) {
			int isDown = (release == 0);
			scanCode = (scanCode & 0x7F) | shift;
			//writeCharacter(scanCode & 0x0F,(scanCode >> 4)+2,isDown ? '*' : '.');
			keyStatus[scanCode] = isDown;
			shift = 0x00;
			release = 0x00;
		}
	}
}

// *******************************************************************************************************************************
//									  Write to display/colour RAM
// *******************************************************************************************************************************

void HWWriteDisplay(WORD16 address,BYTE8 data) {	
	if (address < 0xD085) return;
	int x1 = (address-0xD085) & 0x1F;
	int y1 = ((address-0xD085) >> 5) & 0x1F;
	if (x1 < 24 and y1 < 24) {
		address &= 0x3FF;
		HWWriteCharacter(x1,y1,CPUReadMemory(0xD000+address),CPUReadMemory(0xD400+address));
	}
}

// *******************************************************************************************************************************
//											Access keyboard
// *******************************************************************************************************************************

static const BYTE8 keyboardMap[] = {
		0x16,0x1E,0x26,0x25,0x2E,0x36,0x3D,0x00,	// 	1 2 3 4 5 6 7 .
		0x3E,0x46,0x45,0x52,0x4E,0x66,0x00,0x00, 	// 	8 9 0 @ - <Del> . .
		0x49,0x4B,0x44,0x00,0x5A,0x00,0x00,0x00, 	// <.> L O . <CR> . . .
		0x1D,0x24,0x2D,0x2C,0x35,0x3C,0x43,0x00, 	// W E R T Y U I .
		0x1B,0x23,0x2B,0x34,0x33,0x3B,0x42,0x00,	// S D F G H J K .
		0x22,0x21,0x2A,0x32,0x31,0x3A,0x41,0x00, 	// X C V B N M , .
		0x15,0x1C,0x1A,0x29,0x4A,0x4C,0x4D,0x00, 	// Q A Z Sp / ; P .
		0x00,0x14,0x76,0x00,0x00,0x12,0x59,0x00		// . CTL ESC . . LShift RShift .
};

BYTE8 HWWriteKeyboard(BYTE8 pattern) {
	pattern = pattern ^ 0xFF;
	BYTE8 outPattern = 0x00;
	for (BYTE8 row = 0;row < 8;row++) {
		if ((pattern & (0x80 >> row)) != 0) {
			for (BYTE8 col = 0;col < 8;col++) {
				if (keyStatus[keyboardMap[row*8+col]] != 0) {
					outPattern = outPattern | (0x80 >> col);
				}
			}
		}
	}
	outPattern = outPattern ^ 0xFF;
	return outPattern;
}
#endif

