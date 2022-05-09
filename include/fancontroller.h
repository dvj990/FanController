/*
fancontroller.h authored by Dhiraj jadhav jadhav.dhiraj@gmail.com 
last updated: May 8 2022
 used to declare functions and variables
 import int other cpp files.

*/

//this is a fancontroller.h file to export call and functinons
#ifndef FANCONTROLLER_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define FANCONTROLLER_H

#include <iostream>
#include <thread>
#define MAX_NUMBER_FAN 10
#define MAX_NUMBER_SUBSYSTEMS 10
#define MAX_TEMPERATURE 75
#define MIN_TEMPERATURE 20
#define MAX_PWM 255
#define MIN_PWM 0
#define POLLING_FREQUENCY_MILISECONDS 1000

class Fan_Controller 
{
    public:
        //init function
         void Init();
         void MaintainTemperature();
         float GetSubsystemMaxTemperature();
         void SetSubsystemMaxTemperature(uint8_t valuePWM);
         void Setkeepalive(bool state);
         Fan_Controller(){
             
         };
         ~Fan_Controller()
         {
             
         };
    private:    
        struct Fan
            {
                int number=-1 ;
                int address=-1;
            };
        struct Subsystem
            {
                std::string subsystemName="(NULL)";
                Fan fanNumber[MAX_NUMBER_FAN];
            };
        struct Robot
            {
                std::string robotName ;
                Subsystem subsystemNumber[MAX_NUMBER_SUBSYSTEMS];
            }robot;  
};
#endif
