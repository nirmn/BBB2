#ifndef Ripple_h
#define Ripple_h
#include "ObjBase.h"
using namespace std;



class Ripple : public ObjBase
{
public:
	Ripple(unsigned long i_Time, CRGB i_Leds[NUM_Of_LEDS], unsigned int i_InputSignal, byte i_Strip, CRGB i_Color);
	bool TryToMove(unsigned long i_Time);
	void Close();
private:
	void moove();
	bool checkDone();
  byte m_ItrPerCycle;
  byte m_NumOfCycls;
  byte m_CycleCounter;

};

#endif
