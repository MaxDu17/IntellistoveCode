#ifndef Rotary_h
#define Rotary_h

#include "Arduino.h"

class RotaryPoll
{
  public:
    RotaryPoll();
	void begin(int clk, int dt, int sw);
    int getDirection();
    bool isPushed();
	
  private:
	int _dt;
	int _clk; 
	int _sw; 
    long _curr;
	long _prev;
	int _turn;
	int _attinterkey; 
	volatile int _lastturn;
	int _lastpush; 
	int _single; 
	volatile bool _up; 
	void _detect(); 
	  
	
};

#endif