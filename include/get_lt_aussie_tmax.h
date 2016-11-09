#ifndef GET_LT_AUSSIE_TMAX_H
#define GET_LT_AUSSIE_TMAX_H

#include <stdlib.h>
#include <stdio.h>
#include <netcdf.h>

#define FILE_NAME "eMAST_ANUClimate_day_tmax_v1m0_20000101.nc"
#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}
#define NTIM 1
#define NLAT 3474
#define NLON 4110

#define STRING_LENGTH 2000
#define TRUE 1
#define FALSE 0

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

int is_leap_year(int);

#endif /* GET_LT_AUSSIE_TMAX_H */
