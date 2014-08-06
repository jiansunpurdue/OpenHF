#include "trktreerecoeff.cc"

//decaychannel 2 is D0

void run_trktreerecoeff(int startFile, int endFile, char *filelist, int decaychannel)
{
    trktreerecoeff *a = new trktreerecoeff;

    a->LoopOverFile(startFile, endFile, filelist, decaychannel);

    //delete a; 
}
