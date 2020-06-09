#include <Arduino.h>

/* 
This is a class to help read input from a rotary encoder and two buttons, 
plexed to a single analog input via a voltage ladder.

rotary button values:
1 - resting: 1023
2, 3, 4 - rotate CW: 550, then 622, then 715
4, 3, 2 - rotate CCW: 715, then 622, then 550
5 - center button: 25
6 - back button: 350

direction is 0 for stationary, -1 for CCW, 1 for CW 
*/

enum RotaryReturn{none, Up, Down, Button1, Button2};

class Rotary
{
private:
    int inputPin;
    byte state;
    byte direction;

    int val1;
    int val2;
    int val3;
    int val4;
    int val5;
    int val6;

    byte range;

public:
    Rotary(uint8_t input, int v1, int v2, int v3, int v4, int v5, int v6, int valueRange)
    {
        inputPin = input;
        val1 = v1;
        val2 = v2;
        val3 = v3;
        val4 = v3;
        val5 = v4;
        val6 = v5;
        range = valueRange;
    }

    RotaryReturn check()
    {
        int rotaryValue = analogRead(inputPin);
        if (state == 1)
        {
            if (closeEnough(rotaryValue, val2, range)) // is the rotary encoder being turned CW?
            {
                direction = 0;
                state = 2;
            }
            else if (closeEnough(rotaryValue, val3, range)) // is the rotary encoder being turned CCW?
            {
                direction = 0;
                state = 3;
            }
            else if (closeEnough(rotaryValue, val4, range)) // is the rotary encoder being turned CCW?
            {
                direction = 0;
                state = 4;
            }
            else if (closeEnough(rotaryValue, val5, range)) // is the center button being pressed?
            {
                state = 4;
                return Button1;
            }
            else if (closeEnough(rotaryValue, val6, range)) // is the back button being pressed?
            {
                state = 6;
                return Button2;
            }
        }
        else if (state == 2)
        {
            if (closeEnough(rotaryValue, val1, range)) // did the rotary encoder finish the step?
            {
                state = 1;
                if (direction == 1)
                {
                    return CW;
                }
                else if (direction == -1)
                {
                    return CCW;
                }
            }
            else if (closeEnough(rotaryValue, val3, range)) // is the rotary encoder being turned CW?
            {
                state = 3;
                direction = 1;
            }
            else if (closeEnough(rotaryValue, val4, range)) // is the rotary encoder being turned CW?
            {
                state = 4;
                direction = 1;
            }
        }
        else if (state == 3)
        {
            if (closeEnough(rotaryValue, val1, range)) // did the rotary encoder finish the step?
            {
                state = 1;
                if (direction == 1)
                {
                    return CW;
                }
                else if (direction == -1)
                {
                    return CCW;
                }
            }
            else if (closeEnough(rotaryValue, val2, range)) // is the rotary encoder being turned CCW?
            {
                state = 2;
                direction = -1;
            }
            else if (closeEnough(rotaryValue, val4, range)) // is the rotary encoder being turned CW?
            {
                state = 4;
                direction = 1;
            }
        }
        else if (state == 4)
        {
            if (closeEnough(rotaryValue, val1, range)) // did the rotary encoder finish the step?
            {
                state = 1;
                if (direction == 1)
                {
                    return CW;
                }
                else if (direction == -1)
                {
                    return CCW;
                }
            }
            else if (closeEnough(rotaryValue, val2, range)) // is the rotary encoder being turned CCW?
            {
                state = 2;
                direction = -1;
            }
            else if (closeEnough(rotaryValue, val3, range)) // is the rotary encoder being turned CCW?
            {
                state = 3;
                direction = -1;
            }
        }
        else if (state == 5 || state == 6)
        {
            if (closeEnough(rotaryValue, val1, range)) // was the button released?
            {
                state = 1;
            }
        }
        return none;
    }
};

bool closeEnough(int a, int b, int range){
  if (abs(a - b) <= range)
  {
    return true;
  }
  else
  {
    return false;
  }
}
