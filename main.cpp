#include <LinkedList.h>
#include "ObjBase.h"
#include "ObjFactory.h"
#include "Includes.h"

//********DEFINES*********
#define FIRST_LED_PIN 2
#define FIRST_DRUM_INPUT_PIN SNARE
#define FIRST_GUITAR_INPUT_PIN GREEN_PIN
#define MAX_NUM_OF_OBJECTS 20
#define DELAY_TIME 50
#define INPUT_THRESHOLD 80
#define INPUT_COMBO_THRESHOLD 300
#define WHAWHI_PIN_INPUT 12
#define TIME_BETWEEN_HITS 200
#define TIME_OF_ATTACK 100
#define NUM_OF_DRUM_INPUT_SAMPLES 2

#define LED_TYPE    WS2811
#define COLOR_ORDER RGB //BRG
#define BRIGHTNESS   96

//********GLOBAL VARIABLES*********


CRGB leds[NUM_Of_LEDS];
CLEDController *controllers[NUM_OF_STRIPS];

unsigned int analogInput, whawhiAnalogInput, i, loops;
unsigned long currentTime, lastTime;
byte inputDrumPinMap[NUM_OF_DRUM_INPUTS];     //where each input - in which pin
byte inputGuitarPinMap[NUM_OF_GUITAR_INPUTS]; //where each input - in which pin
unsigned int inputDrumHistoryMap[NUM_OF_DRUM_INPUTS];      //input history for each struck
byte inputGuitarHistoryMap[NUM_OF_GUITAR_INPUTS];   ////input history for each struck
bool inputCreatedObject[NUM_OF_DRUM_INPUTS];			      //flag for Object was created
bool inputFirstSignal[NUM_OF_DRUM_INPUTS];			        //flag for first signal
bool inputNeed2CreateObject[NUM_OF_DRUM_INPUTS];		    //flag to create an object
unsigned long inputTimeTable[NUM_OF_DRUM_INPUTS];	      //time of the first signal
bool isComboInside, wasInZero;
byte k;

LinkedList<ObjBase*> objectsList = LinkedList<ObjBase*>();   
ObjFactory factory(leds);
ObjBase* newObj, *currentObj; //pointers

