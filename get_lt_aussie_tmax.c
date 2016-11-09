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

int main() {

    int   jj, d, r, c, x, y, status, nc_id, var_id, yr, mth, ndays, days_in_mth;
    int   day;
    int   ntime = 1;
    int   nlats = 3474;
    int   nlons = 4110;
    int   window = 5;
    long  offset;
    int   summer_mths[] = {12, 1, 2};
    int   nmonths = 3;
    int   i = 0;
    char  imth[3];
    char  iday[3];
    char  infname[STRING_LENGTH];
    char  fdir[STRING_LENGTH];


    //static float data_out[NLAT][NLON];

    float  max_5day_sum, sum;

    float *data_out = NULL;
    data_out = calloc(NLAT*NLON, sizeof(float));

    strcpy(fdir, "/Users/mdekauwe/Downloads/emast_data");

    for (yr = 1970; yr < 1971; yr++) {
        printf("%d\n", yr);

        ndays = 0;
        for(mth = 0; mth < nmonths; mth++) {

            // Dec, Jan Feb?
            if (mth == 0) {
                ndays += 31;
                days_in_mth = 31;
            } else if (mth == 1) {
                ndays += 31;
                days_in_mth = 31;
            } else if (mth == 2) {
                if (is_leap_year(yr+1)) {
                    ndays += 29;
                    days_in_mth = 29;
                } else {
                    ndays += 28;
                    days_in_mth = 28;
                }
            }

            for (day = 1; day <= days_in_mth; day++) {

                if (day < 10)
	                sprintf(iday, "0%d", day);
	            else
	                sprintf(iday, "%d", day);

	            if (mth < 10)
	                sprintf(imth, "0%d", mth);
	            else
	                sprintf(imth, "%d", mth);

                if (mth == 12) {
                    sprintf(infname,
                            "%s/eMAST_ANUClimate_day_tmax_v1m0_%d%s%s.nc",
                            fdir, yr, imth, iday);
                } else {
                    sprintf(infname,
                            "%s/eMAST_ANUClimate_day_tmax_v1m0_%d%s%s.nc",
                            fdir, yr+1, imth, iday);
                }

                printf("%s\n", infname);

            }  // Day in month loop

            printf("%d\n", ndays);




        } // mth loop


    } // yr loop

    return(EXIT_SUCCESS);

}


int is_leap_year(int year) {

    int leap = FALSE;
    if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
        leap = TRUE;
    }

    return (leap);

}
