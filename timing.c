#include <sys/time.h>

#include "config.h"
#include "builtins.h"
#include "shell.h"
#include "common.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

/*
#define INIT_DYNAMIC_VAR(var, val, gfunc, afunc) \
	do { \
		SHELL_VAR *v = bind_variable(var, (val), 0); \
		v->dynamic_value = gfunc; \
		v->assign_func = afunc; \
	} while (0)
INIT_DYNAMIC_VAR("MYPID", NULL, get_mypid, assign_mypid);
*/

static SHELL_VAR *
timeofday_assign(SHELL_VAR *self, char *value, arrayind_t unused, char *key)
{
	return (self);
}

static SHELL_VAR *
timeofday_get(SHELL_VAR *var)
{
	char *p;
	struct timeval tv = { 0 };

	if (gettimeofday(&tv, NULL) < 0) {
		builtin_error("Failed to get time of day: %m");
		return NULL;
	}

	if (asprintf(&p, "%ld%06ld", tv.tv_sec, tv.tv_usec) < 0) {
		builtin_error("Failed to get memory for time of day: %m");
		return NULL;
	}

	FREE(value_cell(var));

	VSETATTR(var, att_integer);
	var_setvalue(var, p);

	return var;
}

int
enable_timeofday_builtin(WORD_LIST *list)
{
	SHELL_VAR *v = bind_variable("TIMEOFDAY", NULL, 0);

	v->dynamic_value = timeofday_get;
	v->assign_func   = timeofday_assign;

	return 0;
}

char *enable_timeofday_doc[] = {
	"Enable $TIMEOFDAY.",
	"",
	"Enables use of the ${TIMEOFDAY} dynamic variable.",
	"It will yield the current microseconds.",
	0
};

struct builtin enable_timeofday_struct = {
	"enable_timeofday",
	enable_timeofday_builtin,
	BUILTIN_ENABLED,
	enable_timeofday_doc,
	"enable_timeofday",
	0
};