//********FUNCTIONS DECLARATIONS*********
void removeObjectFromList(byte index);
void clearList();
void createNewObject(byte i_ItemToCreate, unsigned long i_Time, unsigned int i_InputSignal);
void updateObjects();
void clearDrumInputs();
//********FUNCTIONS IMPLEMENTATIONS*********
void setup() {
  delay(3000);
	Serial.begin(57600);
	
	#ifdef DEBUG
	Serial.println("entering reset");
	#endif
	//define strips:
  
  controllers[0] = &FastLED.addLeds<LED_TYPE,FIRST_LED_PIN,COLOR_ORDER>(leds, NUM_LEDS0).setCorrection(TypicalLEDStrip);
  controllers[1] = &FastLED.addLeds<LED_TYPE,FIRST_LED_PIN+1,COLOR_ORDER>(leds+NUM_LEDS0, NUM_LEDS1).setCorrection(TypicalLEDStrip);
  controllers[2] = &FastLED.addLeds<LED_TYPE,FIRST_LED_PIN+2,COLOR_ORDER>(leds+NUM_LEDS0+NUM_LEDS1, NUM_LEDS2).setCorrection(TypicalLEDStrip);
  controllers[3] = &FastLED.addLeds<LED_TYPE,FIRST_LED_PIN+3,COLOR_ORDER>(leds+NUM_LEDS0+NUM_LEDS1+NUM_LEDS2, NUM_LEDS3).setCorrection(TypicalLEDStrip);
  controllers[4] = &FastLED.addLeds<LED_TYPE,FIRST_LED_PIN+4,COLOR_ORDER>(leds+NUM_LEDS0+NUM_LEDS1+NUM_LEDS2+NUM_LEDS3, NUM_LEDS4).setCorrection(TypicalLEDStrip);
  controllers[5] = &FastLED.addLeds<LED_TYPE,FIRST_LED_PIN+5,COLOR_ORDER>(leds+NUM_LEDS0+NUM_LEDS1+NUM_LEDS2+NUM_LEDS3+NUM_LEDS4, NUM_LEDS5).setCorrection(TypicalLEDStrip);
  controllers[6] = &FastLED.addLeds<LED_TYPE,FIRST_LED_PIN+6,COLOR_ORDER>(leds+NUM_LEDS0+NUM_LEDS1+NUM_LEDS2+NUM_LEDS3+NUM_LEDS4+NUM_LEDS5, NUM_LEDS6).setCorrection(TypicalLEDStrip);
  controllers[7] = &FastLED.addLeds<LED_TYPE,FIRST_LED_PIN+7,COLOR_ORDER>(leds+NUM_LEDS0+NUM_LEDS1+NUM_LEDS2+NUM_LEDS3+NUM_LEDS4+NUM_LEDS5+NUM_LEDS6, NUM_LEDS7).setCorrection(TypicalLEDStrip);
  controllers[8] = &FastLED.addLeds<LED_TYPE,FIRST_LED_PIN+8,COLOR_ORDER>(leds+NUM_LEDS0+NUM_LEDS1+NUM_LEDS2+NUM_LEDS3+NUM_LEDS4+NUM_LEDS5+NUM_LEDS6+NUM_LEDS7, NUM_LEDS8).setCorrection(TypicalLEDStrip);
  controllers[9] = &FastLED.addLeds<LED_TYPE,FIRST_LED_PIN+9,COLOR_ORDER>(leds+NUM_LEDS0+NUM_LEDS1+NUM_LEDS2+NUM_LEDS3+NUM_LEDS4+NUM_LEDS5+NUM_LEDS6+NUM_LEDS7+NUM_LEDS8, NUM_LEDS9).setCorrection(TypicalLEDStrip);
  controllers[10] = &FastLED.addLeds<LED_TYPE,FIRST_LED_PIN+10,COLOR_ORDER>(leds+NUM_LEDS0+NUM_LEDS1+NUM_LEDS2+NUM_LEDS3+NUM_LEDS4+NUM_LEDS5+NUM_LEDS6+NUM_LEDS7+NUM_LEDS8+NUM_LEDS9, NUM_LEDS10).setCorrection(TypicalLEDStrip);
  
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(12,10000); // limit power draw to 10A at 12v

	// Turn off all LEDs 
  FastLED.clear();

	// Drum setup:
	for(i=0; i < NUM_OF_DRUM_INPUTS; i++){
		inputDrumPinMap[i] = FIRST_DRUM_INPUT_PIN + i; //set input pin for each instrument
		inputDrumHistoryMap[i]=0;          //set analog input zero for each instrument
		inputTimeTable[i]=0;
		inputCreatedObject[i]=false;
		inputFirstSignal[i]=true;
		inputNeed2CreateObject[i] = true;
	}
    // Guitar setup:
    for(i=0; i < NUM_OF_GUITAR_INPUTS-1; i++){
		inputGuitarPinMap[i] = FIRST_GUITAR_INPUT_PIN + i; //set input pin for each instrument
		inputGuitarHistoryMap[i]=LOW;          //set digital input zero for each instrument
	}
	//WHAWHI setup
	{
		inputGuitarPinMap[WHAWHI] = WHAWHI_PIN_INPUT; //set input pin for each instrument
		inputGuitarHistoryMap[WHAWHI]=LOW;          //set digital input zero for each instrument
	}
	

	isComboInside = false;
	wasInZero = false;
  lastTime=0;
  
	FastLED.show();
//	clearList();// free and clear list
  
  #ifdef DEBUG
  Serial.println("reset done.");
  #endif
}

