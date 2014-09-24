#ifndef trigcom_pPb_H
#define trigcom_pPb_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TNamed.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TCanvas.h>
#include "./../interface/hfcand_v0.hh"

#define NPT 16
#define nch 3
#define MAXPARTICLES 50000
#define NTRIG 6

double ptbins[NPT+1] = {0,3,6,8,10,12,14,16,20,30,40,50,60,70,80,100,150};
double Prescl[NTRIG] = { 2.0, 1.75, 1.5, 1.25, 1.0, 1.0};       //MB, Jet20, 40, 60, 80, 100

using namespace std;

class trigcom_pPb 
{
    private:
        string filename;
        TFile* result;  //.. result output file name

        int type;
        float mass;
        float fpt;
        float feta;
        float ffls3d;
        float alpha;
        float fprob;
        float fdr;
        float fchi2;
        float mass_dau;
        float fpt1;
        float fpt2;
		int   fq1;
		int   fq2;

        vector<int>  gIndex1;
        vector<int>  gIndex2;
        vector<int>  gIndex_dau2;

        Int_t mult;
        Float_t pt[MAXPARTICLES];
        Float_t eta[MAXPARTICLES];
        Float_t phi[MAXPARTICLES];
        Int_t pdg[MAXPARTICLES];
        Int_t chg[MAXPARTICLES];
        Int_t sube[MAXPARTICLES];
        Int_t sta[MAXPARTICLES];
        Int_t matchingID[MAXPARTICLES];
        Int_t nMothers[MAXPARTICLES];
        Int_t motherIdx[MAXPARTICLES][200];
        Int_t nDaughters[MAXPARTICLES];
        Int_t daughterIdx[MAXPARTICLES][200];


        // trigger Prescl weight for each events
        float maxJetTrgPt;
        float maxJetTrgEta;
        float maxJetTrgPhi;
        float maxJetTrgMass;

        //trig decison and trig Prescl
		int Jet20;
		int Jet20_Prescl;
		int Jet40;
		int Jet40_Prescl;
		int Jet60;
		int Jet60_Prescl;
		int Jet80;
		int Jet80_Prescl;
		int Jet100;
		int Jet100_Prescl;
		int ZeroBias;
		int ZeroBias_Prescl;
		int L1_ZeroBias;
		int L1_ZeroBias_Prescl;
		int L1_SingleJet12;
		int L1_SingleJet12_Prescl;
		int L1_SingleJet16;
		int L1_SingleJet16_Prescl;
		int L1_SingleJet36;
		int L1_SingleJet36_Prescl;
		

    private:  //.. forest tree used in the analyses
	    TTree *genparticletree;
	    TTree *hftree;
		TTree *HltTree;
		TTree *jetObjTree;
		hfcand_v0* hfcandidate;

        float cut_m_low[nch]; //.. low end of the mass hiso.
        float cut_m_high[nch]; //.. high end of the mass hiso
        float cut_m_dau_low[nch]; //.. low end of the daughter mass cut
        float cut_m_dau_high[nch]; //.. high end of the daughter mass cut
        float cut_ffls3d[nch];
        float cut_falpha0[nch];
        float cut_fprob[nch];
        float cut_fdr[nch];
        float cut_fchi2[nch];



    private: //.. histograms
	    TH1F * hfg_all[NPT];
	    TH1F * hfg_jettrigfired[NPT];

		TH1F * hfg_jet_combined[NPT];
		TH1F * hfg_jet_combined_fill[NPT];
		TH1F * hfg_jet_mb_combined[NPT];

		TH1F * hfg_zerobias_raw[NPT];         //raw is before prescaled
        TH1F * hfg_jet20_raw[NPT];
        TH1F * hfg_jet40_raw[NPT];
        TH1F * hfg_jet60_raw[NPT];
        TH1F * hfg_jet80_raw[NPT];
        TH1F * hfg_jet100_raw[NPT];
		
        TH1F * hfg_zerobias[NPT];
		TH1F * hfg_jet20[NPT];
		TH1F * hfg_jet40[NPT];
		TH1F * hfg_jet60[NPT];
		TH1F * hfg_jet80[NPT];
		TH1F * hfg_jet100[NPT];
        
		TH1F * hfg_mb_jetpt20_raw[NPT];
		TH1F * hfg_jet20_jetpt40_raw[NPT];
		TH1F * hfg_jet40_jetpt60_raw[NPT];
		TH1F * hfg_jet60_jetpt80_raw[NPT];
		TH1F * hfg_jet80_jetpt100_raw[NPT];
		TH1F * hfg_jet100_jetpt200_raw[NPT];


        TH1F * hfg_mb_jetpt20[NPT];
        TH1F * hfg_jet20_jetpt40[NPT];
        TH1F * hfg_jet40_jetpt60[NPT];
        TH1F * hfg_jet60_jetpt80[NPT];
        TH1F * hfg_jet80_jetpt100[NPT];
        TH1F * hfg_jet100_jetpt200[NPT];


    private:
        void Init(int startFile, int endFile, char *filelist);
        void write();
        void get_trg_info();
		void LoopOverEvt( TTree * T);
		void LoopOverHFCandidate();
		int  Find_d0();
		void define_cuts();
		void book_hist();
		int  decideptbin( float dpt);

    public:
        trigcom_pPb();
        virtual ~trigcom_pPb();

        void LoopOverFile(int startFile, int endFile, char *filelist, int decaychannel);
  //      void draw(char* filename, int ich, TCanvas* cfg);

};

#endif //  trigcom_pPb_H
