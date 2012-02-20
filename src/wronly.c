/* wronly.c
 * Write Only Programmer
 * 2006-06-10 by t.kuroki
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "avrspx.h"
//----------------------------------------------------------------------------
bool  f_write_only_programmer;
int   cmdbuf_count;
BYTE *cmdbuf;
static int cmdbuf_size;
//----------------------------------------------------------------------------
int wronly_universal_command(BYTE cmd1, BYTE cmd2,BYTE cmd3, BYTE cmd4)
{
	if (cmdbuf_count >= cmdbuf_size)
	{
		if (!cmdbuf_size)
		{
			cmdbuf_size = 32768;
			cmdbuf = malloc(cmdbuf_size);
		}
		else
		{
			cmdbuf_size <<= 1;
			cmdbuf = realloc(cmdbuf, cmdbuf_size);
		}
	}
	cmdbuf[cmdbuf_count++] = cmd1;
	cmdbuf[cmdbuf_count++] = cmd2;
	cmdbuf[cmdbuf_count++] = cmd3;
	cmdbuf[cmdbuf_count++] = cmd4;
//fprintf(stdout, "%02X %02X %02X %02X\n", cmd1, cmd2, cmd3, cmd4);
	return 0;
}
//----------------------------------------------------------------------------
void wronly_programmer_init(void)
{
	f_write_only_programmer = true;
	cmdbuf_count = cmdbuf_size = 0;
}

void wronly_programmer_close(void)
{
	if (cmdbuf != NULL)
	{
		free(cmdbuf);
		cmdbuf = NULL;
	}
}
//----------------------------------------------------------------------------