void loop() {
	#ifndef DEBUG
	  //Serial.println("new main loop, time");
	  showfps();
	#endif
	
	whawhiAnalogInput = analogRead(inputGuitarPinMap[WHAWHI]);
	  #ifdef DEBUG
	  if(digitalRead(inputGuitarPinMap[UP])==HIGH){
      Serial.println("UP");
	  }
    if(digitalRead(28)==HIGH){
      Serial.println("DOWN");
    }
    #endif

    if(isComboInside == false){
	    //******GET ALL INPUT SIGNALS FOR GUITAR*****
		  #ifdef DEBUG
		    Serial.println("main loop: combo is not running, taking current time and checking if was in zero..");
		  #endif
		  currentTime = millis();
		  if(digitalRead(inputGuitarPinMap[DOWN])==LOW && digitalRead(inputGuitarPinMap[UP])==LOW){
			  wasInZero=true;
			  inputGuitarHistoryMap[UP]=LOW;
			  inputGuitarHistoryMap[DOWN]=LOW;
			  #ifdef DEBUG
			    Serial.println("Guitar: was in zero.");
			  #endif
		  }
		  if(wasInZero==true){
			  //get over all the guitar inputs
			  #ifdef DEBUG
			    Serial.println("Guitar: was in zero, reading inputs");
			  #endif
			  for(i=0; i< NUM_OF_GUITAR_INPUTS-1; i++){
				  inputGuitarHistoryMap[i] = digitalRead(inputGuitarPinMap[i]);
			  }
		  }
		  #ifdef DEBUG
		    Serial.println("main loop: done reading guitar inputs, moving reading drum inputs");
		  #endif		
		  //Check All Drum Inputs
		  for(byte j=0; j < NUM_OF_DRUM_INPUT_SAMPLES; j++){
			  for(i=0; i< NUM_OF_DRUM_INPUTS; i++){
				  analogInput = analogRead(inputDrumPinMap[i]);
				  if(analogInput < INPUT_THRESHOLD){
					  inputNeed2CreateObject[i] = false;
					  continue;	//go to next input
				  }
				  // the input is above threshold
				  //check if the current input is the first input, if so take time
				  if(inputFirstSignal[i] == true){
					  //first time signal, record time
					  inputFirstSignal[i] = false;	//next signal will not be the first signal
					  inputTimeTable[i] = currentTime;
					  inputNeed2CreateObject[i] = true;
					  inputCreatedObject[i] = false;
				  }
				  else if(currentTime - inputTimeTable[i] > TIME_OF_ATTACK){
					  //not first time and out of time bounds, reset the input
					  inputTimeTable[i] = currentTime;
					  inputCreatedObject[i] = false;
					  inputNeed2CreateObject[i] = true;
					  inputDrumHistoryMap[i] = analogInput;
				  }
				  else if(inputCreatedObject[i] == true){
					  //in cap time, not first signal and object was created
					  inputNeed2CreateObject[i] = false;
				  }
				  else {
					  inputNeed2CreateObject[i] = true;
				  }
				  // check if need to create an object 
				  if(inputNeed2CreateObject[i] == true){
					  //signal is good, object was not created, inside time cap, first or not first signal
					  //record signal
					  if(inputDrumHistoryMap[i] <= analogInput){
						  // input rising
						  inputDrumHistoryMap[i] = analogInput;
					  }
				  }//end need to create an object
			  }//end loop over all drum inputs
		  }//end drum sampling loop
	  }//end combo check - end reading inputs
	
	//********** Done reading all inputs, Drums and guitar.
	      #ifdef DEBUG
          Serial.println("main loop: done reading all inputs");
        #endif
	
	
	//******CHECK FOR DRUM COMBOS*****
	//combo 1:
	if(inputNeed2CreateObject[TOM1] && inputNeed2CreateObject[TOM2]){ 
		if(inputDrumHistoryMap[TOM1] > INPUT_COMBO_THRESHOLD && inputDrumHistoryMap[TOM2] > INPUT_COMBO_THRESHOLD && !isComboInside){
			//do combo1
			#ifdef DEBUG
			  Serial.println("combo1 was activated");
			#endif
			isComboInside = true;
			clearList();
			createNewObject(COMBO1, inputTimeTable[TOM1], inputDrumHistoryMap[TOM1]);
			clearDrumInputs();
			inputNeed2CreateObject[TOM1] = false;
			inputCreatedObject[TOM1] = true;
			inputNeed2CreateObject[TOM2] = false;
			inputCreatedObject[TOM2] = true;
		}
	}
  
	//******CHECK IF COMBO IS INSIDE*****
	if((isComboInside == true && objectsList.get(0)->CanBeWithOthers()) || objectsList.size() == 0){
		isComboInside = false;
		#ifdef DEBUG
		  Serial.println("main loop: combo is not running or wasn't activated");
		#endif
	}
	//******Analyze ALL INPUTS SIGNAL*****
	#ifdef DEBUG
    Serial.println("main loop: now analyzing all inputs");
    #endif
	if(isComboInside == false){
		//	Analyze Drum inputs
		for(  i=0; i< NUM_OF_DRUM_INPUTS; i++){
			if(inputNeed2CreateObject[i] == true){
				#ifdef DEBUG
				  Serial.println("Drum: while searching for drum inputs, input was found");
				#endif        
				createNewObject(i, inputTimeTable[i], inputDrumHistoryMap[i]);
				inputNeed2CreateObject[i] = false;
				inputCreatedObject[i] = true;
			}
		}
	
		//Analyze guitar inputs
		if(inputGuitarHistoryMap[UP]==HIGH){
			for(i=0; i<NUM_OF_GUITAR_INPUTS-3; i++){
				if(inputGuitarHistoryMap[i]==HIGH){
					#ifdef DEBUG
					  Serial.print("Guitar: while searching input, input was found: ");
					  Serial.print(i+IGREEN);
					  Serial.println("Pin num + UP. now creating new object.");
					#endif
					createNewObject(i+IGREEN, currentTime, 1023);
					wasInZero=false;
				}
			}
		}
		else if(inputGuitarHistoryMap[DOWN]==HIGH){
			for(i=0; i<NUM_OF_GUITAR_INPUTS-3; i++){
				if(inputGuitarHistoryMap[i]==HIGH){
					#ifdef DEBUG
					  Serial.print("Guitar: while searching input, input was found: ");
					  Serial.print(i+IGREEN2);
					  Serial.println("virtual Pin num + DOWN. now creating new object.");
					#endif
					createNewObject(i+IGREEN2, currentTime, 1023);
					wasInZero=false;
				}
			}
		}
	}
 //********************
 //simulating guitar input every 5 seconds
/*
 if(currentTime-lastTime>2000){
  createNewObject(temp+IGREEN2, currentTime, 1023);
  lastTime=currentTime;
  Serial.print(temp);
  temp += 1; 
  if(temp==5)
    {temp=0;}
    Serial.print("=>");
    Serial.println(temp);
 }
*/
 
 //********************
	//******************************************************************************************************
	//******UPDATE POSITIONS OF ALL OBJECTS*****
	updateObjects();
  
	//********SHOW LEDs ********
	#ifdef DEBUG
	Serial.println("done moving all objects, now showing");
	#endif
	controllers[0]->showLeds();

	}

