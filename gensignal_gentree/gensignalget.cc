#include <fstream>
#include <iostream>
#include "TH1F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TBranch.h"
#include "TMath.h"
#include <TF1.h>
#include "gensignalget.hh"
#include <iomanip>
//
gensignalget::gensignalget()
{
}

//
gensignalget::~gensignalget()
{
//    delete filename;
    delete result;

}
//
void gensignalget::Init(int startFile, int endFile, char *filelist)
{//.. called right after constructor 
    char * outfilename = new char[1000];
    sprintf(outfilename, "gensignalget_%s_%dTo%d.root",  filelist, startFile, endFile);
    result = new TFile(outfilename, "recreate");

    //

    GenParticleTree = 0;


   d0tree = new TTree("d0tree","d0tree");
   d0tree->Branch("d0pt",&d0pt,"d0pt/D");
   d0tree->Branch("d0eta",&d0eta,"d0eta/D");
   d0tree->Branch("d0phi",&d0phi,"d0phi/D");
   d0tree->Branch("d0pid",&d0pid,"d0pid/I");
   d0tree->Branch("d0nofdau",&d0nofdau,"d0nofdau/I");
   d0tree->Branch("d0daupt",d0daupt,"d0daupt[2]/D");
   d0tree->Branch("d0daueta",d0daueta,"d0daueta[2]/D");
   d0tree->Branch("d0dauphi",d0dauphi,"d0dauphi[2]/D");
   d0tree->Branch("d0daupid",d0daupid,"d0daupid[2]/I");

   dstartree = new TTree("dstartree","dstartree");
   dstartree->Branch("dstarpt",&dstarpt,"dstarpt/D");
   dstartree->Branch("dstareta",&dstareta,"dstareta/D");
   dstartree->Branch("dstarphi",&dstarphi,"dstarphi/D");
   dstartree->Branch("dstarpid",&dstarpid,"dstarpid/I");
   dstartree->Branch("dstarnofdau",&dstarnofdau,"dstarnofdau/I");
   dstartree->Branch("dstardaupt",dstardaupt,"dstardaupt[3]/D");
   dstartree->Branch("dstardaueta",dstardaueta,"dstardaueta[3]/D");
   dstartree->Branch("dstardauphi",dstardauphi,"dstardauphi[3]/D");
   dstartree->Branch("dstardaupid",dstardaupid,"dstardaupid[3]/I");
   dstartree->Branch("dstardaumompid",dstardaumompid,"dstardaumompid[3]/I");

   dsphitree = new TTree("dsphitree","dsphitree");
   dsphitree->Branch("dsphipt",&dsphipt,"dsphipt/D");
   dsphitree->Branch("dsphieta",&dsphieta,"dsphieta/D");
   dsphitree->Branch("dsphiphi",&dsphiphi,"dsphiphi/D");
   dsphitree->Branch("dsphipid",&dsphipid,"dsphipid/I");
   dsphitree->Branch("dsphinofdau",&dsphinofdau,"dsphinofdau/I");
   dsphitree->Branch("dsphidaupt",dsphidaupt,"dsphidaupt[3]/D");
   dsphitree->Branch("dsphidaueta",dsphidaueta,"dsphidaueta[3]/D");
   dsphitree->Branch("dsphidauphi",dsphidauphi,"dsphidauphi[3]/D");
   dsphitree->Branch("dsphidaupid",dsphidaupid,"dsphidaupid[3]/I");
   dsphitree->Branch("dsphidaumompid",dsphidaumompid,"dsphidaumompid[3]/I");



   dchargedtree = new TTree("dchargedtree","dchargedtree");
   dchargedtree->Branch("dchargedpt",&dchargedpt,"dchargedpt/D");
   dchargedtree->Branch("dchargedeta",&dchargedeta,"dchargedeta/D");
   dchargedtree->Branch("dchargedphi",&dchargedphi,"dchargedphi/D");
   dchargedtree->Branch("dchargedpid",&dchargedpid,"dchargedpid/I");
   dchargedtree->Branch("dchargednofdau",&dchargednofdau,"dchargednofdau/I");
   dchargedtree->Branch("dchargeddaupt",dchargeddaupt,"dchargeddaupt[3]/D");
   dchargedtree->Branch("dchargeddaueta",dchargeddaueta,"dchargeddaueta[3]/D");
   dchargedtree->Branch("dchargeddauphi",dchargeddauphi,"dchargeddauphi[3]/D");
   dchargedtree->Branch("dchargeddaupid",dchargeddaupid,"dchargeddaupid[3]/I");

}

void gensignalget::Read_tracktree(TTree* T)
{

   T->SetBranchAddress("mult", &mult);
   T->SetBranchAddress("pt", pt);
   T->SetBranchAddress("eta", eta);
   T->SetBranchAddress("phi", phi);
   T->SetBranchAddress("pdg", pdg);
   T->SetBranchAddress("chg", chg);
   T->SetBranchAddress("matchingID", matchingID);
   T->SetBranchAddress("nMothers", nMothers);
   T->SetBranchAddress("motherIdx", motherIdx);
   T->SetBranchAddress("nDaughters", nDaughters);
   T->SetBranchAddress("daughterIdx", daughterIdx);
   T->SetBranchAddress("sta", sta);
	
}

