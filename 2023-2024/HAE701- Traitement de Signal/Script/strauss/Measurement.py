from ctypes import *
import numpy as np
import os
import platform

def buildOSString():
    return platform.system()+"-"+platform.machine()


libNames = {"Linux-x86_64":"libMeasurement-linux.so.1",\
            "Darwin-x86_64":"libMeasurement-macintel.so.1",\
            "Darwin-arm64":"libMeasurement-macarm.so.1",\
            "Windows-AMD64":"libMeasurement-win.dll"   
            }
OS_String = buildOSString()

libPath = os.path.abspath(os.path.dirname(__file__))
libName = os.path.join(libPath,libNames[OS_String])
print(libName)
libmeas = CDLL(libName)
libmeas.callCreateRandomSignal.argtypes = [POINTER(c_double),c_int]

def CreateRandomSignal(N):
    T = np.empty(N,dtype=float)
    libmeas.callCreateRandomSignal(cast(T.ctypes.data,POINTER(c_double)),N)
    return T

    
