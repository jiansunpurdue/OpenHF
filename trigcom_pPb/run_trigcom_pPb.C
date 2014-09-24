#include "trigcom_pPb.cc"

//decaychannel 2 is D0

void run_trigcom_pPb(int startFile, int endFile, char *filelist, int decaychannel)
{
    trigcom_pPb *a = new trigcom_pPb;

    a->LoopOverFile(startFile, endFile, filelist, decaychannel);

    //delete a; 
}
