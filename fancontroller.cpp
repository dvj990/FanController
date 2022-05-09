/*
fancontroller.cpp authored by Dhiraj jadhav jadhav.dhiraj@gmail.com
last updated: May 8 2022
fancontroller.cpp implements functions in fancontroller.h and some local
*/

#include <iostream>
#include <thread>
#include <chrono>
#include "include/fancontroller.h"

using namespace std;

// Prototype and Declration
void setPWMValue(int address, uint8_t pwm_value);
uint8_t getPWMValue(float temperature);
float getTemperature();
std::atomic<bool> keepRunning{true};

/*
Function Name: Init
Arguments: User Inputs to configure the sussystem for robot
Output: txt on std out
Summary:
        functions to gather user inputs for populating hardware configuration
        can take  MAX_NUMBER_FAN 10 per Subsystem
        can take MAX_NUMBER_SUBSYSTEMS 10 {configurable in .h file}
        
*/
void Fan_Controller::Init()
{
    int subsystemCount = -1;
    int fanCount = -1;
    char userAnswerSub;
    char userAnswerFan;
    cout << "Welcome to the Fan Controller Demo" << std::endl;
    cout << "Please Enter Name of Robot" << std::endl;
    cin >> robot.robotName;
    do
    {
        cout << "Would you like to add New Subsystem? (y/n)" << endl;
        cin >> userAnswerSub;
        userAnswerSub = tolower(userAnswerSub);
        if (userAnswerSub == 'y')
        {
            subsystemCount++;
            robot.subsystemNumber[subsystemCount].subsystemName = "Temp" + to_string(subsystemCount);
            cout << "Setting Subsystem Name:  " << robot.subsystemNumber[subsystemCount].subsystemName << endl;
            fanCount = -1;
            do
            {
                cout << "Would you like to add New Fan? (y/n)" << endl;
                cin >> userAnswerFan;
                userAnswerFan = tolower(userAnswerFan);
                if (userAnswerFan == 'y')
                {
                    fanCount++;
                    robot.subsystemNumber[subsystemCount].fanNumber[fanCount].address = fanCount;
                    robot.subsystemNumber[subsystemCount].fanNumber[fanCount].number = fanCount;
                    cout << "setting Fan Number: " << fanCount << " \tFan Address: " << fanCount << endl;
                }
            } while (userAnswerFan != 'n');
        }
    } while (userAnswerSub != 'n');
    cout << "\n\nPrinting " << robot.robotName << " Subsystem Details:" << endl;
    subsystemCount = 0;
    while (robot.subsystemNumber[subsystemCount].subsystemName != "(NULL)" && subsystemCount < MAX_NUMBER_SUBSYSTEMS)
    {
        cout << "Subsystem Name:" << robot.subsystemNumber[subsystemCount].subsystemName << endl;
        fanCount = 0;
        while (robot.subsystemNumber[subsystemCount].fanNumber[fanCount].address != -1 && fanCount < MAX_NUMBER_FAN)
        {
            cout << "Fan Address: " << robot.subsystemNumber[subsystemCount].fanNumber[fanCount].address;
            cout << "\t Fan Number: " << robot.subsystemNumber[subsystemCount].fanNumber[fanCount].number << endl;
            fanCount++;
        }
        subsystemCount++;
    }
}

/*
Function Name: MaintainTemperature
Arguments: None
Output: void
Summary:
        functions will loop thorugh follwing
        loop(while told to keep alive)
            get maximum subsystem temperature
            calcualte PWM linear value
            set pam value
            repeat        
*/
void Fan_Controller::MaintainTemperature()
{
    float maxSubsystemTemperature = 0;
    uint8_t valuePWM = 0;
    while (keepRunning)
    {
        cout << "--------------------------------------------------------------------" << endl;
        maxSubsystemTemperature = GetSubsystemMaxTemperature();
        cout << "Max Subsystem Temperature: " << maxSubsystemTemperature << endl;
        valuePWM = getPWMValue(maxSubsystemTemperature);
        cout << "PWM Value : ";
        cout << (int)valuePWM << endl;
        cout << "Setting PWM value to all PWM registers" << endl;
        SetSubsystemMaxTemperature(valuePWM);
        this_thread::sleep_for(std::chrono::milliseconds(POLLING_FREQUENCY_MILISECONDS));
    }
}

