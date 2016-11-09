#include <stdlib.h>
#include <stdio.h>
#include <netcdf.h>

#define FILE_NAME "eMAST_ANUClimate_day_tmax_v1m0_20000101.nc"
#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}
#define NTIM 1
#define NLAT 3474
#define NLON 4110

int main() {

    int   jj, d, r, c, x, y, status, nc_id, var_id, yr, ndays;
    int   ntime = 1;
    int   nlats = 3474;
    int   nlons = 4110;
    int   window = 5;
    long  offset;
    //static float data_out[NLAT][NLON];

    float  max_5day_sum, sum;

    float *data_out = NULL;
    data_out = calloc(NLAT*NLON, sizeof(float));

    for (yr = 1970; yr < 1971; yr++) {
        printf("%d\n", yr);
        // Add leap year code
        ndays = 90;

        // Need to do this as we are allocating memory on the stack
        static float data_in[90][NLAT][NLON];

        for (d = 0; d < ndays; d++) {

            if ((status = nc_open(FILE_NAME, NC_NOWRITE, &nc_id))) {
                ERR(status);
            }

            if ((status = nc_inq_varid(nc_id, "air_temperature", &var_id))) {
                ERR(status);
            }

            if ((status = nc_get_var_float(nc_id, var_id, &data_in[d][0][0]))) {
                ERR(status);
            }

            if ((status = nc_close(nc_id))) {
                ERR(status);
            }

        }

        for (r = 0; r < nlats; r++) {
            //printf("%d:%d\n", r, nlats);
            for (c = 0; c < nlons; c++) {
                offset = r * nlons + c;
                max_5day_sum = -9999;
                for (d = 0; d < ndays-window; d++) {
                    sum = 0.0;
                    for (jj = d; jj < d+window; jj++) {
                        sum += data_in[jj][r][c];
                    }

                    if (sum > max_5day_sum) {
                        data_out[offset] = sum;
                    }
                }
            }
        }
    }

    return(EXIT_SUCCESS);

}
