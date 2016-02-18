#ifndef __GUIDED_H__
#define __GUIDED_H__

#ifdef __cplusplus
extern "C" {
#endif
	
typedef struct
{
    int rduty;
    int lduty;
} MotorDuty;


MotorDuty * getMotorDuty(int x, int y);
void cmd_runguided(int argc, char *argv[]); 
void cmd_runauto(int argc, char * argv[]);
	
#ifdef __cplusplus
}
#endif
#endif

