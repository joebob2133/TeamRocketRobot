/*
 * File:   Demo2.c
 * Author: TeamRocket
 *
 * Created on October 14, 2021, 12:40 PM
 */


#include "xc.h"
#pragma config FNOSC = FRC
#define CW 0
#define CCW 1
#define READY 1
#define WORKING 0

int steps = 0;
int endSteps = 0;
int state = READY;

void __attribute__((interrupt, no_auto_psv)) _OC1Interrupt(void)
{   
    _OC1IF = 0;  // Clear interrupt flag

    if (steps < endSteps) {                   // Forward
        steps++;
    }
    else{           // Pause
        config_pwm_OC1(0,7999);
        steps = 0;
        state = READY;
        _LATA2 = 1;
    }
    
}

void straightLineMovement (int direction1, int direction2, int speed, int rotations){
    config_pwm_OC1(speed / 2 + 1,speed);
    _LATA0 = direction1;
    _LATA1 = direction2;
//    if(direction == CW){
//        _LATA2 = CW;
//    }
//    else{
//        _LATA2 = CCW;
//    }
    endSteps = rotations * 200;
}

void config_pwm_OC1 (int duty, int period) {
    //-----------------------------------------------------------
    // CONFIGURE PWM1 USING OC1 (on pin 14)
    
    // Set period and duty cycle
    // CHOOSE PERIOD AND DUTY CYCLE HERE
    OC1R = duty;
    OC1RS = period;

    // Clear control bits
    OC1CON1 = 0;
    OC1CON2 = 0;
    
    // Configure OC1
    OC1CON1bits.OCTSEL = 0b111;
    OC1CON2bits.SYNCSEL = 0x1F; 
    OC1CON2bits.OCTRIG = 0;     
    OC1CON1bits.OCM = 0b110;

}


int main(void) {
    // Configure PORTB for digital input (disable analog input)
    ANSB = 0x000;
    
    // Configure all bits of PORTA as outputs
    TRISA = 0x000000;

    // Configure all bits of PORTB as inputs
    TRISB = 0xFFFF;

    config_pwm_OC1(4000,7999);
    
//    _LATA0 = CW;
//    _LATA1 = CW;
    
    _OC1IP = 4;
    _OC1IE = 1;
    _OC1IF = 0;
    
    // debug
    _LATA2 = 0;
    
    float instructions[3][4] = { 
        {CW, CCW, 7999, 4} ,   /*  initializers for row indexed by 0 */
        {CW, CW, 7999, 1.45} ,   /*  initializers for row indexed by 1 */
        {CW, CCW, 7999, 3} 
    };
    int instructionCount = 0;
    while(1){
        if(state == READY && instructionCount < 3){
            //if (){
              //              _LATA2 = 1;
            //}
            state = WORKING;
            straightLineMovement(instructions[instructionCount][0], instructions[instructionCount][1], instructions[instructionCount][2], instructions[instructionCount][3]);
            instructionCount++;
        }
    }
    return 0;
}