//
//
void gensignalget::LoopOverFile(int startFile, int endFile, char *filelist, int decaychannel) 
{//.. loop over files in the "*filelist" from startFile to endFile in sequence
    //

    Init(startFile, endFile, filelist);

    //
    ifstream file_stream(filelist);
    int ifile = 0;
    while (file_stream >> filename) {

        ifile++;
        if(ifile-1 >= endFile) {
            break;
        } else if(ifile-1 < startFile) {
            continue;
        }


        cout << "file  = " << ifile<<" name = "<<filename <<endl;

        if (!TFile::Open(filename.c_str()))   { cout << " fail to open file" << endl; continue;}

        //
        TFile *f = TFile::Open(filename.c_str());


        GenParticleTree  = (TTree*)f->Get("HiGenParticleAna/hi");

        if(!GenParticleTree) {
            cout<<"==> empty tree <=="<<endl;
            continue;
        }

		Read_tracktree( GenParticleTree );

//		Find_d0(GenParticleTree);
        
        Find_dcharged(GenParticleTree);

//        Find_dstar(GenParticleTree);

//		Find_dsphi(GenParticleTree);

        f->Close();
    }


    write();
}
//
void gensignalget::write()
{//.. results 
    result->cd();
	d0tree->Write();
	dstartree->Write();
	dsphitree->Write();
	dchargedtree->Write();
    result->Close();
}

//

void gensignalget::Find_d0(TTree* T)
{
	for(int entry = 0; entry<T->GetEntries(); entry++) 
	{
		T->GetEntry(entry);
//		cout << "number of particles: " << mult << endl;
		for ( int particle = 0; particle < mult; particle++)
		{
	    	if( abs(pdg[particle]) != 421 )   continue;
	    	if( nDaughters[particle] != 2 )   continue;
            if( sta[particle] != 2 )  continue;

		    int dau1 = daughterIdx[particle][0];
		    int dau2 = daughterIdx[particle][1];

			if( !( ( abs(pdg[dau1]) == 321 && abs(pdg[dau2]) == 211 ) || ( abs(pdg[dau1]) == 211 && abs(pdg[dau2]) == 321 ) ) )   continue;
			if( chg[dau1] * chg[dau2]  > 0 ) continue;


            d0pt = pt[particle];
            d0eta = eta[particle];
            d0phi = phi[particle];
            d0pid = pdg[particle];
            d0nofdau = nDaughters[particle];
            d0daupt[0] = pt[dau1];
            d0daupt[1] = pt[dau2];
            d0daueta[0] = eta[dau1];
            d0daueta[1] = eta[dau2];
            d0dauphi[0] = phi[dau1];
            d0dauphi[1] = phi[dau2];
            d0daupid[0] = pdg[dau1];
            d0daupid[1] = pdg[dau2];

			d0tree->Fill();

		}
	}

}

