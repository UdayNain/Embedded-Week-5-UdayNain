//STARTER 
//THIS EXERCISE IS TO BE DONE AND THEN RUN IN REAL LIFE ON A ROBOT
//YOU WILL NOT BE ABLE TO COMPILE THIS UNLESS YOU SET UP THE BUILD ENVIRONMENT
#include "main.h"

DigitalOut led(L26);
DigitalOut led2(L27);
DigitalOut led3(L25);

I2C i2c(I2C_SDA, I2C_SCL);

//DEFINE MOTORS, ETC

int main(){
    DJIMotor pitch(7, CANHandler::CANBUS_2, GIMBLY,"Peach");
    DJIMotor yaw(5, CANHandler::CANBUS_2, GIMBLY,"Yeah");
    DJImotor indexer(7,CANHandler::CANbus_2,C610,"Indexer");
    DJImotor RFLYWHEEL(8,CANHandler::CANbus_2,M3508,"RightFly");
    DJImotor LFLYWHEEL(5,CANHandler::CANbus_2,M3508,"LeftFly");
    int yawSetPoint = 0;
    int rightStickValueX = 0;
    int rightStickValueY = 0;
    int desiredPitch = 0;
    float FF = 0;
    //assigning can handler objects to motor class.
    DJIMotor::s_setCANHandlers(&canHandler1,&canHandler2, false, false); 

    //getting initial feedback.
    DJIMotor::s_getFeedback();

    unsigned long loopTimer_u = us_ticker_read();
    unsigned long timeEnd_u;
    unsigned long timeStart_u;

    int refLoop = 0;

    //DEFINE PIDs AND OTHER CONSTANTS

    while(true){ //main loop
        timeStart_u = us_ticker_read();

        //inner loop runs every 25ms
        if((timeStart_u - loopTimer_u) / 1000 > 25) { 
            loopTimer_u = timeStart_u;
            led = !led; //led blink tells us how fast the inner loop is running

            if (refLoop >= 5) { //ref code runs 5 of every inner loop, 
                refLoop = 0;
                refereeThread(&referee);
            }
            refLoop ++;

            remoteRead(); //reading data from remote

            //MAIN CODE 
            //YAW
            rightStickValueX = remote.rightX()/660;
            if(remote.rightX()>10 || remote.rightX()<-10){
                yawSetPoint -= rightStickValueX * JOYSTICK_SENSE_YAW;
            }
            yaw.setPosition(yawSetPoint);
            
            //PITCH
            Remote::SwitchState pitch_enabled = remote.rightSwitch();
            if(pitch_enabled == Remote::SwitchState::UP){
                rightStickValueY = remote.rightY()/660;
                desiredPitch -= rightStickValueY *JOYSTICK_SENSE_PITCH;
                if(desiredPitch>=LOWERBOUND){
                    desiredPitch = LOWERBOUND;
                }
                else if(desiredPitch<=UPPERBOUND){
                    desiredPitch=UPPERBOUND;
                }
                FF = k * sin((desiredPitch/180*PI)-pitch_phase);
                pitch.pidPosition.feedForward = int((INT16_T_MAX) * FF);
                pitch.setPosition(int((desiredPitch/360)*TICKS_REVOLUTION + InitialOffset_Ticks));
            }
            else{
                pitch.setPower(0);
            }
            
            //Shoot
            Remote::SwitchState leftSwitchValue = remote.leftSwitch();
            if(leftSwitchValue == Remote::SwitchState::UP){
                shootReady = false;
                shootTargetPosition = 8192 * 12 + (indexer>>MULTITURNANGLE);
                if(robot.status.shooter_barrel_heat_limit<10 || power_heat_datashooter_17mm_1_barrel_heat < robot_status.shooter_barrel_heat_limit - 40){
                    shoot = true;
                }
            }
            else{
                shootReady = true;
            }
            if(leftSwitchValue != Remote::SwitchState::DOWN || leftSwitchValue != Remote::SwitchState::UNKNOWN){
                RFLYWHEEL.setSpeed(7000);
                LFLYWHEEL.setSpeed(-7000);
            }
            else{
                RFLYWHEEL.setSpeed(0);
                LFLYWHEEL.setSpeed(0);
            }
            
            
            
            //MOST CODE DOESNT NEED TO RUN FASTER THAN EVERY 25ms

            timeEnd_u = us_ticker_read();

            DJIMotor::s_sendValues();
        }

        //FEEDBACK CODE DOES NEED TO RUN FASTER THAN 1MS
        //OTHER QUICK AND URGENT TASKS GO HERE

        DJIMotor::s_getFeedback();
        ThisThread::sleep_for(1ms);
    }
}
