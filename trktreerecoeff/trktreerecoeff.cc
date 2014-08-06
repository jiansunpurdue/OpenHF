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
#include "trktreerecoeff.hh"
#include <iomanip>
//
trktreerecoeff::trktreerecoeff()
{
}

//
trktreerecoeff::~trktreerecoeff()
{
//    delete filename;
    delete result;

}
//
void trktreerecoeff::Init(int startFile, int endFile, char *filelist)
{//.. called right after constructor 
    char * outfilename = new char[1000];
    sprintf(outfilename, "trktreerecoeff_%s_%dTo%d.root",  filelist, startFile, endFile);
    result = new TFile(outfilename, "recreate");

    //

    Tracktree = 0;
	Genparticletree = 0;


   d0tree = new TTree("d0tree","d0tree");
   d0tree->Branch("d0pt",&d0pt,"d0pt/D");
   d0tree->Branch("d0eta",&d0eta,"d0eta/D");
   d0tree->Branch("d0phi",&d0phi,"d0phi/D");
   d0tree->Branch("d0pid",&d0pid,"d0pid/I");
   d0tree->Branch("d0nofdau",&d0nofdau,"d0nofdau/I");
   d0tree->Branch("d0daupt",d0daupt,"d0daupt[2]/D");
   d0tree->Branch("d0daurecopt",d0daurecopt,"d0daurecopt[2]/D");
//   d0tree->Branch("d0daueta",d0daueta,"d0daueta[2]/D");
//   d0tree->Branch("d0dauphi",d0dauphi,"d0dauphi[2]/D");
   d0tree->Branch("d0daupid",d0daupid,"d0daupid[2]/I");
   d0tree->Branch("d0allhighpurity",&d0allhighpurity,"d0allhighpurity/I");

   dstartree = new TTree("dstartree","dstartree");
   dstartree->Branch("dstarpt",&dstarpt,"dstarpt/D");
   dstartree->Branch("dstareta",&dstareta,"dstareta/D");
   dstartree->Branch("dstarphi",&dstarphi,"dstarphi/D");
   dstartree->Branch("dstarpid",&dstarpid,"dstarpid/I");
   dstartree->Branch("dstarnofdau",&dstarnofdau,"dstarnofdau/I");
   dstartree->Branch("dstardaupt",dstardaupt,"dstardaupt[3]/D");
   dstartree->Branch("dstardaurecopt",dstardaurecopt,"dstardaurecopt[3]/D");
//   dstartree->Branch("dstardaueta",dstardaueta,"dstardaueta[3]/D");
//   dstartree->Branch("dstardauphi",dstardauphi,"dstardauphi[3]/D");
   dstartree->Branch("dstardaupid",dstardaupid,"dstardaupid[3]/I");
//   dstartree->Branch("dstardaumompid",dstardaumompid,"dstardaumompid[3]/I");
   dstartree->Branch("dstarallhighpurity",&dstarallhighpurity,"dstarallhighpurity/I");

   dsphitree = new TTree("dsphitree","dsphitree");
   dsphitree->Branch("dsphipt",&dsphipt,"dsphipt/D");
   dsphitree->Branch("dsphieta",&dsphieta,"dsphieta/D");
   dsphitree->Branch("dsphiphi",&dsphiphi,"dsphiphi/D");
   dsphitree->Branch("dsphipid",&dsphipid,"dsphipid/I");
   dsphitree->Branch("dsphinofdau",&dsphinofdau,"dsphinofdau/I");
   dsphitree->Branch("dsphidaupt",dsphidaupt,"dsphidaupt[3]/D");
   dsphitree->Branch("dsphidaurecopt",dsphidaurecopt,"dsphidaurecopt[3]/D");
//   dsphitree->Branch("dsphidaueta",dsphidaueta,"dsphidaueta[3]/D");
//   dsphitree->Branch("dsphidauphi",dsphidauphi,"dsphidauphi[3]/D");
   dsphitree->Branch("dsphidaupid",dsphidaupid,"dsphidaupid[3]/I");
//   dsphitree->Branch("dsphidaumompid",dsphidaumompid,"dsphidaumompid[3]/I");
   dsphitree->Branch("dsphiallhighpurity",&dsphiallhighpurity,"dsphiallhighpurity/I");



   dchargedtree = new TTree("dchargedtree","dchargedtree");
   dchargedtree->Branch("dchargedpt",&dchargedpt,"dchargedpt/D");
   dchargedtree->Branch("dchargedeta",&dchargedeta,"dchargedeta/D");
   dchargedtree->Branch("dchargedphi",&dchargedphi,"dchargedphi/D");
   dchargedtree->Branch("dchargedpid",&dchargedpid,"dchargedpid/I");
   dchargedtree->Branch("dchargednofdau",&dchargednofdau,"dchargednofdau/I");
   dchargedtree->Branch("dchargeddaupt",dchargeddaupt,"dchargeddaupt[3]/D");
   dchargedtree->Branch("dchargeddaurecopt",dchargeddaurecopt,"dchargeddaurecopt[3]/D");
//   dchargedtree->Branch("dchargeddaueta",dchargeddaueta,"dchargeddaueta[3]/D");
//   dchargedtree->Branch("dchargeddauphi",dchargeddauphi,"dchargeddauphi[3]/D");
   dchargedtree->Branch("dchargeddaupid",dchargeddaupid,"dchargeddaupid[3]/I");
   dchargedtree->Branch("dchargedallhighpurity",&dchargedallhighpurity,"dchargedallhighpurity/I");

}

