/*
 * (C) Copyright 2014 Google, Inc
 * Simon Glass <sjg@chromium.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CLI_H
#define __CLI_H

#define DEBUG_PARSER			0	/* set to 1 to debug */

#define CONFIG_SYS_MAXARGS		64	/* max command args */
#define CONFIG_SYS_CBSIZE		512	/* Console I/O Buffer Size */
#define CONFIG_SYS_PROMPT		"# "

char * strcpy(char * dest,const char *src);
unsigned long simple_strtoul(const char *cp, char **endp, unsigned int base);
int strict_strtoul(const char *cp, unsigned int base, unsigned long *res);
unsigned long long simple_strtoull(const char *cp, char **endp, unsigned int base);

int cli_simple_run_command(const char *cmd, int flag);
int cli_readline(const char *const prompt);
int cli_readline_into_buffer(const char *const prompt, char *buffer,
				int timeout);
int cli_simple_parse_line(char *line, char *argv[]);

#define endtick(seconds) (get_ticks() + (uint64_t)(seconds) * get_tbclk())

#endif