# FanController

Demo.cpp authored by Dhiraj jadhav jadhav.dhiraj@gmail.com
last updated: May 6 2022
Input: None
Output: fan speed PWM values on STD out

How to use <folder>$ ./demo [no arguments]
  
What it does?
Name of robot:
Main{  
  loop:
      add Subsystem
      loop:
            add Fan 
            return
      return
  create thread maintaintemperature
  loop (intill keepalive is set to false)
      read subsystem temperature
      calculate highest value
      calculate PWM value
   return
Note :fan PWm values will be calculated for 10 sec
Program will terminate gracefully.