#ifndef gensignalget_H
#define gensignalget_H

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

#define MAXPARTICLES 50000

using namespace std;

class gensignalget 
{
    private:
        string filename;

        TFile* result;  //.. result output file name

        
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
        
        double d0pt;
        double d0eta;
        double d0phi;
        int    d0pid;
        int    d0nofdau;
        double d0daupt[2];
        double d0daueta[2];
        double d0dauphi[2];
        int    d0daupid[2];
     
        double dstarpt;
        double dstareta;
        double dstarphi;
        int dstarpid;
        int dstarnofdau;
        double dstardaupt[3];
        double dstardaueta[3];
        double dstardauphi[3];
        int    dstardaupid[3];
        int    dstardaumompid[3];
     
        double dsphipt;
        double dsphieta;
        double dsphiphi;
        int dsphipid;
        int dsphinofdau;
        double dsphidaupt[3];
        double dsphidaueta[3];
        double dsphidauphi[3];
        int    dsphidaupid[3];
        int    dsphidaumompid[3];
     
     
        double dchargedpt;
        double dchargedeta;
        double dchargedphi;
        int dchargedpid;
        int dchargednofdau;
        double dchargeddaupt[3];
        double dchargeddaueta[3];
        double dchargeddauphi[3];
        int    dchargeddaupid[3];

    private:  //.. forest tree used in the analyses

        TTree *GenParticleTree; 
 
		TTree *d0tree;
		TTree *dstartree;
		TTree *dsphitree;
		TTree *dchargedtree;

    private: //.. different cuts used to produce results.




    private:
        void Init(int startFile, int endFile, char *filelist);
        void Find_dcharged(TTree* T);
		void Find_d0(TTree* T);
		void Find_dstar(TTree* T);
		void Find_dsphi(TTree* T);
        void write();
        void Read_tracktree(TTree* T);

    public:
        gensignalget();
        virtual ~gensignalget();

        void LoopOverFile(int startFile, int endFile, char *filelist, int decaychannel);
  //      void draw(char* filename, int ich, TCanvas* cfg);

};

#endif //  gensignalget_H
