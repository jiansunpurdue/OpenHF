#ifndef trktreerecoeff_H
#define trktreerecoeff_H

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

#define MAXTRACKS 50000
#define MAXMATCH 5
#define MAXQUAL 5
#define MAXPARTICLES 50000

using namespace std;

class trktreerecoeff 
{
    private:
        string filename;

        TFile* result;  //.. result output file name

  // -- rec tracks --
        int nTrk;
        int matchedGenID[MAXTRACKS][MAXMATCH];
        float trkEta[MAXTRACKS];
        float trkPhi[MAXTRACKS];
        float trkPt[MAXTRACKS];
        bool trkQual[MAXTRACKS];


        float trkPtError[MAXTRACKS];
        int trkNHit[MAXTRACKS];
        int trkNlayer[MAXTRACKS];
        int trkNlayer3D[MAXTRACKS];
        float trkChi2[MAXTRACKS];
        float trkChi2hit1D[MAXTRACKS];
        float trkNdof[MAXTRACKS];
        float trkDz[MAXTRACKS];
        float trkDz1[MAXTRACKS];               // dZ to the first vertex
        float trkDz2[MAXTRACKS];               // dZ to the second vertex
        float trkDzError[MAXTRACKS];
        float trkDzError1[MAXTRACKS];
        float trkDzError2[MAXTRACKS];
        float trkDxy[MAXTRACKS];
        float trkDxyBS[MAXTRACKS];
        float trkDxy1[MAXTRACKS];              // d0 to the first vertex
        float trkDxy2[MAXTRACKS];              // d0 to the second vertex
        float trkDxyError[MAXTRACKS];
        float trkDxyErrorBS[MAXTRACKS];
        float trkDxyError1[MAXTRACKS];
        float trkDxyError2[MAXTRACKS];
        float trkVx[MAXTRACKS];
        float trkVy[MAXTRACKS];
        float trkVz[MAXTRACKS];
        bool  trkFake[MAXTRACKS];
        float trkAlgo[MAXTRACKS];
        float dedx[MAXTRACKS];
        int trkCharge[MAXTRACKS];
        int trkNVtx[MAXTRACKS];
        unsigned int trkVtxIndex[MAXTRACKS];
        int nTrkTimesnVtx;
      
        float trkExpHit1Eta[MAXTRACKS];
        float trkExpHit2Eta[MAXTRACKS];
        float trkExpHit3Eta[MAXTRACKS];
        float trkStatus[MAXTRACKS];
        float trkPId[MAXTRACKS];
        float trkMPId[MAXTRACKS];
        float trkGMPId[MAXTRACKS];


		//gen particles
		//

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
		double d0daurecopt[2];
        double d0daueta[2];
        double d0dauphi[2];
        int    d0daupid[2];
		int    d0allhighpurity;
     
        double dstarpt;
        double dstareta;
        double dstarphi;
        int dstarpid;
        int dstarnofdau;
        double dstardaupt[3];
		double dstardaurecopt[3];
        double dstardaueta[3];
        double dstardauphi[3];
        int    dstardaupid[3];
        int    dstardaumompid[3];
		int    dstarallhighpurity;
     
        double dsphipt;
        double dsphieta;
        double dsphiphi;
        int dsphipid;
        int dsphinofdau;
        double dsphidaupt[3];
		double dsphidaurecopt[3];
        double dsphidaueta[3];
        double dsphidauphi[3];
        int    dsphidaupid[3];
        int    dsphidaumompid[3];
		int    dsphiallhighpurity;
     
     
        double dchargedpt;
        double dchargedeta;
        double dchargedphi;
        int dchargedpid;
        int dchargednofdau;
        double dchargeddaupt[3];
		double dchargeddaurecopt[3];
        double dchargeddaueta[3];
        double dchargeddauphi[3];
        int    dchargeddaupid[3];
		int    dchargedallhighpurity;

    private:  //.. forest tree used in the analyses
        TTree *Tracktree;  
		TTree *Genparticletree;
		TTree *d0tree;
		TTree *dstartree;
		TTree *dsphitree;
		TTree *dchargedtree;

    private: //.. different cuts used to produce results.




    private:
        void Init(int startFile, int endFile, char *filelist);
        void Find_dcharged(TTree* T);
		void Find_d0dstar(TTree* T);
		void Find_dsphi(TTree* T);
        void write();
        void Read_tree();

    public:
        trktreerecoeff();
        virtual ~trktreerecoeff();

        void LoopOverFile(int startFile, int endFile, char *filelist, int decaychannel);
  //      void draw(char* filename, int ich, TCanvas* cfg);

};

#endif //  trktreerecoeff_H