void trktreerecoeff::Read_tree()
{

   Tracktree->SetBranchAddress("nTrk", &nTrk);
   Tracktree->SetBranchAddress("trkPt", trkPt);
   Tracktree->SetBranchAddress("trkEta", trkEta);
   Tracktree->SetBranchAddress("trkPhi", trkPhi);
   Tracktree->SetBranchAddress("highPurity", trkQual);
   Tracktree->SetBranchAddress("trkPId", trkPId);
   Tracktree->SetBranchAddress("trkMPId", trkMPId);
   Tracktree->SetBranchAddress("trkGMPId", trkGMPId);
   Tracktree->SetBranchAddress("matchedGenID",matchedGenID);

   Genparticletree->SetBranchAddress("mult", &mult);
   Genparticletree->SetBranchAddress("pt", pt);
   Genparticletree->SetBranchAddress("eta", eta);
   Genparticletree->SetBranchAddress("phi", phi);
   Genparticletree->SetBranchAddress("pdg", pdg);
   Genparticletree->SetBranchAddress("chg", chg);
   Genparticletree->SetBranchAddress("nMothers", nMothers);
   Genparticletree->SetBranchAddress("motherIdx", motherIdx);
   Genparticletree->SetBranchAddress("nDaughters", nDaughters);
   Genparticletree->SetBranchAddress("daughterIdx", daughterIdx);
   Genparticletree->SetBranchAddress("sta", sta);
   Genparticletree->SetBranchAddress("matchingID", matchingID);

	
}

//
//
void trktreerecoeff::LoopOverFile(int startFile, int endFile, char *filelist, int decaychannel) 
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


        Tracktree  = (TTree*)f->Get("regitTrack/trackTree");
//        Tracktree  = (TTree*)f->Get("anaTrack/trackTree");
//        Tracktree  = (TTree*)f->Get("ppTrack/trackTree");
		Genparticletree = (TTree*)f->Get("HiGenParticleAna/hi");

        if(!Tracktree || !Genparticletree) {
            cout<<"==> empty tree <=="<<endl;
            continue;
        }

		Tracktree->AddFriend(Genparticletree);

		Read_tree();

		Find_d0dstar(Tracktree);
        
//        Find_dcharged(Tracktree);

//		Find_dsphi(Tracktree);

        f->Close();
    }


    write();
}
//
void trktreerecoeff::write()
{//.. results 
    result->cd();
	d0tree->Write();
	dstartree->Write();
	dsphitree->Write();
	dchargedtree->Write();
    result->Close();
}

//

void trktreerecoeff::Find_d0dstar(TTree* T)
{
	for(int entry = 0; entry<T->GetEntries(); entry++) 
	{
		T->GetEntry(entry);
		for ( int tk1 = 0; tk1 < nTrk -1 ; tk1++ )
		{
			if( matchedGenID[tk1][0] == -999 || matchedGenID[tk1][0] == 0 )    continue;       

			for ( int tk2 = tk1+1 ; tk2 < nTrk ; tk2++ )
			{
				if( matchedGenID[tk2][0] == -999 || matchedGenID[tk2][0] == 0 )    continue;

				int dau1 = matchedGenID[tk1][0];
				int dau2 = matchedGenID[tk2][0]; 

                //mathed to two different gen particles
                if ( dau1 == dau2 )     continue;

                // one kaon, one pion
                if( ! ( ( abs(pdg[dau1]) == 211 && abs(pdg[dau2]) == 321 ) || ( abs(pdg[dau1]) == 321 && abs(pdg[dau2]) == 211 ) )  )
                    continue;

                //only one mother
                if (!( nMothers[dau1] == 1 && nMothers[dau2] == 1 ))   continue;

                //mother index the same and not -999
                if ( !( motherIdx[dau1][0] == motherIdx[dau2][0] && motherIdx[dau1][0] != -999 ) )
                    continue;

                int d0index = motherIdx[dau1][0];
                //is D charged and just three daughters
                if( ! (abs (pdg[d0index]) == 421 && nDaughters[d0index] == 2) )  continue;
                
				cout << "reco D0" <<  endl;

                d0pt = pt[d0index];
                d0eta = eta[d0index];
                d0phi = phi[d0index];
                d0pid = pdg[d0index];
                d0nofdau = nDaughters[d0index];
                d0daupt[0] = pt[dau1];
                d0daupt[1] = pt[dau2];
                d0daurecopt[0] = trkPt[tk1];
                d0daurecopt[1] = trkPt[tk2];
                d0daupid[0] = pdg[dau1];
                d0daupid[1] = pdg[dau2];
				d0allhighpurity = 0;
				if( trkQual[tk1] && trkQual[tk2] )    d0allhighpurity = 1;

				d0tree->Fill();

				if( nMothers[d0index] != 1 )   continue;
				
				int d0mother = motherIdx[d0index][0];
				if ( !( abs (pdg[d0mother]) == 413 && nDaughters[d0mother] == 2 ))   continue;

				for( int tk3 = 0; tk3 < nTrk; tk3++ )
				{
					if( matchedGenID[tk3][0] == -999 || matchedGenID[tk3][0] == 0 )    continue;
					
					int dau3 = matchedGenID[tk3][0];
					
                    if( ! ( abs(pdg[dau3]) == 211 && nMothers[dau3] == 1 && motherIdx[dau3][0] == d0mother ))   continue;

					int dstar = d0mother;

					cout << "reco dstar " << endl;

                    dstarpt = pt[dstar];
                    dstareta = eta[dstar];
                    dstarphi = phi[dstar];
                    dstarpid = pdg[dstar] ;
                    dstarnofdau = nDaughters[dstar];
                    dstardaupt[0] = pt[dau1];
                    dstardaupt[1] = pt[dau2];
                    dstardaupt[2] = pt[dau3];
                    dstardaurecopt[0] = trkPt[tk1];
                    dstardaurecopt[1] = trkPt[tk2];
                    dstardaurecopt[2] = trkPt[tk3];
                    dstardaupid[0] = pdg[dau1];
                    dstardaupid[1] = pdg[dau2];
                    dstardaupid[2] = pdg[dau3];
					dstarallhighpurity = 0;

					if( trkQual[tk1] && trkQual[tk2] && trkQual[tk3] )    dstarallhighpurity = 1;

					dstartree->Fill();

				}




			}
		}

	}

}


