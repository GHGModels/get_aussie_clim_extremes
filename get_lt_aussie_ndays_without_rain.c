//
//
// PROGRAM:
//              get_lt_aussie_ndays_without_rain
//
// DESCRIPTION:
//              Using the eMAST PPT data estimate: (i) the number of days
//              without PPT for each pixel across the continent
//
// AUTHOR:      Martin De Kauwe
//
// EMAIL:       mdekauwe@gmail.com
//
// DATE:        2nd March, 2017
//

#include "get_lt_aussie_ndays_without_rain.h"

int main(int argc, char **argv) {

    int   rr, cc, yr, ndays, days_in_mth;
    int   mth_id, day, nmonths = 3, nday_idx;
    long  offset;
    char  imth[3];
    char  iday[3];
    char  infname[STRING_LENGTH];
    char  ofname1[STRING_LENGTH];
    char  ofname2[STRING_LENGTH];

    // Need to be declared like this otherwise the netcdf read will attempt to
    // use the heap rather than the stack to allocate memory and run out.
    // I'm sure there is a way to get netcdf to read into a 1D array but I
    // don't know how to do that and can't find a quick example
    // NB. I'm declaring 1 extra spot for leap years, so we will need to make
    // sure when we read from this array we are checking leap years.
    static float data_in[MAX_DAYS][NLAT][NLON];
    static float nc_data_out1[NLAT][NLON];
    static float nc_data_out2[NLAT][NLON];
    float       *data_out = NULL;
    float       *data_out2 = NULL;
    int         *cnt_all_yrs = NULL;
    int         *cnt_all_yrs2 = NULL;

    // allocate some memory
    control *c;
    c = (control *)malloc(sizeof (control));
    if (c == NULL) {
        fprintf(stderr, "control structure: Not allocated enough memory!\n");
        exit(EXIT_FAILURE);
    }

    if ((data_out = (float *)calloc(NLAT*NLON, sizeof(float))) == NULL) {
        fprintf(stderr, "Error allocating space for data_out array\n");
        exit(EXIT_FAILURE);
    }

    if ((data_out2 = (float *)calloc(NLAT*NLON, sizeof(float))) == NULL) {
        fprintf(stderr, "Error allocating space for data_out2 array\n");
        exit(EXIT_FAILURE);
    }

    if ((cnt_all_yrs = (int *)calloc(NLAT*NLON, sizeof(int))) == NULL) {
        fprintf(stderr, "Error allocating space for cnt_all_yrs array\n");
        exit(EXIT_FAILURE);
    }

    if ((cnt_all_yrs2 = (int *)calloc(NLAT*NLON, sizeof(int))) == NULL) {
        fprintf(stderr, "Error allocating space for cnt_all_yrs2 array\n");
        exit(EXIT_FAILURE);
    }

    // Initial values, these can be changed on the cmd line
    strcpy(c->fdir, "/Users/mdekauwe/Downloads/emast_data");
    strcpy(c->var_name, "air_temperature");
    c->window = 5;
    c->start_yr = 1970;
    c->end_yr = 1971;

    clparser(argc, argv, c);

    // off we go
    for (yr = c->start_yr; yr < c->end_yr; yr++) {
        printf("%d\n", yr);
        ndays = 0;
        nday_idx = 0;
        for(mth_id = 0; mth_id < nmonths; mth_id++) {
            days_in_mth = days_in_a_month(yr, mth_id, &ndays);
            for (day = 1; day <= days_in_mth; day++) {
                get_input_filename(c, day, mth_id, yr, iday, imth, infname);
                read_nc_file_into_array(c, infname, nday_idx, data_in);
                nday_idx++;
            }  // Day in month loop
        } // mth loop

        calculate_dry_spells(c, ndays, data_in, &(*data_out), &(*data_out2),
                             &(*cnt_all_yrs), &(*cnt_all_yrs2));
    } // yr loop
    calculate_avg_dry_spells_over_all_years(c, &(*data_out), &(*data_out2),
                                            &(*cnt_all_yrs), &(*cnt_all_yrs2));

    // Write data to two netcdf files.

    // Copy data into netcdf structured array. I'm sure i don't need to do this
    // but I'm not sure how to pass 1D array to netcdf write! Pain for now
    for (rr = 0; rr < NLAT; rr++) {
        for (cc = 0; cc < NLON; cc++) {
            offset = rr * NLON + cc;
            nc_data_out1[rr][cc] = data_out[offset];
            nc_data_out2[rr][cc] = data_out2[offset];
        }
    }

    sprintf(ofname1, "%d_avg_dry_spell.nc", c->window);
    write_nc_file(ofname1, nc_data_out1);

    sprintf(ofname2, "%d_longest_dry_spell.nc", c->window);
    write_nc_file(ofname2, nc_data_out2);

    free(data_out);
    free(data_out2);
    free(cnt_all_yrs);
    free(cnt_all_yrs2);
    free(c);

    return(EXIT_SUCCESS);

}

int days_in_a_month(int yr, int mth_id, int *ndays) {
    // Figure out the days in a month and increment the number of number of days
    // in a year counter

    int days_in_mth;

    if (mth_id == 0) {
        *ndays += 31;
        days_in_mth = 31;
    } else if (mth_id == 1) {
        *ndays += 31;
        days_in_mth = 31;
    } else if (mth_id == 2) {
        if (is_leap_year(yr+1)) {
            *ndays += 29;
            days_in_mth = 29;
        } else {
            *ndays += 28;
            days_in_mth = 28;
        }
    }

    return (days_in_mth);
}

