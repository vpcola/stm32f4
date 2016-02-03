
#ifndef __QUE__

#define __QUE__


#define NULL ((int *)0x00000000)
#define change_que(p)	que[current_que].prm=p
#define NUM_QUE (200)




typedef struct{
	float x;
	float y;
}vector;




typedef enum{
	n_a,
	straight,
	slalom,
	turn,
	stop,
	wait_soft,
	back,
	st_edge,
	adj_front,
}q_mode;



typedef struct{
	q_mode	mode;
	void	*ptr;
	float	speed;
    float	prm;
	float	n_prm;
}t_que;



typedef struct{
	unsigned short	step;
	float			deg_sum;
	vector			vec;
	float			len1;
	float			len2;
}t_sla;



extern t_que que[NUM_QUE];
extern volatile int exec_que;
extern unsigned char soft_flag;

void init_que(void);
int add_que(q_mode mode, float prm, float speed, void *sla);
int add_len_que(float pm);
int delete_que(int num);
void Que(void);
void set_nextque(void);
void que_operation(void);
int reset_speed_que(void);

#else

#endif

