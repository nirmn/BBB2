#include "LightEdge.h"



void LightEdge::moove()
{  
  byte i;
  #ifdef DEBUG
    Serial.println("check-if-need-to-move return true.");
    Serial.println("entering moove");
    Serial.print("current interval: ");
    Serial.println(m_CurrentInterval);
    Serial.print("moove: m_StripNumber: ");
    Serial.println(m_StripNumber);
  #endif
//********************************************************************************************************

      myFadeAllEdges(m_Edge, 30);
      myFillSolidEdge(m_Pos, m_Edge, CHSV( m_Pos*5, 255, 192));
    
    m_CurrentInterval += 1;
    m_TimeOfNextInterval += m_TimeOfInterval;
    
    if (m_Pos == NUM_OF_STRIPS-1 && !m_IsUp){
      m_IsUp = !m_IsUp;
      m_Pos--;
    }
    else if(m_Pos == 0&&m_IsUp){
      m_IsUp = !m_IsUp;
      m_Pos++;
    }
    else if(!m_IsUp)
      m_Pos++;
     else
        m_Pos--;  


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

LightEdge::LightEdge(unsigned long i_Time, CRGB i_Leds[NUM_Of_LEDS], unsigned int i_InputSignal, byte i_Edge, CRGB i_Color ) : ObjBase(i_Time, i_Leds)
{ 
  m_InputSignal = i_InputSignal;
//  m_TimeOfInterval= 600 / (i_InputSignal/100);
  m_TimeOfInterval= 50;
  m_CanBeWithOthers = true;
  m_DefaultColor = i_Color;
  m_StripNumber = 0;
  m_Edge = i_Edge;
  m_NumOfIntervals = NUM_OF_STRIPS*3;
  m_FinalTime = m_StartingTime + m_TimeOfInterval*m_NumOfIntervals;
  m_TailSize = 3;
  m_Pos = 0;
  m_IsUp = false;// starting down
  #ifdef DEBUG 
    Serial.print("Ctor LightEdge: i_StripNumber: ");
    Serial.println(m_StripNumber);
    Serial.print("Ctor LightEdge: i_inputSinal: ");
    Serial.println(i_InputSignal);
    Serial.print("Ctor LightEdge: time of interval: ");
    Serial.print(m_TimeOfInterval);
    Serial.println(" ms");
  #endif
}

void LightEdge::Close(){
  #ifdef DEBUG
    Serial.print("LightEdge: Close(): Closing LightEdge; numLEDS:");
    Serial.print(numOfLEDsInStrip[m_StripNumber]);
    Serial.print(" Strip#: ");
    Serial.println(m_StripNumber);
    Serial.print(" m_Pos: ");
    Serial.println(m_Pos);
    Serial.print(" m_IsUp: ");
    Serial.println(m_IsUp);
        Serial.print(" m_Tail: ");
    Serial.println(m_TailSize);
  #endif
  
  myFillSolidAllEdges(m_Edge, CRGB::Black);
        
  #ifdef DEBUG
    Serial.println("LightEdge: Close(): After Closing LightEdge");
  #endif
}

bool LightEdge::checkDone()
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
