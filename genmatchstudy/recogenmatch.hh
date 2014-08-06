#ifndef recogenmatch_H
#define recogenmatch_H

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

#define MAXPARTICLES 50000
#define nch 3

using namespace std;

class recogenmatch 
{
    private:
        string filename;

        TFile* result;  //.. result output file name

        double d0pt;
        double d0eta;
        double d0phi;
        int    d0pid;
		double d0fm;
		double d0fpt;
		double d0ffls3d;
		double d0alpha;
		double d0fdr;
		double d0fchi2;
		double d0fprob;
		int    d0passingtopocuts;

        double dstarpt;
        double dstareta;
        double dstarphi;
        int dstarpid;
		double dstarfm;
		double dstarfpt;
		double dstarffls3d;
		double dstaralpha;
		double dstarfdr;
		double dstarfchi2;
		double dstarfprob;
		double dstarmass_dau;
		int    dstarpassingtopocuts;


        double dchargedpt;
        double dchargedeta;
        double dchargedphi;
        int dchargedpid;
		double dchargedfm;
		double dchargedfpt;
		double dchargedffls3d;
		double dchargedalpha;
		double dchargedfdr;
		double dchargedfchi2;
		double dchargedfprob;
		double dchargedmass_dau;
		int    dchargedpassingtopocuts;


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



    private:  //.. forest tree used in the analyses
        TTree *genparticletree;
        TTree *hftree;  
		TTree *d0tree;
		TTree *dstartree;
		TTree *dchargedtree;
		hfcand_v0* hfcandidate; 

    private: //.. different cuts used to produce results.

        float cut_m_low[nch]; //.. low end of the mass hiso.
        float cut_m_high[nch]; //.. high end of the mass hiso
        float cut_m_dau_low[nch]; //.. low end of the daughter mass cut
        float cut_m_dau_high[nch]; //.. high end of the daughter mass cut
        float cut_ffls3d[nch];
        float cut_falpha0[nch];
        float cut_fprob[nch];
        float cut_fdr[nch];
        float cut_fchi2[nch];


    private:
        void Init(int startFile, int endFile, char *filelist);
        void Find_dcharged(TTree* T);
		void Find_d0(TTree* T);
		void Find_dstar(TTree* T);
        void write();
		void define_cuts();

    public:
        recogenmatch();
        virtual ~recogenmatch();

        void LoopOverFile(int startFile, int endFile, char *filelist, int decaychannel);
  //      void draw(char* filename, int ich, TCanvas* cfg);

};

#endif //  recogenmatch_H
