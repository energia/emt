/*
 *  ======== motorLoop ========
 *  This sketch controls the motors on the Zumo by polling a global
 *  variable, motorWASD, once per PERIOD milliseconds for one of the
 *  following motor commands:
 *      'w' - drive forward
 *      's' - drive backward
 *      'a' - turn left
 *      'd' - turn right
 *      ' ' - stop
 *
 *  Other sketches or interrupts can control the zumo by simply writing the
 *  desired command to motorWASD.
 */
#include <Energia.h>

#include "ZumoMotors.h"

#include "app.h"

#define PERIOD  1           /* period of motor control updates */

char motorWASD = ' ';       /* current motor drive command */

static ZumoMotors motors;   /* Zumo motor driver provided by Pololu */

static int clip(int speed);
static void drive(char wasd, int goal, unsigned int duration);

/*
 *  ======== motorSetup ========
 */
__extern void motorSetup(void)
{
    Serial.println("motorSetup ...");
    /* initialize the Pololu driver motor library */
    motors.setRightSpeed(0);
    motors.setLeftSpeed(0);

    /* setup an LED to indcate forward/backward movement or turning */
    pinMode(MAIN_LED_PIN, OUTPUT);
    Serial.println("motorSetup done.");
}

/*
 *  ======== motorLoop ========
 */
__extern void motorLoop(void)
{
    /* state used to blink LED */
    static unsigned count = 0;
    static char state = 0;
    
    switch (motorWASD) {
        case 's':
        case 'w': {
            /* illuminate LED while driving */
            digitalWrite(MAIN_LED_PIN, HIGH);
            drive(motorWASD, 200, PERIOD);
            break;
        }

        case 'd':
        case 'a': {
            /* blink LED while turning */
            if (count == ((count / 100) * 100)) {
                state ^= 1;
                digitalWrite(MAIN_LED_PIN, state);
            }   
            drive(motorWASD, 100, PERIOD);
            break;
        }

        default: {
            /* turn off LED while stopped */
            motorWASD = ' ';
            digitalWrite(MAIN_LED_PIN, LOW);
            drive(' ', 0, 10);
            break;
        }
    }

    count++;
}

/*
 *  ======== clip ========
 */
static int clip(int speed)
{
    if (speed < -400) {
        speed = -400;
    }
    else if (speed > 400) {
        speed = 400;
    }
    return (speed);
}

/*
 *  ======== drive ========
 */
#define P 0.9375f
static void drive(char wasd, int goal, unsigned int duration)
{
    static int leftSpeed = 0;
    static int rightSpeed = 0;
    
    while (duration > 0) {
        duration--;

        /* gradually adjust curent speeds to goal */
        switch (wasd) {
            case ' ': { /* stop */
                leftSpeed = (int)(P * leftSpeed);
                rightSpeed = (int)(P * rightSpeed);
                break;
            }
            case 'w': { /* forward */
                leftSpeed = (leftSpeed - goal) * P + goal;
                rightSpeed = (rightSpeed - goal) * P + goal;
                break;
            }

            case 's': { /* backward */
                leftSpeed = (leftSpeed + goal) * P - goal;
                rightSpeed = (rightSpeed + goal) * P - goal;
                break;
            }

            case 'd': { /* turn right */
                leftSpeed = (leftSpeed - goal) * P + goal;
                rightSpeed = (rightSpeed + goal) * P - goal;
                break;
            }

            case 'a': { /* turn left */
                leftSpeed = (leftSpeed + goal) * P - goal;
                rightSpeed = (rightSpeed - goal) * P + goal;
                break;
            }

            default: {
                break;
            }
        }

        /* clip speeds to allowable range */
        leftSpeed = clip(leftSpeed);
        rightSpeed = clip(rightSpeed);
    
        /* set motor speeds */
        motors.setLeftSpeed(leftSpeed);
        motors.setRightSpeed(rightSpeed);

        /* sleep for 1 ms (so duration is in milliseconds) */
        delay(1);
    }
}

