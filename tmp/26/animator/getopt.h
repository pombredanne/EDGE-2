#pragma once

#define getopt xgetopt
#define optarg xoptarg
#define optind xoptind

extern char *optarg; /* Global argument pointer. */
extern int optind;   /* Global argv index. */

int
getopt(int argc, char *argv[], char *optstring);

