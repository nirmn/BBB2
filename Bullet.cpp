#include "Bullet.h"



void Bullet::moove()
{  
	#ifdef DEBUG
		Serial.println("check-if-need-to-move return true.");
		Serial.println("entering moove");
		Serial.print("current interval: ");
		Serial.println(m_CurrentInterval);
		Serial.print("moove: m_StripNumber: ");
		Serial.println(m_StripNumber);
	#endif
//********************************************************************************************************
    if(m_IsUp == true)
      myFadeToBackBy(m_StripNumber,m_Pos,m_TailSize, 25);
    else myFadeToFrontBy(m_StripNumber,m_Pos,m_TailSize, 25);
    //int pos = map(sin8(m_CurrentInterval),0,255,0,numOfLEDsInStrip[m_StripNumber]);//beatsin8(50,0,numOfLEDsInStrip[m_StripNumber],0,0);
    m_Leds[c2d(m_StripNumber,m_Pos)] += CHSV( m_Pos*2, 255, 192);
    
    m_CurrentInterval += 1;
    m_TimeOfNextInterval += m_TimeOfInterval;
    
    if (m_Pos == numOfLEDsInStrip[m_StripNumber]-1 && m_IsUp){
      m_IsUp = !m_IsUp;
      m_Pos--;
    }
    else if(m_Pos == 0&&!m_IsUp){
      m_IsUp = !m_IsUp;
      m_Pos++;
    }
    else if(m_IsUp)
      m_Pos++;
     else
        m_Pos--;   
//********************************************************************************************************
	#ifdef DEBUG
		Serial.print("moove: done mooving strip number: ");
		Serial.println(m_StripNumber);
		Serial.print("moove: next m_Pos is: ");
		Serial.println(m_Pos);
    Serial.print("moove: next Interval is: ");
    Serial.println(m_CurrentInterval);
        Serial.print("moove: next going to: ");
    Serial.println(m_IsUp);
	#endif
}

Bullet::Bullet(unsigned long i_Time, CRGB i_Leds[NUM_Of_LEDS], unsigned int i_InputSignal, byte i_StripNumber, CRGB i_Color ) : ObjBase(i_Time, i_Leds)
{ 
	m_InputSignal = i_InputSignal;
//	m_TimeOfInterval= 600 / (i_InputSignal/100);
	m_TimeOfInterval= 50;
	m_CanBeWithOthers = true;
	m_DefaultColor = i_Color;
	m_StripNumber = i_StripNumber;
	m_NumOfIntervals = numOfLEDsInStrip[m_StripNumber]*3;
	m_FinalTime = m_StartingTime + m_TimeOfInterval*m_NumOfIntervals;
	m_TailSize = 15;
  m_Pos = 0;
  m_IsUp = true;
	#ifdef DEBUG 
		Serial.print("Ctor Bullet: i_StripNumber: ");
		Serial.println(m_StripNumber);
		Serial.print("Ctor Bullet: i_inputSinal: ");
		Serial.println(i_InputSignal);
		Serial.print("Ctor Bullet: time of interval: ");
		Serial.print(m_TimeOfInterval);
		Serial.println(" ms");
	#endif
}

void Bullet::Close(){
	#ifdef DEBUG
		Serial.print("Bullet: Close(): Closing Bullet; numLEDS:");
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
	
    if(m_Pos < m_TailSize)    //if position is at the beginning
        myBackFillSolid( m_StripNumber, m_TailSize-1 ,m_TailSize, CRGB::Black);  //close first m_TailSize LEDs
    else if(numOfLEDsInStrip[m_StripNumber]-m_Pos < m_TailSize)
        myFrontFillSolid( m_StripNumber, numOfLEDsInStrip[m_StripNumber]-m_TailSize ,m_TailSize, CRGB::Black); //close last m_TailSize LEDs
    else if(m_IsUp==true)
        myBackFillSolid( m_StripNumber, m_Pos ,m_TailSize, CRGB::Black);
    else 
        myFrontFillSolid( m_StripNumber, m_Pos ,m_TailSize, CRGB::Black);
        
	#ifdef DEBUG
		Serial.println("Bullet: Close(): After Closing Bullet");
	#endif
}

bool Bullet::checkDone()
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
