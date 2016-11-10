#ifndef GET_LT_AUSSIE_TMAX_H
#define GET_LT_AUSSIE_TMAX_H

#include <stdlib.h>
#include <stdio.h>
#include <netcdf.h>
#include <strings.h>
#include <string.h>

#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}
#define NTIM 1
#define NLAT 3474
#define NLON 4110
#define MAX_DAYS 91
#define NYRS 42

#define STRING_LENGTH 2000
#define TRUE 1
#define FALSE 0

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

typedef struct  {
    char   fdir[STRING_LENGTH];
    char   var_name[STRING_LENGTH];
    int    window;
} control;

void clparser(int, char **, control *);
int  is_leap_year(int);

#endif /* GET_LT_AUSSIE_TMAX_H */
