#include "candbasedtree.cc"

//decaychannel 2 is D0

void run_candtree(int startFile, int endFile, char *filelist, int decaychannel)
{
    candbasedtree *a = new candbasedtree;

    a->LoopOverFile(startFile, endFile, filelist, decaychannel);

    //delete a; 
}
