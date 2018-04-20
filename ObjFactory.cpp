#include "ObjFactory.h"
//#include "BulletBW.h"
//#include "Flash.h"
#include "Bullet.h"
#include "LightEdge.h"
#include "Ripple.h"

#define color 0,0,20

ObjBase* ObjFactory::MakeNewObject(byte i_ItemToCreate, unsigned long i_Time, unsigned int i_InputSignal)
{
	#ifndef DEBUG
		Serial.print("*****factory: i_ItemToCreate: ");
		Serial.println(i_ItemToCreate);
		Serial.print("factory: i_Time: ");
		Serial.println(i_Time);
		Serial.print("factory: i_InputSignal: ");
		Serial.println(i_InputSignal);
   #endif
    
	ObjBase* newObject = 0;
    m_SpeedFactor=1;




    switch (i_ItemToCreate) {
        case IGREEN: //********** UP STROCKES:   ***********************
			newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 10,CRGB(color));
			m_History[IGREEN-ICF] += 1;
		break;
        case IRED:
			newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 10,CRGB(color));
			m_History[IRED-ICF] += 1;
		break;
        case IYELLOW:
			newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 0,CRGB(color));
			m_History[IYELLOW-ICF] += 1;
		break;
        case IBLUE:
			newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 0,CRGB(color));
			m_History[IBLUE-ICF] += 1;
		break;
        case IORANGE:
			newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 0,CRGB(color));
			m_History[IORANGE-ICF] += 1;
		break;
        
        //********** DOWN STROCKES:   ***********************
       
        case IGREEN2:
			newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 0,CRGB(color));
			m_History[IGREEN2-ICF2] += 1;
		break;
        case IRED2:
			newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 0,CRGB(color));
			m_History[IRED2-ICF2] += 1;
		break;
        case IYELLOW2:
			newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 0,CRGB(color));
			m_History[IYELLOW2-ICF2] += 1;
		break;
        case IBLUE2:
            newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 0,CRGB(color));
			m_History[IBLUE2-ICF2] += 1;
		break;
        case IORANGE2:
            newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 0,CRGB(color));
			m_History[IORANGE2-ICF2] += 1;
		break;
		  
		  //********** DRUMS:   ***********************
		
        case ISNARE:
			    newObject = new Ripple(i_Time, m_Leds, i_InputSignal, 10,CRGB(color));
			    //newObject = new LightEdge(i_Time, m_Leds, i_InputSignal, 0,CRGB(color));
          //newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 7,CRGB(color));
			    m_History[ISNARE] += 1;
        break;
        case IHIHAT:
            newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 9,CRGB(color));
			m_History[ISNARE] += 1;
		break;
        case ICRASH:
            newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 8,CRGB(color));
          m_History[ICRASH] += 1;
		break;
        case ITOM1:
            newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 7,CRGB(color));
            m_History[ITOM1] += 1;
		break;
        case IRIDE:
            newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 6,CRGB(color));
            m_History[IRIDE] += 1;
		break;
		case IFLOAR:
            newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 9,CRGB(color));
            m_History[IFLOAR] += 1;
		break;
        case ITOM2:
            newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 8,CRGB(color));
            m_History[ITOM2] += 1;
		break;
        case IBASS:
            newObject = new Bullet(i_Time, m_Leds, i_InputSignal, 7,CRGB(color));
            m_History[IBASS] += 1;
		break;
    }
    return newObject;
}

ObjFactory::ObjFactory(CRGB i_Leds[])
{
  m_Leds = i_Leds;
  m_Counter=0;
  m_Strip = 0;
}
