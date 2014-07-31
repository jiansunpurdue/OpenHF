#include "gensignalget.cc"

//decaychannel 2 is D0

void run_gensignalget(int startFile, int endFile, char *filelist, int decaychannel)
{
    gensignalget *a = new gensignalget;

    a->LoopOverFile(startFile, endFile, filelist, decaychannel);

    //delete a; 
}