void trktreerecoeff::Find_dcharged(TTree* T)
{
   for(int entry = 0; entry<T->GetEntries(); entry++) 
   {
	   T->GetEntry(entry);
	   for( int tk1 = 0; tk1 < nTrk - 2 ; tk1++)
	   {
		   if( matchedGenID[tk1][0] == -999 || matchedGenID[tk1][0] == 0 )    continue;
		   for ( int tk2 = tk1+1 ; tk2 < nTrk - 1 ; tk2++ )
		   {
			   if( matchedGenID[tk2][0] == -999 || matchedGenID[tk2][0] == 0 )    continue;
			   for ( int tk3 = tk2+1 ; tk3 < nTrk; tk3++ )
			   {
				   if( matchedGenID[tk3][0] == -999 || matchedGenID[tk3][0] == 0 )    continue;

				   int dau1 = matchedGenID[tk1][0];
				   int dau2 = matchedGenID[tk2][0];
				   int dau3 = matchedGenID[tk3][0];

				   if( dau1 == dau2 || dau1 == dau3 || dau2 == dau3 )   continue;

				   if( !( ( abs(pdg[dau1]) == 321 && abs(pdg[dau2]) == 211 && abs(pdg[dau3]) == 211) || ( abs(pdg[dau1]) == 211 && abs(pdg[dau2]) == 321 && abs(pdg[dau3]) == 211 ) || (abs(pdg[dau1]) == 211 && abs(pdg[dau2]) == 211 && abs(pdg[dau3]) == 321 )) )
					   continue;

				   if (!( nMothers[dau1] == 1 && nMothers[dau2] == 1 && nMothers[dau3] == 1 ))   continue;

                   if ( !( motherIdx[dau1][0] == motherIdx[dau2][0]  && motherIdx[dau1][0] == motherIdx[dau3][0] && motherIdx[dau1][0] != -999 ) )
					   continue;

				   int motherindex = motherIdx[dau1][0];
				   if( ! (abs (pdg[motherindex]) == 411 && nDaughters[motherindex] == 3) )  continue;

                   int dcharged = motherindex;
				   
				   cout << "reco dcharged" << endl;
                    
					dchargedpt = pt[dcharged];
                    dchargedeta = eta[dcharged];
                    dchargedphi = phi[dcharged];
                    dchargedpid = pdg[dcharged] ;
                    dchargednofdau = nDaughters[dcharged];
                    dchargeddaupt[0] = pt[dau1];
                    dchargeddaupt[1] = pt[dau2];
                    dchargeddaupt[2] = pt[dau3];
                    dchargeddaurecopt[0] = trkPt[tk1];
                    dchargeddaurecopt[1] = trkPt[tk2];
                    dchargeddaurecopt[2] = trkPt[tk3];
                    dchargeddaupid[0] = pdg[dau1];
                    dchargeddaupid[1] = pdg[dau2];
                    dchargeddaupid[2] = pdg[dau3];
					dchargedallhighpurity = 0;

					if( trkQual[tk1] && trkQual[tk2] && trkQual[tk3] )    dchargedallhighpurity = 1;

					dchargedtree->Fill();

			   }

		   }


       }


   }

}

//
void trktreerecoeff::Find_dsphi(TTree* T)
{//.. loop over each event. Add different analysis function here. 

    for(int entry = 0; entry<T->GetEntries(); entry++) {

        T->GetEntry(entry);


    }
}
//
