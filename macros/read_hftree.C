#include <fstream>
#include <iostream>

#include "TH1F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"

#include "./../data_format/hfcand_v0.hh"

#define NPT  8 //.. number of histograms
double ptbins[NPT+1] = {0,4,5,8,12,16,25,40,200};

//const int nbin = 200;
const int nbin = 350;
//const short NPT = 1;
//const float delta_pt = 400;

using namespace std;

int decideptbin( float pt )
{
        int ipt = -1;
        for ( int i = 0 ; i < NPT; i++)
        {
            if (pt >= ptbins[i] && pt < ptbins[i+1])  { ipt = i; break; }
        }
        if ( pt > ptbins[NPT] ) ipt = NPT-1;
        return ipt;
}



void read_hftree(int startFile, int endFile, char *filelist, int type, 
        float cut_m_low, float cut_m_high, 
        float cut_m_dau_tight_low, float cut_m_dau_tight_high,
        float cut_m_dau_loose_low, float cut_m_dau_loose_high,
        float cut_ffls3d = 2, float cut_falpha0 = 0.5, float cut_fprob = 0.05, 
        float cut_fdr = 0.25, float cut_fchi2 = 2.0, 
        int nseg = 1000)
{
//   gSystem->Load("../../../../lib/slc5_amd64_gcc434/libUserCodeOpenHF.so");
//    gSystem->Load("../../../../lib/slc5_amd64_gcc434/libAna00.so");
    hfcand_v0* hfcandidate = new hfcand_v0;
    //.. output file ...
    char* filename = new char[1000];
    char* hname = new char[1000];
    char* pt_range = new char[1000];
    char* mesonName = new char[100];

    if(type==1) 
        mesonName = "Dstar2D0Pi";
    else if(type==2)
        mesonName = "D02KPi";
    else if(type==3)
        mesonName = "Ds2PhiPi";
    else if(type==4)
        mesonName = "Ds2KstarK";
    else if(type==5)
        mesonName = "Dpm2KPiPi";

    sprintf(filename, "ana_%s_%s_%dTo%d_dcut%f.root", mesonName, filelist, startFile, endFile, cut_ffls3d);

    TFile result(filename, "recreate");

    //.histogram
    TH1* hfg[NPT] = {0};
    TH1* hbg[NPT] = {0};
    TH1* hdau[NPT] = {0};
    TH1* hfgdiff[NPT] = {0};
    TH1* hbgdiff[NPT] = {0};
    for(short i = 0; i<NPT; i++) {
        float pt_low = ptbins[i];;
        float pt_high = ptbins[i+1];;

        sprintf(hname, "hfg%d", i);
        sprintf(pt_range, "pt: %f %f", pt_low, pt_high);
        hfg[i] = new TH1F(hname, pt_range, nbin, cut_m_low, cut_m_high);

        sprintf(hname, "hbg%d", i);
        hbg[i] = new TH1F(hname, pt_range, nbin, cut_m_low, cut_m_high);

        sprintf(hname, "hdau%d", i);
        hdau[i] = new TH1F(hname, pt_range, nbin, cut_m_dau_loose_low, cut_m_dau_loose_high);

        sprintf(hname, "hfgdiff%d", i);
        hfgdiff[i] = new TH1F(hname, pt_range, nbin, 0.135, 0.154);

        sprintf(hname, "hbgdiff%d", i);
        hbgdiff[i] = new TH1F(hname, pt_range, nbin, 0.135, 0.154);

        //.. track error ...
        hfg[i]->Sumw2();
        hbg[i]->Sumw2();
        hdau[i]->Sumw2();
        hfgdiff[i]->Sumw2();
        hbgdiff[i]->Sumw2();
    }

    //..loop over files ...
    ifstream file_stream(filelist);

    int ifile = 0;
    while (file_stream >> filename) {

        if(ifile >=endFile) 
            break;

        if(ifile<startFile || !filename) {
            ifile++;
            continue;
        }

        ifile++;

        cout << "file  = " << ifile<<" name = "<<filename <<endl;


        //..the files are in dcache ..
		if (!TFile::Open(filename)) {  cout << "fail to open file" << endl;  continue;}
        TFile *f = TFile::Open(filename);
        TTree *hftree = (TTree*)f->Get("HFtree/hftree");
//        TTree *hftree = (TTree*)f->Get("hftree");

        hftree->SetBranchAddress("hfcandidate", &hfcandidate);

        for(int j = 0; j<hftree->GetEntries(); j++) {
            hftree->GetEntry(j);

            for(int ic = 0; ic<hfcandidate->get_nhfcand(); ic++) {
                snglhfcand* cand = hfcandidate->get_hfcand(ic);

                if(cand->get_type() != type) continue;

                float mass = cand->get_fm();
                float fpt = cand->get_fpt();
				float feta = cand->get_feta();
                float ffls3d = cand->get_ffls3d();
                float alpha = cand->get_falpha0();
                float fprob = cand->get_fprob();
                float fdr = cand->get_fdr();
                float fchi2 = cand->get_fchi2();
                float mass_dau = cand->get_fmdau1();

				if ( feta > 2 || feta < -2 ) continue;


                int ih = decideptbin(fpt);

                if(ih >= NPT)
                    ih = NPT - 1;  //.. the last bin include all higher pT region...

                if(mass > cut_m_low && mass < cut_m_high && 
                        ffls3d > cut_ffls3d && alpha < cut_falpha0 && 
                        fprob > cut_fprob && fdr < cut_fdr && fchi2 < cut_fchi2) {

                    //.. forground ..
                    if(mass_dau > cut_m_dau_loose_low && mass_dau < cut_m_dau_loose_high) {
                        hdau[ih]->Fill(mass_dau);

                        if(mass_dau > cut_m_dau_tight_low && mass_dau < cut_m_dau_tight_high) {
                            hfg[ih]->Fill(mass);
                            hfgdiff[ih]->Fill(mass-mass_dau);
                        }
                    }

                    //.. D0 has no daughter mass, treat separately. ...
                    if(type==2) {
                            hfg[ih]->Fill(mass);
                    }

                    //.. background through side band ...
                    if(mass_dau < cut_m_dau_tight_low || mass_dau > cut_m_dau_tight_high) {
                        hbg[ih]->Fill(mass);
                        hbgdiff[ih]->Fill(mass-mass_dau);
                    }
                }
            }

            hfcandidate->Reset();
        }

		f->Close();

        delete f;
    }

    //.. normalization of through side bands...


    //.. save files ...
    result.cd();
    for(short i = 0; i<NPT; i++) {
        hfg[i]->Write();
        hbg[i]->Write();
        hdau[i]->Write();
        hfgdiff[i]->Write();
        hbgdiff[i]->Write();
    }
    result.Close();
    cout << "output file closed" << endl;
}
