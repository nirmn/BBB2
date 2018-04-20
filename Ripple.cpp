#include "Ripple.h"



void Ripple::moove()
{  
  unsigned int i;
  int pVal;
  #ifdef DEBUG
    Serial.println("check-if-need-to-move return true.");
    Serial.println("entering moove");
    Serial.print("current interval: ");
    Serial.println(m_CurrentInterval);
    Serial.print("moove: m_StripNumber: ");
    Serial.println(m_StripNumber);
  #endif
//********************************************************************************************************
  
  for(i=0;i<numOfLEDsInStrip[m_StripNumber];i++){
      pVal = m_CurrentInterval*128/m_ItrPerCycle; //shift factor
      if(i>numOfLEDsInStrip[m_StripNumber]/2)
        pVal *= -1;
      
      pVal = map(i, 0,numOfLEDsInStrip[m_StripNumber]-1,-64, 196) + pVal;
      pVal = map(sin8(pVal),0,255,0,255-((255/(m_ItrPerCycle))*m_CycleCounter));

     
      m_Leds[c2d(m_StripNumber,i)] = CHSV( 100*(m_CycleCounter+1),255, pVal);
      /*
    /*
    if(i<(numOfLEDsInStrip[m_StripNumber]/2.0)-1)
      m_Leds[c2d(m_StripNumber,i)] += CHSV( 20,192, (i+1)*2*255/numOfLEDsInStrip[m_StripNumber]);
    else
      m_Leds[c2d(m_StripNumber,i)] += CHSV( 20,192, (numOfLEDsInStrip[m_StripNumber]-i)*2*255/numOfLEDsInStrip[m_StripNumber]);
  */
  }

    m_CurrentInterval += 1;
    if(m_CurrentInterval % m_ItrPerCycle == 0)
        m_CycleCounter ++;
    m_TimeOfNextInterval += m_TimeOfInterval;
//********************************************************************************************************
  #ifdef DEBUG
    Serial.print("moove: done mooving strip number: ");
    Serial.println(m_StripNumber);
    Serial.print("moove: m_Pos is: ");
    Serial.println(m_Pos);
    Serial.print("moove: m_currentInterval is: ");
    Serial.println(m_CurrentInterval);
  #endif
}

Ripple::Ripple(unsigned long i_Time, CRGB i_Leds[NUM_Of_LEDS], unsigned int i_InputSignal, byte i_Strip, CRGB i_Color ) : ObjBase(i_Time, i_Leds)
{ 
  m_InputSignal = i_InputSignal;
//  m_TimeOfInterval= 600 / (i_InputSignal/100);
  m_TimeOfInterval= 10;
  m_CanBeWithOthers = true;
  m_DefaultColor = i_Color;
  m_StripNumber = i_Strip;
 
  m_ItrPerCycle = 10; // how fast the fading will be/ 
  m_NumOfCycls = 4; // 1 cycle is half transfer
  m_NumOfIntervals = m_ItrPerCycle*m_NumOfCycls;
  m_FinalTime = m_StartingTime + m_TimeOfInterval*m_NumOfIntervals;
  m_CycleCounter = 0;
  #ifdef DEBUG 
    Serial.print("Ctor Ripple: i_StripNumber: ");
    Serial.println(m_StripNumber);
    Serial.print("Ctor Ripple: i_inputSinal: ");
    Serial.println(i_InputSignal);
    Serial.print("Ctor Ripple: time of interval: ");
    Serial.print(m_TimeOfInterval);
    Serial.println(" ms");
  #endif
}

void Ripple::Close(){
  #ifdef DEBUG
    Serial.print("Ripple: Close(): Closing Ripple; numLEDS:");
    Serial.print(numOfLEDsInStrip[m_StripNumber]);
    Serial.print(" Strip#: ");
    Serial.println(m_StripNumber);

  #endif
  myFrontFillSolid(m_StripNumber, 0, numOfLEDsInStrip[m_StripNumber], CRGB::Black);
        
  #ifdef DEBUG
    Serial.println("Ripple: Close(): After Closing Ripple");
  #endif
}

bool Ripple::checkDone()
{
  
  if (m_CurrentInterval == m_NumOfIntervals && m_FinalTime <= m_CurrentTime) {
    #ifdef DEBUG
    Serial.println("entering checkDone -> return done");
    #endif
    return true;
  }
  #ifdef DEBUG
    Serial.println("entering checkDone -> return not done");
    #endif
  return false;
}
