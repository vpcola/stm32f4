#ifndef _TINY_SHELL_H_
#define _TINY_SHELL_H_

#ifdef __cplusplus
extern "C" {
#endif

#define SHELL_MAX_LINE_LENGTH       64
#define SHELL_MAX_ARGUMENTS         4

typedef void (*shellcmd_t) (int , char **);
typedef struct {
    const char * sc_name;
    shellcmd_t sc_func;
} ShellCommand;

typedef struct {
    ShellCommand * sc_commands;
} ShellConfig;

void shellStart(void const *p);
int  shellGetLine(char *line, unsigned size);
void shellUsage(const char *p);	
	
#ifdef __cplusplus
}
#endif

#endif
