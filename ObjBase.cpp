#include "ObjBase.h"

bool ObjBase::TryToMove(unsigned long i_Time)
{
  m_CurrentTime = i_Time;
  #ifdef DEBUG
	Serial.println("entering try");
	Serial.print("current time is: ");
	Serial.println(m_CurrentTime);
	Serial.print("time of next interval= ");
	Serial.println(m_TimeOfNextInterval);
  #endif
//   do actual move:
	if(CheckIfNeedToMove())
	{
	  moove();
	}
//   check if need to be killed
  return(!checkDone());
  }

ObjBase::ObjBase(unsigned long i_Time, CRGB i_Leds[])
{
  m_Leds = i_Leds;
  m_StartingTime= m_TimeOfNextInterval = i_Time;
  m_CurrentInterval = 0;
}

bool ObjBase::CheckIfNeedToMove(){
  #ifdef DEBUG
	Serial.println("entering check if need to move");
  #endif
  return (m_CurrentTime >= m_TimeOfNextInterval && m_CurrentInterval < m_NumOfIntervals);
}

bool ObjBase::CanBeWithOthers(){
  return m_CanBeWithOthers;
}

void ObjBase::myFadeToBackBy(byte i_Strip, byte i_From, byte i_NumLeds, byte i_By){
  byte by =(i_By*256.0)/100.0;
  
  for (byte i=0; i<i_NumLeds; i++){
	  if(i_From-i >=0)
		  m_Leds[c2d(i_Strip,i_From-i)].fadeToBlackBy(by);
    else return;
  }
}

void ObjBase::myFadeToFrontBy(byte i_Strip, byte i_From, byte i_NumLeds, byte i_By){
  byte by =(i_By*256.0)/100.0;
  
  for (byte i=0; i<i_NumLeds; i++){
    if(i_From+i < numOfLEDsInStrip[i_Strip])
      m_Leds[c2d(i_Strip,i_From+i)].fadeToBlackBy(by);
    else return;
  }
}

void ObjBase::myFillSolidEdge(byte i_Strip, byte i_Edge, CRGB i_Color){
  byte i;
  for(i=0;i<numOfLEDsInEdge[i_Strip][i_Edge];i++){
    m_Leds[StartingPointOfLEDInEdge[i_Strip][i_Edge]+i] = i_Color;
  }
}

void ObjBase::myFillSolidAllEdges(byte i_Edge, CRGB i_Color){
  byte i,j;
  for(i=0;i<NUM_OF_STRIPS;i++){
    for(j=0;j<numOfLEDsInEdge[i][i_Edge];j++){
      m_Leds[StartingPointOfLEDInEdge[i][i_Edge]+j] = i_Color;
    }
  }
}

void ObjBase::myFrontFillSolid(byte i_Strip, byte i_From, byte i_NumLeds, CRGB i_Color){
  byte i;
  for (i=0; i<i_NumLeds; i++){
    if(i_From+i < numOfLEDsInStrip[i_Strip])
      m_Leds[c2d(i_Strip,i_From+i)] = i_Color;
    else return;
  }
}

void ObjBase::myBackFillSolid(byte i_Strip, byte i_From, byte i_NumLeds, CRGB i_Color){
  byte i;
  for (i=0; i<i_NumLeds; i++){
    if(i_From-i >=0)
      m_Leds[c2d(i_Strip,i_From-i)] = i_Color;
    else return;
  }
}

void ObjBase::myFadeAllEdges(byte i_Edge, byte i_By){
    if(i_By<1)
    return;
  byte i;
  byte by =(i_By*256.0)/100.0;
  for(i=0;i<NUM_OF_STRIPS;i++){
      fadeToBlackBy(m_Leds+StartingPointOfLEDInEdge[i][i_Edge],numOfLEDsInEdge[i][i_Edge], by);
  }
}
void ObjBase::myFadeEdge(byte i_Strip, byte i_Edge, byte i_By){
  if(i_By<1)
    return;
  byte by =(i_By*256.0)/100.0;
  
  fadeToBlackBy(m_Leds+StartingPointOfLEDInEdge[i_Strip][i_Edge],numOfLEDsInEdge[i_Strip][i_Edge], by);
}
unsigned int ObjBase::c2d(byte i_Strip, byte i_LEDs){
	unsigned int res=0;
	
	res += StartingPointOfLEDInEdge[i_Strip][0];
	if(i_LEDs>0){
		res += i_LEDs;	
	}
	
	return res;
}
