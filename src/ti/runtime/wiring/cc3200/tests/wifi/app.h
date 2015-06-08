#ifndef app_h
#define app_h

/* AP sketch external declarations */
__extern void apSetup(void);
__extern void apLoop(void);

/* shell sketch external declarations */
__extern void shellSetup(void);
__extern void shellLoop(void);

/* main external declarations */
extern Char mainOutputBuf[];
#define MAIN_LED_PIN 13

#endif
