#include "shell.h"
#include "cmsis_os.h"
#include "task.h"
#include "portable.h"

#include "utility.h"
#include "usart.h"
#include "motor.h"
#include "adc.h"
#include "guided.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void shellUsage(const char *p) 
{
	 printf("Usage: %s\r\n", p);
}

static void list_commands(const ShellCommand scp[]) 
{
    while (scp->sc_name != NULL) {
        printf("%s ", scp->sc_name);
        scp++;
    }
}

static void cmd_info(int argc, char *argv[]) 
{

  (void)argv;
  if (argc > 0) 
  {
    shellUsage("info");
    return;
  }

  printf("Build Date/Time: %s %s\r\n", __DATE__, __TIME__);
  printf("FreeRTOS Total Heap Size: %d\r\n", configTOTAL_HEAP_SIZE);
  printf("FreeRTOS Free Memory: %d\r\n", xPortGetFreeHeapSize());
	

}

static void cmd_tasks(int argc, char *argv[]) 
{
    char tmpbuff[512];

    (void)argv;
    if (argc > 0) 
    {
        shellUsage("info");
        return;
    }

    memset(tmpbuff, 0, sizeof(tmpbuff));
    /* The list of tasks and their status */
    vTaskList((char *)tmpbuff);		
    printf("Name          State  Priority  Stack   Num\r\n");
    printf("%s\r\n", tmpbuff);
}

static ShellCommand local_commands[] = {
  {"info", cmd_info},
  {"tasks", cmd_tasks},
  {"motor", cmd_motor},
  {"joystick", cmd_joystick},
  {"runguided", cmd_runguided},
  { NULL, NULL }
};

static int cmdexec(const ShellCommand *scp, 
        char *name, int argc, char *argv[]) 
{
    while (scp->sc_name != NULL) {
        if (strcasecmp(scp->sc_name, name) == 0) {
            scp->sc_func(argc, argv);
            return 0;
        }
        scp++;
    }
    return 1;
}

void shellStart(void const *p) 
{
    int n;
	  // TODO: Add a shell command extension, 
	  // for now set this to NULL
    const ShellCommand *scp = NULL;
    char *lp, *cmd, *tokp, line[SHELL_MAX_LINE_LENGTH];
    char *args[SHELL_MAX_ARGUMENTS + 1];


    if (p != NULL)
        scp = ((const ShellConfig *) p)->sc_commands;

    printf("\r\nWelcome to NYP Project\r\n");
		printf("Diagnostic/Debug Shell v1.0\r\n");
		printf("By Vergil Cola\r\n");
		printf("   Jerry Maturan\r\n");
    while (1) 
    {
        osThreadYield(); // Let other same priority task proceed to their run loop
        // if this thread is excuted first.

        printf(">> ");
        if (shellGetLine(line, sizeof(line))) 
            //if (scanf("%64s", line) == 0)
        {
            printf("\r\nlogout");
            break;
        }
        lp = _strtok(line, " \t", &tokp);
        cmd = lp;
        n = 0;
        while ((lp = _strtok(NULL, " \t", &tokp)) != NULL) {
            if (n >= SHELL_MAX_ARGUMENTS) {
                printf("too many arguments\r\n");
                cmd = NULL;
                break;
            }
            args[n++] = lp;
        }
        args[n] = NULL;
        if (cmd != NULL) {
            if (strcasecmp(cmd, "exit") == 0) {
                if (n > 0) {
                    shellUsage("exit");
                    continue;
                }
                // Break here breaks the outer loop
                // hence, we exit the shell.
                break;
            }
            else if (strcasecmp(cmd, "help") == 0) {
                if (n > 0) {
                    shellUsage("help");
                    continue;
                }
                printf("Commands: help exit ");
                list_commands(local_commands);
                if (scp != NULL)
                    list_commands(scp);
                printf("\r\n");
            }
            else if (cmdexec(local_commands, cmd, n, args) &&
                    ((scp == NULL) || cmdexec(scp, cmd, n, args))) {
                printf("%s", cmd);
                printf(" ?\r\n");
            }
        }

    }
}

/* Starts the shell in a separate thread */
//void shellThreadStart(const ShellConfig *args)
//{
//  osThreadDef(ShellThread, shellStart, osPriorityNormal, 0, configMINIMAL_STACK_SIZE + 128);
//  osThreadCreate (osThread(ShellThread), (void *) args);
//}

int shellGetLine(char *line, unsigned size) {
    char *p = line;

    while (1)
    {
        char c;
        int ret;

        if ((ret = Get_Char()) < 0)
            continue; // UART Busy

        c = ret;

        if (c == 4) {
            printf("^D");
            return 1;
        }
        if (c == 8) {
            if (p != line) {
                Send_Char(c);
                Send_Char(0x20);
                Send_Char(c);
                p--;
            }
            continue;
        }
        if (c == '\r') {
            Send_Char('\r');
            Send_Char('\n');
            *p = 0;
            return 0;
        }
        if (c < 0x20)
            continue;
        if (p < line + size - 1) {
            Send_Char(c);
            *p++ = (char)c;
        }
    }
}

void shellGoBackLine(int lineno)
{
	printf("\033[%dA", lineno);
}

void shellGotoXY(int x, int y)
{
	printf("\033[%d,%dH", x, y);
}

void shellClearScreen()
{
	printf("\033[2J");
}



