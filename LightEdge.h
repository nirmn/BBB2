#ifndef LightEdge_h
#define LightEdge_h
#include "ObjBase.h"
using namespace std;



class LightEdge : public ObjBase
{
public:
	LightEdge(unsigned long i_Time, CRGB i_Leds[NUM_Of_LEDS], unsigned int i_InputSignal, byte i_Edge, CRGB i_Color);
	bool TryToMove(unsigned long i_Time);
	void Close();
private:
	void moove();
	bool checkDone();
	byte m_TailSize;
  unsigned int m_Pos;
  bool m_IsUp;
  byte m_Edge;
};

#endif
