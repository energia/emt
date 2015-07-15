#ifndef app_h
#define app_h

#include <L3G.h>
#include <LSM303.h>

/* imu sketch external declarations */
extern LSM303 imuCompass; /* acceleration and magnetometer */
extern L3G    imuGyro;    /* gyro data */
__extern void imuSetup(void);
__extern void imuLoop(void);

/* motor sketch external declarations */
extern char motorWASD;
__extern void motorSetup(void);
__extern void motorLoop(void);

/* AP sketch external declarations */
__extern void apSetup(void);
__extern void apLoop(void);

/* shell sketch external declarations */
__extern void shellSetup(void);
__extern void shellLoop(void);

/* main external declarations */
extern Char mainOutputBuf[];
#define MAIN_LED_PIN RED_LED

#endif
