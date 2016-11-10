#!/usr/bin/env python

"""
Quickly plot the ouput from the C code

That's all folks.
"""
__author__ = "Martin De Kauwe"
__version__ = "1.0 (08.11.2016)"
__email__ = "mdekauwe@gmail.com"

import os
import netCDF4 as nc
import matplotlib.pyplot as plt
import sys
import pandas as pd
import numpy as np
import calendar
from datetime import datetime, timedelta

def main():

    fn = "test1.nc"
    #fn = "/Users/mdekauwe/Downloads/emast_data/eMAST_ANUClimate_day_tmax_v1m0_20000101.nc"
    f = nc.Dataset(fn)
    data = f.variables['Tmax'][:,:]
    #data = f.variables['air_temperature'][0,:,:]

    plt.imshow(data)
    plt.colorbar()
    plt.show()


if __name__ == "__main__":

    main()
