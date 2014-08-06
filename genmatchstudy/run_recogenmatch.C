#include "recogenmatch.cc"

//decaychannel 2 is D0

void run_recogenmatch(int startFile, int endFile, char *filelist, int decaychannel)
{
    recogenmatch *a = new recogenmatch;

    a->LoopOverFile(startFile, endFile, filelist, decaychannel);

    //delete a; 
}