float Fan_Controller::GetSubsystemMaxTemperature()
{
    int subsystemCount = 0;
    float maxTemperature = 0;
    float subsystemtemperature = 0;

    while (robot.subsystemNumber[subsystemCount].subsystemName != "(NULL)")
    {
        cout << "Get Temperature of Subsystem Name: " << robot.subsystemNumber[subsystemCount].subsystemName << " ";
        subsystemtemperature = getTemperature();
        cout << subsystemtemperature << " C" << endl;
        if (subsystemtemperature > maxTemperature)
        {
            maxTemperature = subsystemtemperature;
        }
        subsystemCount++;
    }
    return maxTemperature;
}

/*
Function Name: SetSubsystemMaxTemperature
Arguments: uint8_t pwm value
Output: void
Summary:
        Sets PWM value to all Fan's in subsystem's
*/
void Fan_Controller::SetSubsystemMaxTemperature(uint8_t valuePWM)
{
    int subsystemCount = 0;
    int fanCount = 0;

    while (robot.subsystemNumber[subsystemCount].subsystemName != "(NULL)" && subsystemCount < MAX_NUMBER_SUBSYSTEMS)
    {
        cout << "Subsystem Name:" << robot.subsystemNumber[subsystemCount].subsystemName << endl;
        fanCount = 0;
        while (robot.subsystemNumber[subsystemCount].fanNumber[fanCount].address != -1 && fanCount < MAX_NUMBER_FAN)
        {
            cout << "\t Fan Number: " << robot.subsystemNumber[subsystemCount].fanNumber[fanCount].number;
            setPWMValue(robot.subsystemNumber[subsystemCount].fanNumber[fanCount].address, valuePWM);

            fanCount++;
        }
        subsystemCount++;
    }
}


/*
Function Name: Setkeepalive
Arguments:bool state
Output: void
Summary:
        Sets KeepRunning variable for thread communication
*/
void Fan_Controller::Setkeepalive(bool state)
{
    keepRunning = state;
}

/************** Local Function Implementation*****************************/


/*
Function Name: getPWMValue
Arguments:float temperature
Output: uint8_t pwm value
Summary:
       finds the linear corelation for 8 bit PWM value
      2d Graph : x-axis PWM Value , Y-axis Temperature Values
      (x0,y0)= Starting point on 2d graph , (x1,y1)= End point on 2d graph
      (X,Y) = Point at with we want calcualte PWM Value for Given Temperature
      Linear Interpoaltion formula Y = [y0(x1-X)+y1(X-x0)]/(x1-x0)
*/
uint8_t getPWMValue(float temperature)
{
    float numeartor = ((MIN_PWM * (MAX_TEMPERATURE - temperature)) + (MAX_PWM * (temperature - MIN_TEMPERATURE)));
    float denomator = (MAX_TEMPERATURE - MIN_TEMPERATURE);
    return (uint8_t)(numeartor / denomator);
}


/*
Function Name: setPWMValue
Arguments: address of register, 8bit pwm value
Output: none
Summary:
       function to write the PWM value to address
*/
void setPWMValue(int address, uint8_t pwm_value)
{
    /*
    
    */
    cout << "Fan Address: " << address;
    cout << "\t PWM Value: " << (int)pwm_value << endl; // comented the cout to reduce the clutter on the terminal
}


/*
Function Name: getTemperature
Arguments: None
Output: float
Summary:
       frandom number generator as mock temperature sensor
*/
float getTemperature()
{
    return float(rand() % MAX_TEMPERATURE + 1);
}