void gensignalget::Find_dstar(TTree* T)
{//.. loop over each event. Add different analysis function here. 

    for(int i = 0; i<T->GetEntries(); i++) {
//   for(int i = 0; i<10; i++) {

        T->GetEntry(i);

		for ( int particle = 0; particle < mult; particle++)
		{
	    	if( abs(pdg[particle]) != 413 )   continue;
	    	if( nDaughters[particle] != 2 )   continue;
            if( sta[particle] != 2 )  continue;

			int dau1 = 0;
			int dau2 = 0;
			int dau3 = 0;
			int d0_index;

		    int tmpdau1 = daughterIdx[particle][0];
			int tmpdau2 = daughterIdx[particle][1];

			if( !( ( abs(pdg[tmpdau1]) == 421 && abs(pdg[tmpdau2]) == 211 ) || ( abs(pdg[tmpdau1]) == 211 && abs(pdg[tmpdau2]) == 421 ) ))
				continue;

			if( abs( pdg[tmpdau1] ) == 211 )    { dau1 = tmpdau1; d0_index = tmpdau2; }
			else                                { dau1 = tmpdau2; d0_index = tmpdau1; }

			if( nDaughters[d0_index] != 2 )   continue;
			if( sta[d0_index] != 2 )  continue;

			dau2 = daughterIdx[d0_index][0];
			dau3 = daughterIdx[d0_index][1];

		    if( !( ( abs(pdg[dau2]) == 321 && abs(pdg[dau3]) == 211 ) || ( abs(pdg[dau2]) == 211 && abs(pdg[dau3]) == 321 ) ) )   continue;	

            dstarpt = pt[particle];
            dstareta = eta[particle];
            dstarphi = phi[particle];
            dstarpid = pdg[particle];
            dstarnofdau = nDaughters[particle];
            dstardaupt[0] = pt[dau1];
            dstardaupt[1] = pt[dau2];
			dstardaupt[2] = pt[dau3];
            dstardaueta[0] = eta[dau1];
            dstardaueta[1] = eta[dau2];
			dstardaueta[2] = eta[dau3];
            dstardauphi[0] = phi[dau1];
            dstardauphi[1] = phi[dau2];
			dstardauphi[2] = phi[dau3];
            dstardaupid[0] = pdg[dau1];
            dstardaupid[1] = pdg[dau2];
			dstardaupid[2] = pdg[dau3];

			dstartree->Fill();

		}

    }
}
//
//
void gensignalget::Find_dcharged(TTree* T)
{//.. loop over each event. Add different analysis function here. 

    for(int i = 0; i<T->GetEntries(); i++) {
//   for(int i = 0; i<10; i++) {

        T->GetEntry(i);

		for ( int particle = 0; particle < mult; particle++)
		{
	    	if( abs(pdg[particle]) != 411 )   continue;
	    	if( nDaughters[particle] != 3 )   continue;
            if( abs(sta[particle]) != 2 )  continue;

//			cout << "dcharged pt : " <<  pt[particle] << endl;

		    int dau1 = daughterIdx[particle][0];
		    int dau2 = daughterIdx[particle][1];
			int dau3 = daughterIdx[particle][2];

//			cout << pdg[dau1] << "   " << pdg[dau2] << "    " << pdg[dau3] << endl;

			if( !( ( abs(pdg[dau1]) == 321 && abs(pdg[dau2]) == 211 && abs(pdg[dau3]) == 211) || ( abs(pdg[dau1]) == 211 && abs(pdg[dau2]) == 321 && abs(pdg[dau3]) == 211 ) || (abs(pdg[dau1]) == 211 && abs(pdg[dau2]) == 211 && abs(pdg[dau3]) == 321 )) )   continue;
			

            dchargedpt = pt[particle];
            dchargedeta = eta[particle];
            dchargedphi = phi[particle];
            dchargedpid = pdg[particle];
            dchargednofdau = nDaughters[particle];
            dchargeddaupt[0] = pt[dau1];
            dchargeddaupt[1] = pt[dau2];
			dchargeddaupt[2] = pt[dau3];
            dchargeddaueta[0] = eta[dau1];
            dchargeddaueta[1] = eta[dau2];
			dchargeddaueta[2] = eta[dau3];
            dchargeddauphi[0] = phi[dau1];
            dchargeddauphi[1] = phi[dau2];
			dchargeddauphi[2] = phi[dau3];
            dchargeddaupid[0] = pdg[dau1];
            dchargeddaupid[1] = pdg[dau2];
			dchargeddaupid[2] = pdg[dau3];

			dchargedtree->Fill();

		}

    }
}
//

void gensignalget::Find_dsphi(TTree* T)
{
   for(int i = 0; i<T->GetEntries(); i++) 
   {
	   T->GetEntry(i);
		
		for ( int particle = 0; particle < mult; particle++)
		{
	    	if( abs(pdg[particle]) != 431 )   continue;
	    	if( nDaughters[particle] != 2 )   continue;
            if( sta[particle] != 2 )  continue;

			int dau1 = 0;
			int dau2 = 0;
			int dau3 = 0;
			int phi_index;

		    int tmpdau1 = daughterIdx[particle][0];
			int tmpdau2 = daughterIdx[particle][1];

			if( !( ( abs(pdg[tmpdau1]) == 333 && abs(pdg[tmpdau2]) == 211 ) || ( abs(pdg[tmpdau1]) == 211 && abs(pdg[tmpdau2]) == 333 ) ))
				continue;

			if( abs( pdg[tmpdau1] ) == 211 )    { dau1 = tmpdau1; phi_index = tmpdau2; }
			else                                { dau1 = tmpdau2; phi_index = tmpdau1; }

			if( nDaughters[phi_index] != 2 )   continue;
			if( sta[phi_index] != 2 )  continue;

			dau2 = daughterIdx[phi_index][0];
			dau3 = daughterIdx[phi_index][1];

		    if( !( ( abs(pdg[dau2]) == 321 && abs(pdg[dau3]) == 321 ) ) )   continue;	

            dsphipt = pt[particle];
            dsphieta = eta[particle];
            dsphiphi = phi[particle];
            dsphipid = pdg[particle];
            dsphinofdau = nDaughters[particle];
            dsphidaupt[0] = pt[dau1];
            dsphidaupt[1] = pt[dau2];
			dsphidaupt[2] = pt[dau3];
            dsphidaueta[0] = eta[dau1];
            dsphidaueta[1] = eta[dau2];
			dsphidaueta[2] = eta[dau3];
            dsphidauphi[0] = phi[dau1];
            dsphidauphi[1] = phi[dau2];
			dsphidauphi[2] = phi[dau3];
            dsphidaupid[0] = pdg[dau1];
            dsphidaupid[1] = pdg[dau2];
			dsphidaupid[2] = pdg[dau3];

			dsphitree->Fill();

		}
   }

}

