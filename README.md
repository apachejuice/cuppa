# Cuppa
Cuppa is a very simple option parser for C. It builds with `-Wall -Wextra -Werror`, and functions very similarly to the classic `getopt()` function. The main API consists of 3 functions, a constant and a structure.

### `static const char *cuppa_argv0`
Set this to `argv[0]` before calling any other cuppa functions. Its value is used in error messages.

### `typedef struct Option CuppaOption`
Defined as
```c
typedef struct {
    union {
        struct {
            char *name; 
            int is_short;
            const char *res;
        } named;
        struct {
            char *value;
            int idx;
        } positional;
    };

    int which;
} Option;
```
The first union member, `named` contains information of a named argument: its name, whether it is a short argument or not and the value it holds (if any). The second union member contains the value an index of a positional argument. The second member of the main struct holds the information on whether this option is positional or named. These values are all determined during the parsing process.

### `void cuppa_opt_arr_free (CuppaOption *opts, int argc)`
This function frees an array of options `opts` ensuring that memory does not leak. Pass the `argc` in the main method *unchanged* to this function.

### `CuppaOption *cuppa_get_opts (int argc, char *argv[])`
Runs the option parser. Pass `argc` and `argv` from the main method. After running `cuppa_handle_opts`, free the return value of this function with `cuppa_opt_arr_free`.

### `int cuppa_handle_opts (CuppaOption *arr, int argc, char *(*handler) (const char *, const char *, bool, int))`
The heart of Cuppa. The first argument should be the return value from `cuppa_get_opts`, the second one `argc` from the main method and the last one a callback function that handles the option parsing itself. The arguments to the handler function are as follows:
- a string containing the name of the argument (NULL) in positional arguments
- a string containing the value of the argument
- whether or not a named argument has two dashes
- the index of the argument in argv.

The return value shall be an error message or NULL if everything is fine.

<sub>Cuppa 1.0 by apachejuice</sub>
