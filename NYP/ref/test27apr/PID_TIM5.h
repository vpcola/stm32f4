

typedef struct{
    volatile float error;
    volatile float old_error;
    volatile float sum;
    volatile float kp;
    volatile float ki;
    volatile float kd;
    volatile float control;
}t_control;


t_control con_ang, con_spd_l,con_spd_r, con_wall;
extern volatile int enable_con_wall;


void Control(void);
void calc_PID(t_control *c, float e);
void init_PIDparameters(void);
void update_machine(void);
void fail_detect(void);
void calc_wall_control(void);
void calc_angular_control(void);
void flush_Control(void);
