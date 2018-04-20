#ifndef _OBJ_FACTORY_
#define _OBJ_FACTORY_

#include "ObjBase.h"
#include "Includes.h"

#define ICF 10-NUM_OF_DRUM_INPUTS
#define ICF2 15

class ObjFactory
{	
private:
	CRGB *m_Leds;
	unsigned int m_Counter;
	unsigned int m_SpeedFactor;
	unsigned long m_Strip;
	unsigned long m_History[NUM_OF_DRUM_INPUTS+(NUM_OF_GUITAR_INPUTS*2)]={0};
public:
	ObjFactory(CRGB i_Leds[]);
	ObjBase* MakeNewObject(byte i_ItemToCreate, unsigned long i_Time, unsigned int i_InputSignal);
};

#endif
