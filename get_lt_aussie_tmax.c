//
//
// PROGRAM:
//              get_lt_aussie_tmax
//
// DESCRIPTION:
//              Using the eMAST Tmax data estimate the long-term N-days Tmax
//              for each pixel across the continent. Where N might be 3, 4 or
//              5 days, not decided yet.
//
// AUTHOR:      Martin De Kauwe
//
// EMAIL:       mdekauwe@gmail.com
//
// DATE:        10th November, 2016
//

#include "get_lt_aussie_tmax.h"

int main(int argc, char **argv) {

    int   jj, d, rr, cc, x, y, status, nc_id, var_id, yr, mth, ndays, days_in_mth;
    int   i, mth_id, day, dd, nmonths = 3;
    long  offset;
    char  imth[3];
    char  iday[3];
    char  infname[STRING_LENGTH];
    char  fdir[STRING_LENGTH];
    char  var_name[STRING_LENGTH];

    // Need to be declared like this otherwise the netcdf read will attempt to
    // use the heap to allocate memory and run out, rather than the stack
    // I'm sure there is a way to get netcdf to read into a 1D array but I
    // don't know how to do that and can't find a quick example
    // NB. I'm declaring 1 extra spot for leap years, so we will need to make
    // sure when we read from this array we are checking leap years.
    static float data_in[MAX_DAYS][NLAT][NLON];
    float        max_5day_sum, sum;
    float       *data_out = NULL;

    control *c;
    c = (control *)malloc(sizeof (control));
	if (c == NULL) {
		fprintf(stderr, "control structure: Not allocated enough memory!\n");
		exit(EXIT_FAILURE);
	}

    data_out = calloc(NLAT*NLON, sizeof(float));

    // Initial assumptions, these can be changed on the cmd line
    strcpy(c->fdir, "/Users/mdekauwe/Downloads/emast_data");
    strcpy(c->var_name, "air_temperature");
    c->window = 5;

    clparser(argc, argv, c);

    for (yr = 1970; yr < 1971; yr++) {
        printf("%d\n", yr);

        ndays = 0;
        for(mth_id = 0; mth_id < nmonths; mth_id++) {

            // Dec, Jan Feb?
            if (mth_id == 0) {
                ndays += 31;
                days_in_mth = 31;
            } else if (mth_id == 1) {
                ndays += 31;
                days_in_mth = 31;
            } else if (mth_id == 2) {
                if (is_leap_year(yr+1)) {
                    ndays += 29;
                    days_in_mth = 29;
                } else {
                    ndays += 28;
                    days_in_mth = 28;
                }
            }

            for (day = 1; day <= days_in_mth; day++) {

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
                    sprintf(infname,
                            "%s/eMAST_ANUClimate_day_tmax_v1m0_%d%s%s.nc",
                            c->fdir, yr, imth, iday);
                } else {
                    sprintf(infname,
                            "%s/eMAST_ANUClimate_day_tmax_v1m0_%d%s%s.nc",
                            c->fdir, yr+1, imth, iday);
                }

                // For now just read the same file again and again!
                sprintf(infname,
                        "%s/eMAST_ANUClimate_day_tmax_v1m0_20000101.nc",
                        c->fdir);

                if ((status = nc_open(infname, NC_NOWRITE, &nc_id))) {
                    ERR(status);
                }

                if ((status = nc_inq_varid(nc_id, c->var_name, &var_id))) {
                    ERR(status);
                }

                if ((status = nc_get_var_float(nc_id, var_id,
                                               &data_in[day-1][0][0]))) {
                    ERR(status);
                }

                if ((status = nc_close(nc_id))) {
                    ERR(status);
                }
                //printf("%s\n", infname);
            }  // Day in month loop
        } // mth loop

        // Calculate the maximum n-day Tmax sum across this years Australian
        // summer for every pixel
        for (rr = 0; rr < NLAT; rr++) {
            for (cc = 0; cc < NLON; cc++) {
                offset = rr * NLON + cc;
                max_5day_sum = -9999;
                for (dd = 0; dd < ndays - c->window; dd++) {
                    sum = 0.0;
                    for (jj = dd; jj < dd+c->window; jj++) {
                        sum += data_in[jj][rr][cc];
                    }
                    if (sum > max_5day_sum) {
                        data_out[offset] = sum;
                    }
                }
            }
        }



    } // yr loop

    //for (r = 0; r < NLAT; r++) {
    //    for (c = 0; c < NLON; c++) {
    //        printf("%lf\n", data_in[15][r][c]);
    //    }
    //}

    return(EXIT_SUCCESS);

}


int is_leap_year(int year) {

    int leap = FALSE;
    if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
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
			} else {
                fprintf(stderr,"%s: unknown argument on command line: %s\n",
                        argv[0], argv[i]);
				exit(EXIT_FAILURE);
     		}
        }
    }
	return;
}