void get_input_filename(control *c, int day, int mth_id, int yr,
                        char *iday, char *imth, char *infname) {

    if (day < 10) {
        sprintf(iday, "0%d", day);
    } else {
        sprintf(iday, "%d", day);
    }

    if (mth_id == 0) {
        sprintf(imth, "12");
    } else if (mth_id == 1) {
        sprintf(imth, "01");
    } else if (mth_id == 2) {
        sprintf(imth, "02");
    }

    if (mth_id == 0) {
        sprintf(infname, "%s/eMAST_ANUClimate_day_tmax_v1m0_%d%s%s.nc",
                c->fdir, yr, imth, iday);
    } else {
        sprintf(infname, "%s/eMAST_ANUClimate_day_tmax_v1m0_%d%s%s.nc",
                c->fdir, yr+1, imth, iday);
    }
    return;
}

void calculate_dry_spells(control *c, int ndays,
                          float data_in[MAX_DAYS][NLAT][NLON],
                          float *data_out, float *data_out2, int *cnt_all_yrs,
                          int *cnt_all_yrs2) {
    // Calculate the longest dry spell in a year and keep a record of all of
    // the dry spells to figure out the average

    int    rr, cc, i, j;
    long   offset, count, yr_count;
    float  yr_max;

    for (rr = 0; rr < NLAT; rr++) {
        for (cc = 0; cc < NLON; cc++) {
            offset = rr * NLON + cc;

            count = 0;
            yr_count = 0;
            for (i = 0; i < ndays ; i++) {

                // Effective rainfall, values less than 2 mm
                if (data_in[i][rr][cc] < 2.0) {
                    count++;
                } else if (data_in[i][rr][cc] > 2.0) {
                    // Need to save dry spells to figure average dry spell
                    data_out[offset] += (float)count;
                    cnt_all_yrs[offset]++;
                    count = 0;
                }

                if (count > yr_count) {
                    yr_count = count;
                }

            } // end day loop

            // Save longest dry spell in a year
            if (yr_count > 0.0) {
                data_out2[offset] += (float)yr_count;
                cnt_all_yrs2[offset]++;
            }

        } // end column loop
    } // end row loop

    return;
}

void calculate_avg_dry_spells_over_all_years(control *c, float *data,
                                             float *data2, int *count,
                                             int *count2) {

    int  yr, rr, cc;
    long offset;

    for (rr = 0; rr < NLAT; rr++) {
        for (cc = 0; cc < NLON; cc++) {
            offset = rr * NLON + cc;
            if (data[offset] > 0.0) {
                data[offset] /= (float)count[offset];
            }

            if (data2[offset] > 0.0) {
                data2[offset] /= (float)count2[offset];
            }
        }
    }

    return;
}

int is_leap_year(int year) {

    int leap = FALSE;
    if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0)) {
        leap = TRUE;
    }

    return (leap);

}

void clparser(int argc, char **argv, control *c) {
    int i;

    for (i = 1; i < argc; i++) {
        if (*argv[i] == '-') {
            if (!strncasecmp(argv[i], "-fd", 3)) {
                strcpy(c->fdir, argv[++i]);
            } else if (!strncasecmp(argv[i], "-vn", 3)) {
                strcpy(c->var_name, argv[++i]);
            } else if (!strncasecmp(argv[i], "-w", 2)) {
                c->window = atoi(argv[++i]);
            } else if (!strncasecmp(argv[i], "-sy", 3)) {
                c->start_yr = atoi(argv[++i]);
            } else if (!strncasecmp(argv[i], "-ey", 3)) {
                c->end_yr = atoi(argv[++i]);
            } else {
                fprintf(stderr,"%s: unknown argument on command line: %s\n",
                        argv[0], argv[i]);
                exit(EXIT_FAILURE);
            }
        }
    }
    return;
}

void read_nc_file_into_array(control *c, char *infname, int day_idx,
                 float nc_in[MAX_DAYS][NLAT][NLON]) {

    int  status, nc_id, var_id;

    if ((status = nc_open(infname, NC_NOWRITE, &nc_id))) {
        ERR(status);
    }

    if ((status = nc_inq_varid(nc_id, c->var_name, &var_id))) {
        ERR(status);
    }

    if ((status = nc_get_var_float(nc_id, var_id,
                                   &nc_in[day_idx][0][0]))) {
        ERR(status);
    }

    if ((status = nc_close(nc_id))) {
        ERR(status);
    }

    return;
}

void write_nc_file(char *ofname, float out[NLAT][NLON]) {

    int  status, nc_id, var_id, nday_idx;
    int  x_dimid, y_dimid;
    int  dimids[NDIMS];

    if ((status = nc_create(ofname, NC_CLOBBER, &nc_id))) {
        ERR(status);
    }

    if ((status = nc_def_dim(nc_id, "x", NLON, &x_dimid))) {
        ERR(status);
    }

    if ((status = nc_def_dim(nc_id, "y", NLAT, &y_dimid))) {
        ERR(status);
    }

    dimids[0] = y_dimid;
    dimids[1] = x_dimid;

    if ((status = nc_def_var(nc_id, "Tmax", NC_FLOAT, NDIMS,
                             dimids, &var_id))) {
        ERR(status);
    }

    if ((status = nc_enddef(nc_id))) {
        ERR(status);
    }

    if ((status = nc_put_var_float(nc_id, var_id, &out[0][0]))) {
        ERR(status);
    }

    if ((status = nc_close(nc_id))) {
        ERR(status);
    }

    return;
}
