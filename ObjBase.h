#ifndef _OBJ_BASE_
#define _OBJ_BASE_
#include "Includes.h"
//#include "Colors.h"
using namespace std;


class ObjBase {
protected:
//[row][col]
unsigned int StartingPointOfLEDInEdge[NUM_OF_STRIPS][NUM_OF_EDGES]  = {
    {START_LED_A0E0,START_LED_A0E1,START_LED_A0E2,START_LED_A0E3,0},
    {START_LED_A1E0,START_LED_A1E1,START_LED_A1E2,START_LED_A1E3,0},
    {START_LED_A2E0,START_LED_A2E1,START_LED_A2E2,START_LED_A2E3,0},
    {START_LED_A3E0,START_LED_A3E1,START_LED_A3E2,START_LED_A3E3,0},
    {START_LED_A4E0,START_LED_A4E1,START_LED_A4E2,START_LED_A4E3,0},
    {START_LED_A5E0,START_LED_A5E1,START_LED_A5E2,START_LED_A5E3,0},
    {START_LED_A6E0,START_LED_A6E1,START_LED_A6E2,START_LED_A6E3,0},
    {START_LED_A7E0,START_LED_A7E1,START_LED_A7E2,START_LED_A7E3,0},
    {START_LED_A8E0,START_LED_A8E1,START_LED_A8E2,START_LED_A8E3,0},
    {START_LED_A9E0,START_LED_A9E1,START_LED_A9E2,START_LED_A9E3,START_LED_A9E4},
    {START_LED_A10E0,START_LED_A10E1,START_LED_A10E2,START_LED_A10E3,0}
  };
byte numOfLEDsInEdge[NUM_OF_STRIPS][NUM_OF_EDGES] = {
    {NUM_OF_LEDS_A0E0,NUM_OF_LEDS_A0E1,NUM_OF_LEDS_A0E2,NUM_OF_LEDS_A0E3,0},
    {NUM_OF_LEDS_A1E0,NUM_OF_LEDS_A1E1,NUM_OF_LEDS_A1E2,NUM_OF_LEDS_A1E3,0},
    {NUM_OF_LEDS_A2E0,NUM_OF_LEDS_A2E1,NUM_OF_LEDS_A2E2,NUM_OF_LEDS_A2E3,0},
    {NUM_OF_LEDS_A3E0,NUM_OF_LEDS_A3E1,NUM_OF_LEDS_A3E2,NUM_OF_LEDS_A3E3,0},
    {NUM_OF_LEDS_A4E0,NUM_OF_LEDS_A4E1,NUM_OF_LEDS_A4E2,NUM_OF_LEDS_A4E3,0},
    {NUM_OF_LEDS_A5E0,NUM_OF_LEDS_A5E1,NUM_OF_LEDS_A5E2,NUM_OF_LEDS_A5E3,0},
    {NUM_OF_LEDS_A6E0,NUM_OF_LEDS_A6E1,NUM_OF_LEDS_A6E2,NUM_OF_LEDS_A6E3,0},
    {NUM_OF_LEDS_A7E0,NUM_OF_LEDS_A7E1,NUM_OF_LEDS_A7E2,NUM_OF_LEDS_A7E3,0},
    {NUM_OF_LEDS_A8E0,NUM_OF_LEDS_A8E1,NUM_OF_LEDS_A8E2,NUM_OF_LEDS_A8E3,0},
    {NUM_OF_LEDS_A9E0,NUM_OF_LEDS_A9E1,NUM_OF_LEDS_A9E2,NUM_OF_LEDS_A9E3,NUM_OF_LEDS_A9E4},
    {NUM_OF_LEDS_A10E0,NUM_OF_LEDS_A10E1,NUM_OF_LEDS_A10E2,NUM_OF_LEDS_A10E3,0}
  };
	byte numOfLEDsInStrip[NUM_OF_STRIPS] = {NUM_LEDS0, NUM_LEDS1, NUM_LEDS2, NUM_LEDS3, NUM_LEDS4, NUM_LEDS5, NUM_LEDS6, NUM_LEDS7, NUM_LEDS8, NUM_LEDS9, NUM_LEDS10};
public:
	ObjBase(unsigned long i_Time,CRGB i_Leds[]) ;
	virtual void Close()=0;
	bool TryToMove(unsigned long i_Time);
	bool CanBeWithOthers();
	unsigned long FinalTime(){return m_FinalTime;};
protected:
   CRGB *m_Leds;
   byte m_StripNumber;
   virtual bool checkDone()=0;
   unsigned long m_StartingTime;
   unsigned long m_CurrentTime;
   unsigned long m_TimeOfNextInterval;
   unsigned long m_FinalTime;
   unsigned int m_CurrentInterval;
   unsigned int m_NumOfIntervals;
   unsigned int m_InputSignal;
   virtual void moove() = 0;
   bool CheckIfNeedToMove();
   unsigned int m_TimeOfInterval;
   bool m_CanBeWithOthers;
   CRGB  m_DefaultColor;
   void myFadeToBackBy(byte i_Strip, byte i_From, byte i_NumLeds, byte i_By);
   void myFadeToFrontBy(byte i_Strip, byte i_From, byte i_NumLeds, byte i_By);
   void myFillSolidEdge(byte i_Strip, byte i_Edge, CRGB i_Color);
   void myFillSolidAllEdges(byte i_Edge, CRGB i_Color);
   void myFadeEdge(byte i_Strip, byte i_Edge, byte i_By);
   void myFadeAllEdges(byte i_Edge, byte i_By);
   unsigned int c2d(byte i_Strip, byte i_LED);
   void myBackFillSolid(byte i_Strip, byte i_From, byte i_NumLeds, CRGB i_Color);
   void myFrontFillSolid(byte i_Strip, byte i_From, byte i_NumLeds, CRGB i_Color);
};

#endif
