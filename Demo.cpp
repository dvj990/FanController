/*
Demo.cpp authored by Dhiraj jadhav jadhav.dhiraj@gmail.com
last updated: May 6 2022
Input: None
Output: fan speed PWM values on STD out

How to use <folder>$ ./demo [no arguments]
Name of robot:
loop:
   add Subsystem
      loop:
         add Fan
         return
    return
create thread maintaintemprature
   loop (intill keepalive is set to false)
      read subsystem temperature
      calcualte highest value
      calculate PWM value
      return
Note :fan PWm values will be calcualted for 10 sec
Program will terminate grasfully can will terminate

*/

#include <iostream>
#include <thread>
#include "include/fancontroller.h"

using namespace std;

/*
Function Name: main
Arguments: None
Output: txt on std out
Summary:
         main function to Demo.cpp used to demonstrate the fancontroll libary
*/
int main()
{
   Fan_Controller Darvin1;
   Darvin1.Init();
   thread th(&Fan_Controller::MaintainTemprature, Darvin1);
   cout << "wait for 10 seconds" << endl;
   this_thread::sleep_for(std::chrono::milliseconds(POLLING_FREQUENCY_MILISECONDS * 10));
   Darvin1.Setkeepalive(false);

   th.join();
   return EXIT_SUCCESS;
}