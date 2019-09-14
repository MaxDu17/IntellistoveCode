#ifndef Rotary_h
#define Rotary_h

#include "Arduino.h"

class Rotary
{
  public:
    Rotary();
	void begin(int clk, int dt, int sw);
	void manualdt(int attdt);
    int getDirection();
    bool isPushed();
	void detect(); 
	int getPin(); 
  private:
	int _dt;
	int _clk; 
	int _sw; 
    int _curr;
	int _prev;
	int _turn;
	int _attinterkey; 
	volatile int _lastturn;
	int _lastpush; 
	int _single; 
	volatile bool _up; 
	  
	
};

#endif