/*
  RotaryPoll.cpp - Library for basic rotary encoder needs.
  Created by Maximilian Du, May 27, 2017
*/

#include "Arduino.h"
#include "RotaryPoll.h"

RotaryPoll::RotaryPoll()
{
}
void RotaryPoll::begin(int clk, int dt, int sw)
{
	_clk = clk; 
	_dt = dt; //note: dt has to be either on pin 2 or 3 for it to work// 
	_sw = sw;
	_turn = 0;
	_single = 0; 
	_prev = millis(); //sets timestamp PREVIOUS to the current up time of the program// 
	pinMode(_clk, INPUT); // This is the input on the arduino for clk//
	pinMode(_dt, INPUT); // This is the input on the arduino for dt//
	pinMode(_sw, INPUT_PULLUP);
	
	delay(100); 
	

}


int RotaryPoll::getDirection()
{
 volatile int temp; 
 
   
     if( digitalRead(_dt) == 0)
	 {
		_detect(); 
		if(_up) // if up is true, it means its turned counter-clockwise, since the phase is offset to the left//
		{

		_curr = millis(); 
			if (_curr - _prev > 100) //this is bounce protections// //change back to 100 after// 
			{
				_lastturn = 2;   
				_prev = millis(); //sets prev timestamp to current up time//
			}
			else
			{
				_lastturn = 0; //this means that the rotary has bounced, so store no turn//
			}

		}
     
      else if (!_up) // if up is false, it means its turned clockwise, since the phase is offset to the left//
      {
        _curr = millis(); 
        if (_curr - _prev > 100)
        {
          _lastturn = 1;
          _prev = millis(); 
        }
        else
        {
			_lastturn = 0;       
        }
      } 
	temp = _lastturn;
	_lastturn = 0;
	return temp;
	 }
  
  
	return 0;
}

bool RotaryPoll::isPushed()
{
	bool temp;
  if(digitalRead(_sw) == LOW)
  { 
	if(_single == 0)
	{
		_lastpush = true;
		_single = 1; //prevents from registering multiple times while held down//
	}
	else
	{
		_lastpush = false; 
	}
	  
  }
  else
  {
	  _lastpush = false; 
	  _single = 0; //if button is released, reset variable.//
	  
  }
  
	temp = _lastpush;
	_lastpush = false;
	return temp; 
	
	
  
}
void RotaryPoll::_detect()
{
	_up = (digitalRead(_clk) == digitalRead(_dt)); // if both things match, up is true, which declares the turn direction//
}

