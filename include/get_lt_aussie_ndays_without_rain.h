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
#define NDIMS 2

#define STRING_LENGTH 2000
#define TRUE 1
#define FALSE 0

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

typedef struct  {
    char   fdir[STRING_LENGTH];
    char   var_name[STRING_LENGTH];
    int    window;
    int    start_yr;
    int    end_yr;
} control;

void clparser(int, char **, control *);
int  is_leap_year(int);
void read_nc_file_into_array(control *, char *, int,
                             float nc_in[MAX_DAYS][NLAT][NLON]);
void write_nc_file(char *,  float [NLAT][NLON]);
int  days_in_a_month(int, int, int *);
void get_input_filename(control *, int, int, int, char *, char *, char *);

void calculate_dry_spells(control *, int ,
                          float data_in[MAX_DAYS][NLAT][NLON],
                          float *, float *, int *, int*);
void calculate_avg_dry_spells_over_all_years(control *, float *, float *,
                                             int *, int *);

#endif /* GET_LT_AUSSIE_TMAX_H */