void removeObjectFromList(byte i_Index){
  #ifdef DEBUG
  Serial.print("removeObjectFromList: removing obj#: ");
  Serial.println(i_Index);
  #endif
  ObjBase* currentObject = objectsList.get(i_Index);
  
  currentObject->Close();
  delete(currentObject);
  objectsList.remove(i_Index);
    #ifdef DEBUG
  Serial.print("removeObjectFromList: done removing obj#: ");
  Serial.println(i_Index);
  #endif
}

void clearList(){
	while(objectsList.size()){
		removeObjectFromList(0);
	}
	objectsList.clear();
}

void createNewObject(byte i_ItemToCreate, unsigned long i_Time, unsigned int i_InputSignal){
	//*****CREATE NEW OBJECT******
	//i_ItemToCreate: ISNARE/IGREEN/IGREEN2
	#ifdef DEBUG
	Serial.println("createNewObject: creating new obj");
  Serial.print("inside createNewObject: i_ItempToCreate: ");
  Serial.println(i_ItemToCreate);
	#endif
	newObj = factory.MakeNewObject(i_ItemToCreate, i_Time, i_InputSignal);  // returns address of new obj
    #ifdef DEBUG
    Serial.print("createNewObject: list size before adding item: ");
    Serial.println(objectsList.size());
    Serial.println("createNewObject: obj was created");
    #endif
	//*********CHECK LIST SIZE TO PUSH THE OBJECT****** 
	if(objectsList.size() == MAX_NUM_OF_OBJECTS){  //check if there is space for new object
		//delete the old, insert the new
		#ifdef DEBUG
		Serial.println("createNewObject: not enough space, deleting object");
		Serial.print("createNewObject: before delete list size is: ");
		Serial.println(objectsList.size());
		#endif
		removeObjectFromList(0); //here we need to remove the one that gonna die first, should be the first in the list
          
		#ifdef DEBUG
		//Serial.println("object was deleted");
		Serial.print("createNewObject: list size is: ");
		Serial.println(objectsList.size());
		#endif
	}
	//*********ADD TO LIST*************** 
	//search where to insert and than insert
	unsigned long newObjTOD = newObj->FinalTime();// time of death for the new object
	for(byte i = 0; i < objectsList.size(); i++){
		currentObj = objectsList.get(i);
		if(currentObj->FinalTime() >=  newObjTOD ){
			objectsList.add(i,newObj);
			#ifdef DEBUG
			Serial.println("createNewObject: object was added");
			#endif
			return;
		}
	}
	objectsList.add(newObj);//if a place was not found the object will be insert to the end.
	#ifdef DEBUG
	Serial.println("createNewObject: object was added");
	#endif
}

void updateObjects(){
    currentTime = millis();
    for(k = 0; k < objectsList.size(); k++){
		currentObj = objectsList.get(k);
		#ifdef DEBUG
		Serial.println("updateObjects: before trying an object");
		#endif
		if(currentObj->TryToMove(currentTime) == false){
			//object done moving, need to be removed
			#ifdef DEBUG
			Serial.print("updateObjects: object needs to be removed. obj #");
       Serial.println(k);
			#endif
			removeObjectFromList(k);
			k -= 1;
			#ifdef DEBUG
			Serial.println("updateObjects: object was removed");
			#endif
		}
	}
}

void clearDrumInputs(){
	for(k=0; k < NUM_OF_DRUM_INPUTS; k++){
		inputDrumHistoryMap[k]=0;          //set analog input zero for each instrument
	}
}

void showfps(){
  currentTime = millis();
  loops++;
  if(currentTime - lastTime > 1000){
    Serial.println(loops);
    lastTime = currentTime;
    loops = 0;
    
  }
}

