#include "cuppa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void __attribute__ ((noreturn)) die (const char *fmt, ...) {
    fprintf (stderr, "%s: fatal: ", cuppa_argv0);

    va_list ap;
    va_start (ap, fmt);
    vfprintf (stderr, fmt, ap);
    fprintf (stderr, "\n");
    va_end (ap);
    exit (1);
}

/* Free an array of options. This function is needed because the name of an argument is allocated. */
void cuppa_opt_arr_free (CuppaOption *opts, int argc) {
    for (int i = 0; i < argc - 1; i++) {
        if (opts[i].which == OPT_NAMED) {
            free (opts[i].named.name);
        } else {
            free (opts[i].positional.value);
        }
    }

    free (opts);
}

static char *opt_split_arg (const char *arg, const char **value) {
    if (!arg || !value) {
        die ("opt_split_arg(): invalid arguments");
    }

    // NOTE: we expect arg to not contain -'s.
    const char *v = NULL;
    const char *a = arg;
    char *argname;

    while (*a) {
        if (*a == ' ' || *a == '=') {
            v       = a + 1;
            argname = calloc (sizeof (char), v - arg);
            memcpy (argname, arg, v - arg - 1);
            argname[v - arg] = 0;
            break;
        }

        a++;
    }

    if (v == NULL) {
        argname = strdup (arg);
    }

    *value = v;
    return argname;
}

/* Get options from command line. */
CuppaOption *cuppa_get_opts (int argc, char *argv[]) {
    argc -= 1;
    argv += 1;
    CuppaOption *arr = calloc (sizeof (CuppaOption), argc);

    for (int i = 0; i < argc; i++) {
        const char *arg = argv[i];
        if (*arg != '-') {
            CuppaOption pos           = {0};
            pos.which            = OPT_POS;
            pos.positional.idx   = i;
            pos.positional.value = strdup (arg);
            printf ("val: %s\n", pos.positional.value);
            arr[i] = pos;
        } else {
            CuppaOption named = {0};
            arg++;
            named.which          = OPT_NAMED;
            named.named.is_short = *arg != '-';
            if (!named.named.is_short) {
                arg++;
            }

            named.named.name = opt_split_arg (arg, &named.named.res);
            arr[i]           = named;
        }
    }

    return arr;
}

int cuppa_handle_opts (CuppaOption *arr, int argc,
                         char *(*handler) (const char *, const char *, bool,
                                           int) ) {
    char *err;
    for (int i = 0; i < argc - 1; i++) {
        int idx = i - 1;
        if (arr[i].which == OPT_NAMED) {
            err = handler (arr[i].named.name, arr[i].named.res,
                           !arr[i].named.is_short, idx);
        } else {
            err = handler (NULL, arr[i].positional.value, false, idx);
        }

        if (err) {
            fprintf (stderr, "Error: %s\n", err);
            free (err);
            return 2;
        }
    }

    return 0;
}
