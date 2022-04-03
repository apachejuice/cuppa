/*
   Copyright 2022 apachejuice

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef __cuppa_h__
#define __cuppa_h__
#include <stdbool.h>

// major version
#define CUPPA_MAJOR 0x01
// minor version
#define CUPPA_MINOR 0x00
// version hex
#define CUPPA_VERSION 0x0100
// version string
#define CUPPA_VERSIONSTR "Cuppa 1.0"
#define CUPPA_AUTHOR "apachejuice"
#define CUPPA_BUGREPORT "https://github.com/apachejuice/cuppa/issues"

// set this to `argv[0]`.
static const char *cuppa_argv0;

#define OPT_POS   1  // positional argument
#define OPT_NAMED 2  //named argument

typedef struct Option {
    union {
        struct {
            char *name;  // for the option '--foo-bar' this is 'foo-bar'
            int is_short;  // nonzero if the option consists of one character and thus only uses one dash
            const char *res;  // passed arg if such exists
        } named;
        struct {
            char *value;  // argument value
            int idx;      // index in argv
        } positional;
    };

    int which;  // OPT_POS / OPT_NAMED
} CuppaOption;

/* Free an array of options. This function is needed because the name of an argument is allocated. */
void cuppa_opt_arr_free (CuppaOption *opts, int argc);
/* Get options from command line. */
CuppaOption *cuppa_get_opts (int argc, char *argv[]);
// Run the handler function on each commandline option. The arguments to the handler function are as follows:
// - a string containing the name of the argument (NULL) in positional arguments
// - a string containing the value of the argument
// - whether or not a named argument has two dashes
// - the index of the argument in argv.
// The return value shall be an error message or NULL if everything is fine.
int cuppa_handle_opts (CuppaOption *arr, int argc,
                         char *(*handler) (const char *, const char *, bool,
                                           int) );

#endif
