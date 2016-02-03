/*#include "PG(que).h"
#include "device.h"
#include "PID_TIM5.h"

//#include "glob_val.h"
//#include "machine_info.h"
//#include "system.h"

volatile int  exec_que, current_que, que_count, finish_que, const_angv_counter=0;
unsigned long stop_time;
unsigned char soft_flag = 0;
unsigned char deaccel_flag = 0;
float         end_ang;
float         turn_angv, turn_ang, turn_angacc;
const float   turn_bef = (GyrateAngvel*GyrateAngvel)/(2.0f*GyrateAngacc);
float         last_obj_spd;
float         save_accel_len = 0.0f;
t_que         que[NUM_QUE];
t_slalom      *sla;


void Que(void){
    if(enables.que == 0){
        return;
    }

    display_setbin(COLOR_RED, 0);
    display_setbin(COLOR_GREEN, exec_que);

    //que finished?
    if(is_que_finish() == 1){
        finish_que = exec_que;
        exec_que++;
        set_nextque();
    }

    que_operation();

}


void que_operation(void){

    volatile float sq_speed,acc_len;

	switch(que[exec_que].mode){
		case straight:

			sq_speed = (((tm.speed * tm.speed)-(que[exec_que].speed * que[exec_que].speed)))/(2.0f*accel);
			acc_len = que[exec_que].prm-que[exec_que].n_prm;

			if(( acc_len <= sq_speed ) || (deaccel_flag == 1)){
				tm.accel = -accel;
				deaccel_flag = 1;
			}else{
				tm.accel = accel;
				if((save_accel_len >= que[exec_que].prm)){
					tm.accel = 0.0f;
				}
			}

			if(tm.speed > max_speed){
				tm.speed = max_speed;	//max
				tm.accel = 0.0f;
			}

			if((tm.speed < que[exec_que].speed) && (deaccel_flag == 1)){
				tm.accel = accel;
                deaccel_flag = 0;
			}


			que[exec_que].n_prm += 0.001f * tm.speed;

			break;

		case st_edge:

			if(que[exec_que+1].prm == 1.0){
			}


			if(tm.speed > last_obj_spd){
				tm.accel= -1.0f*accel;
				if(tm.speed <= last_obj_spd){
					tm.speed = last_obj_spd;
				}
			}else if(tm.speed < last_obj_spd){
				tm.accel= accel;
				if(tm.speed >= last_obj_spd){
					tm.speed = last_obj_spd;
				}
			}
			break;

		case slalom:

            if(deaccel_flag == 0){
                if(turn_angv < sla->angvel){
                    tm.angacc = que[exec_que].prm * sla->angacc;
                    turn_angv += 0.001f * sla->angacc;
                }else{
                    tm.angacc = 0.0f;
                    tm.angvel = que[exec_que].prm * sla->angvel;
                    const_angv_counter++;
                    if(const_angv_counter >= sla->const_angvel_time){
                        deaccel_flag = 1;
                    }
                } //   GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  //this part causes debug error
            }else{
                tm.angacc = -1.0f * que[exec_que].prm * sla->angacc;
                turn_angv -= 0.001f * sla->angacc;
            }

            turn_ang += 0.001f * turn_angv;

			break;

		case turn:

			if(deaccel_flag == 0){
				if(turn_angv < GyrateAngvel){
					turn_angacc = GyrateAngacc;
				}else{
                    turn_angacc = 0.0f;
					if(((90.0f * que[exec_que].speed) - turn_bef) < turn_ang){
						deaccel_flag = 1;
					}
				}
			}else{
                turn_angacc = -GyrateAngacc;
			}

            tm.angacc = que[exec_que].prm * turn_angacc;

            turn_angv += 0.001f * turn_angacc;
            turn_ang  += 0.001f * turn_angv;

			break;

		case stop:
            tm.angacc = tm.angvel = 0.0f;
            tm.accel = tm.speed = 0.0f;
			break;

		case wait_soft:
            tm.angacc = tm.angvel = 0.0f;
            tm.accel = tm.speed = 0.0f;
			break;

		case back:
			que[exec_que].n_prm += -0.001f*tm.speed;
            tm.angacc = 0.0f;
			break;
		case adj_front:
			tm.angacc = tm.angvel = 0.0f;
			break;
	}

}


void set_nextque(void){

    deaccel_flag = 0;
    que[exec_que].n_prm = 0.0f;

    switch(que[exec_que].mode){
        case straight:
            sensor_setenable(1);
            enable_con_wall = 1;

            last_obj_spd = que[exec_que].speed;
            tm.angvel = tm.angacc = 0.0f;
            tm.accel = accel;
            break;

        case st_edge:
            sensor_setenable(1);
            enable_con_wall = 1;
            if(que[exec_que].prm <= 0.0f){
                que[exec_que].prm = 0.1f;
            }
            tm.angvel = tm.angacc = 0.0f;
            break;

        case slalom:
            sensor_setenable(0);
            enable_con_wall = 0;
            que[exec_que].n_prm = 0.0f;
            sla = (t_slalom *)que[exec_que].ptr;
            tm.angacc = sla->angacc;
            deaccel_flag = 0;
            end_ang = tm.ang + (que[exec_que].prm * que[exec_que].speed * 45.0f);
            const_angv_counter = 0;
            turn_angv = 0.0f;
            turn_ang  = 0.0f;
            break;

        case turn:
            sensor_setenable(0);
            enable_con_wall = 0;
            turn_angv = 0.0f;
            turn_ang = 0.0f;
            end_ang = tm.ang + (que[exec_que].prm * que[exec_que].speed * 90.0f);
            break;

        case stop:
            sensor_setenable(0);
            enable_con_wall = 0;
            stop_time = timer_getms();
            tm.speed = 0.0;
            tm.angvel = tm.angacc = 0.0f;
            break;

        case wait_soft:
//            enables.control = 0;
            break;

        case back:
            break;

        case adj_front:
            stop_time = timer_getms();
            break;

    }

}


int is_que_finish(void){
    int next_que = 0;

    if(fail == 1){
        next_que = 1;
    }

	switch(que[exec_que].mode){
		case n_a:
			next_que = 1;
			break;

		case straight:
			if(que[exec_que].n_prm > que[exec_que].prm){
				next_que = 1;
                tm.accel = 0.0f;
                tm.speed = que[exec_que].speed;
			}
			break;

		case slalom:
			if( (deaccel_flag == 1) && (turn_angv <= 0.0f) ){
				next_que = 1;
				tm.ang = end_ang;
			}
			break;

		case turn:
			if( (deaccel_flag == 1) && (turn_angv <= 0.0f) ){
				next_que = 1;
                tm.ang = end_ang;
			}
			break;

		case stop:
			if(((unsigned long)stop_time+que[exec_que].prm) < timer_getms()){
				next_que = 1;
			}
			break;

		case wait_soft:
			next_que =0;
			if(soft_flag == 1){
				next_que = 1;
				soft_flag = 0;
			}
			break;

		case back:
			if(que[exec_que].n_prm > que[exec_que].prm){
				next_que = 1;
				tm.speed = 0.0;
			}
			break;

		case st_edge:
			if(que[exec_que].n_prm > que[exec_que].speed){
				next_que = 1;
			}
			break;

		case adj_front:
			if(((tm.speed < 5.0) && (tm.speed > -5.0) && ((rm.ang-tm.ang) < 2) && ((rm.ang-tm.ang) > -2)) || (((unsigned long)stop_time+1000) < timer_getms())){
				next_que = 1;
			}
			break;
	}

    return next_que;

}


void init_que(void){
	int i;

	exec_que = 0;
	que_count = 0;
	current_que = 0;
    deaccel_flag = 0;

	que[0].mode = wait_soft;
	for(i = 1; i < NUM_QUE; i++){
		que[i].mode = n_a;
		que[i].speed = 0;
		que[i].prm = 0.0;
		que[i].n_prm = 0.0;
		que[i].ptr = NULL;
	}

	add_que(wait_soft,0,0,NULL);

}



int add_que(q_mode mode, float prm, float speed, void *ptr){

	que[current_que].mode = mode;
	que[current_que].prm = prm;
	que[current_que].n_prm = 0;
	que[current_que].speed = speed;
	que[current_que].ptr = ptr;

	que_count++;
	current_que++;
	if(current_que == NUM_QUE){
		current_que = 0;
		return NUM_QUE-1;
	}
	if(mode == st_edge){
		if(prm <= 0.0){
			prm = 0.1;
		}
	}

	return current_que-1;
}


int add_len_que(float pm){
	que[current_que-1].prm += pm;
	return ;
}

int reset_speed_que(void){
    que[current_que-1].speed = 0.0f;
    return ;
}


int delete_que(int num){

	que[num].mode = n_a;
	return;
}
*/


