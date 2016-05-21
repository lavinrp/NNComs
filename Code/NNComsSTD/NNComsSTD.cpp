// NNComsSTD.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "NNComsSTD.h"


// This is an example of an exported variable
NNCOMSSTD_API int nNNComsSTD=0;

// This is an example of an exported function.
NNCOMSSTD_API int fnNNComsSTD(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see NNComsSTD.h for the class definition
CNNComsSTD::CNNComsSTD()
{
    return;
}


