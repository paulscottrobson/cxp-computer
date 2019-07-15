// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		hardware.h
//		Purpose:	Hardware Emulation Header
//		Created:	12th July 2019
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#ifndef _HARDWARE_H
#define _HARDWARE_H

#ifdef ESP32
void HWWriteCharacter(BYTE8 x,BYTE8 y,BYTE8 ch,BYTE8 colour);
BYTE8 HWGetScanCode(void);
#endif

void HWReset(void);
void HWSync(void);
BYTE8 HWWriteKeyboard(BYTE8 pattern);
void HWWriteDisplay(WORD16 address,BYTE8 data);

#endif