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

    int   jj, d, r, c, x, y, status, nc_id, var_id, yr, mth, ndays;
    int   ntime = 1;
    int   nlats = 3474;
    int   nlons = 4110;
    int   window = 5;
    long  offset;
    int   summer_mths[] = {12, 1, 2};
    int   nmonths = 3;
    int   i = 0;

    //static float data_out[NLAT][NLON];

    float  max_5day_sum, sum;

    float *data_out = NULL;
    data_out = calloc(NLAT*NLON, sizeof(float));

    for (yr = 1970; yr < 1971; yr++) {
        printf("%d\n", yr);

        ndays = 0;
        for(mth = 0; mth < nmonths; mth++) {

            // Dec, Jan Feb?
            if (mth == 0) {
                ndays += 31;
            } else if (mth == 1) {
                ndays += 31;
            } else if (mth == 2) {
                if (is_leap_year(yr)) {
                    ndays += 29;
                } else {
                    ndays += 28;
                }
            }
        } // mth loop
        printf("%d\n", ndays);
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
