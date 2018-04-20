#ifndef Bullet_h
#define Bullet_h
#include "ObjBase.h"
using namespace std;



class Bullet : public ObjBase
{
public:
	Bullet(unsigned long i_Time, CRGB i_Leds[NUM_Of_LEDS], unsigned int i_InputSignal, byte i_StripNumber, CRGB i_Color);
	bool TryToMove(unsigned long i_Time);
	void Close();
private:
	void moove();
	bool checkDone();
	byte m_TailSize;
  unsigned int m_Pos;
  bool m_IsUp;

};

#endif
