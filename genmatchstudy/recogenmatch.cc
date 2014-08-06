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
#include "recogenmatch.hh"
#include <iomanip>
#include "./../interface/hfcand_v0.hh"
//
recogenmatch::recogenmatch()
{
}

//
recogenmatch::~recogenmatch()
{
//    delete filename;
    delete result;

}
//
void recogenmatch::Init(int startFile, int endFile, char *filelist)
{//.. called right after constructor 
    char * outfilename = new char[1000];
    sprintf(outfilename, "recogenmatch_%s_%dTo%d.root",  filelist, startFile, endFile);
    result = new TFile(outfilename, "recreate");

    //

    hftree = 0;
	genparticletree = 0;
    hfcandidate = new hfcand_v0;

	define_cuts();

   d0tree = new TTree("d0tree","d0tree");
   d0tree->Branch("d0pt",&d0pt,"d0pt/D");
   d0tree->Branch("d0eta",&d0eta,"d0eta/D");
   d0tree->Branch("d0phi",&d0phi,"d0phi/D");
   d0tree->Branch("d0pid",&d0pid,"d0pid/I");
   d0tree->Branch("d0fm",&d0fm,"d0fm/D");
   d0tree->Branch("d0fpt",&d0fpt,"d0fpt/D");
   d0tree->Branch("d0ffls3d",&d0ffls3d,"d0ffls3d/D");
   d0tree->Branch("d0alpha",&d0alpha,"d0alpha/D");
   d0tree->Branch("d0fdr",&d0fdr,"d0fdr/D");
   d0tree->Branch("d0fchi2",&d0fchi2,"d0fchi2/D");
   d0tree->Branch("d0fprob",&d0fprob,"d0fprob/D");
   d0tree->Branch("d0passingtopocuts",&d0passingtopocuts,"d0passingtopocuts/I");

   dstartree = new TTree("dstartree","dstartree");
   dstartree->Branch("dstarpt",&dstarpt,"dstarpt/D");
   dstartree->Branch("dstareta",&dstareta,"dstareta/D");
   dstartree->Branch("dstarphi",&dstarphi,"dstarphi/D");
   dstartree->Branch("dstarpid",&dstarpid,"dstarpid/I");
   dstartree->Branch("dstarfm",&dstarfm,"dstarfm/D");
   dstartree->Branch("dstarfpt",&dstarfpt,"dstarfpt/D");
   dstartree->Branch("dstarffls3d",&dstarffls3d,"dstarffls3d/D");
   dstartree->Branch("dstaralpha",&dstaralpha,"dstaralpha/D");
   dstartree->Branch("dstarfdr",&dstarfdr,"dstarfdr/D");
   dstartree->Branch("dstarfchi2",&dstarfchi2,"dstarfchi2/D");
   dstartree->Branch("dstarfprob",&dstarfprob,"dstarfprob/D");
   dstartree->Branch("dstarmass_dau",&dstarmass_dau,"dstarmass_dau/D");
   dstartree->Branch("dstarpassingtopocuts",&dstarpassingtopocuts,"dstarpassingtopocuts/I");


   dchargedtree = new TTree("dchargedtree","dchargedtree");
   dchargedtree->Branch("dchargedpt",&dchargedpt,"dchargedpt/D");
   dchargedtree->Branch("dchargedeta",&dchargedeta,"dchargedeta/D");
   dchargedtree->Branch("dchargedphi",&dchargedphi,"dchargedphi/D");
   dchargedtree->Branch("dchargedpid",&dchargedpid,"dchargedpid/I");
   dchargedtree->Branch("dchargedfm",&dchargedfm,"dchargedfm/D");
   dchargedtree->Branch("dchargedfpt",&dchargedfpt,"dchargedfpt/D");
   dchargedtree->Branch("dchargedffls3d",&dchargedffls3d,"dchargedffls3d/D");
   dchargedtree->Branch("dchargedalpha",&dchargedalpha,"dchargedalpha/D");
   dchargedtree->Branch("dchargedfdr",&dchargedfdr,"dchargedfdr/D");
   dchargedtree->Branch("dchargedfchi2",&dchargedfchi2,"dchargedfchi2/D");
   dchargedtree->Branch("dchargedfprob",&dchargedfprob,"dchargedfprob/D");
   dchargedtree->Branch("dchargedmass_dau",&dchargedmass_dau,"dchargedmass_dau/D");
   dchargedtree->Branch("dchargedpassingtopocuts",&dchargedpassingtopocuts,"dchargedpassingtopocuts/I");


}


//
//
void recogenmatch::LoopOverFile(int startFile, int endFile, char *filelist, int decaychannel) 
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


//        hftree  = (TTree*)f->Get("RegitHFtree/hftree");
        hftree  = (TTree*)f->Get("HFtree/hftree");
		genparticletree = (TTree*)f->Get("HiGenParticleAna/hi");

        if(!hftree || !genparticletree) {
            cout<<"==> empty tree <=="<<endl;
            continue;
        }

		hftree->AddFriend(genparticletree);
		
		cout << " entries :" << hftree->GetEntries() << "    " << genparticletree->GetEntries() << endl;

		hftree->SetBranchAddress("hfcandidate", &hfcandidate);
        genparticletree->SetBranchAddress("mult", &mult);
        genparticletree->SetBranchAddress("pt", pt);
        genparticletree->SetBranchAddress("eta", eta);
        genparticletree->SetBranchAddress("phi", phi);
        genparticletree->SetBranchAddress("pdg", pdg);
        genparticletree->SetBranchAddress("chg", chg);
        genparticletree->SetBranchAddress("matchingID", matchingID);
        genparticletree->SetBranchAddress("nMothers", nMothers);
        genparticletree->SetBranchAddress("motherIdx", motherIdx);
        genparticletree->SetBranchAddress("nDaughters", nDaughters);
        genparticletree->SetBranchAddress("daughterIdx", daughterIdx);
        genparticletree->SetBranchAddress("sta", sta);

		Find_d0(hftree);
        
        Find_dcharged(hftree);

		Find_dstar(hftree);

        f->Close();
    }


    write();
}
//
void recogenmatch::write()
{//.. results 
    result->cd();
	d0tree->Write();
	dstartree->Write();
	dchargedtree->Write();
    result->Close();
}

//

void recogenmatch::Find_d0(TTree* T)
{
	for(int entry = 0; entry<T->GetEntries(); entry++) 
	{
		T->GetEntry(entry);

        for(int icand = 0; icand<hfcandidate->get_nhfcand(); icand++) {

			snglhfcand* cand = hfcandidate->get_hfcand(icand);
			if(cand->get_type() != 2) continue;

                float mass = cand->get_fm();
                float fpt = cand->get_fpt();
                float feta = cand->get_feta();
                float ffls3d = cand->get_ffls3d();
                float alpha = cand->get_falpha0();
                float fprob = cand->get_fprob();
                float fdr = cand->get_fdr();
                float fchi2 = cand->get_fchi2();
                float mass_dau = cand->get_fmdau1();
           
				vector<int>  gIndex1 = cand->get_gIndex1();
				vector<int>  gIndex2 = cand->get_gIndex2();


				for( int n1 = 0; n1 < gIndex1.size(); n1++ )
				{
					for( int n2 = 0; n2 < gIndex2.size(); n2++ )
					{

            				int dau1 = gIndex1[n1];
            				int dau2 = gIndex2[n2];
            
            				if ( dau1 == -999 || dau2 == -999 )     continue;

							//mathed to two different gen particles
							if ( dau1 == dau2 )     continue;
							

                            // one kaon, one pion 
                            if( ! ( abs(pdg[dau1]) == 211 && abs(pdg[dau2]) == 321)  )   
								continue;
 
                            //only one mother
							if (!( nMothers[dau1] == 1 && nMothers[dau2] == 1 ))   continue;

                            //mother index the same and not -999
							if ( !( motherIdx[dau1][0] == motherIdx[dau2][0] && motherIdx[dau1][0] != -999 ) )
								continue;

							int allmotherindex = motherIdx[dau1][0];
							//is D charged and just three daughters
							if( ! (abs (pdg[allmotherindex]) == 421 && nDaughters[allmotherindex] == 2) )  continue;

							cout << "candidate mathed to gen" << endl;

							d0pt = pt[allmotherindex];
							d0eta = eta[allmotherindex];
							d0phi = phi[allmotherindex];
							d0pid = pdg[allmotherindex];
							d0fm = mass;
							d0fpt = fpt;
							d0ffls3d = ffls3d;
							d0alpha = alpha;
							d0fprob = fprob;
							d0fdr = fdr;
							d0fchi2 = fchi2;
							d0passingtopocuts = 0;
							if(mass > cut_m_low[1] && mass < cut_m_high[1] && ffls3d > cut_ffls3d[1] && alpha < cut_falpha0[1] && fprob > cut_fprob[1] && fdr < cut_fdr[1] && fchi2 < cut_fchi2[1]) 
                               d0passingtopocuts = 1;

							d0tree->Fill();

            
					}
				}

		}


	}

}

//
void recogenmatch::Find_dcharged(TTree* T)
{//.. loop over each event. Add different analysis function here. 

    for(int entry = 0; entry<T->GetEntries(); entry++) {
//   for(int i = 0; i<10; i++) {

        T->GetEntry(entry);

		for(int icand = 0; icand<hfcandidate->get_nhfcand(); icand++) {


			snglhfcand* cand = hfcandidate->get_hfcand(icand);
			if(cand->get_type() != 5) continue;

                float mass = cand->get_fm();
                float fpt = cand->get_fpt();
                float feta = cand->get_feta();
                float ffls3d = cand->get_ffls3d();
                float alpha = cand->get_falpha0();
                float fprob = cand->get_fprob();
                float fdr = cand->get_fdr();
                float fchi2 = cand->get_fchi2();
                float mass_dau = cand->get_fmdau1();
           
                vector<int>  gIndex_dau2 = cand->get_gIndex_dau2();
				vector<int>  gIndex1 = cand->get_gIndex1();
				vector<int>  gIndex2 = cand->get_gIndex2();

//				if ( !( gIndex_dau2.size() == 1 && gIndex1.size() == 1 && gIndex2.size() == 1 ))    { cout << " multi match" << endl; continue;}
//
//				cout << "alpha: " << alpha << endl;

				for( int n1 = 0; n1 < gIndex1.size(); n1++ )
				{
					for( int n2 = 0; n2 < gIndex2.size(); n2++ )
					{
						for ( int n3 = 0; n3 < gIndex_dau2.size(); n3++ )
						{

            				int dau1 = gIndex1[n1];
            				int dau2 = gIndex2[n2];
            				int dau3 = gIndex_dau2[n3];
            
            				if ( dau3 == -999 || dau1 == -999 || dau2 == -999 )     continue;

							//mathed to three different gen particles
							if ( dau1 == dau2 || dau1 == dau3 || dau2 == dau3 )     continue;
							

                            // one kaon, two pion 
                            if( !(( abs(pdg[dau1]) == 211 && abs(pdg[dau2]) == 321 && abs(pdg[dau3]) == 211 )) )   
								continue;
 
                            //only one mother
							if (!( nMothers[dau1] == 1 && nMothers[dau2] == 1 && nMothers[dau3] == 1 ))   continue;

                            //mother index the same and not -999
							if ( !( motherIdx[dau1][0] == motherIdx[dau2][0]  && motherIdx[dau1][0] == motherIdx[dau3][0] && motherIdx[dau1][0] != -999 ) )
								continue;

							int allmotherindex = motherIdx[dau1][0];
							//is D charged and just three daughters
							if( ! (abs (pdg[allmotherindex]) == 411 && nDaughters[allmotherindex] == 3) )  continue;

							cout << "candidate mathed to gen" << endl;

							dchargedpt = pt[allmotherindex];
							dchargedeta = eta[allmotherindex];
							dchargedphi = phi[allmotherindex];
							dchargedpid = pdg[allmotherindex];
							dchargedfm = mass;
							dchargedfpt = fpt;
							dchargedffls3d = ffls3d;
							dchargedalpha = alpha;
							dchargedfprob = fprob;
							dchargedfdr = fdr;
							dchargedfchi2 = fchi2;
							dchargedmass_dau = mass_dau;
                            dchargedpassingtopocuts = 0;
                            if(mass > cut_m_low[2] && mass < cut_m_high[2] && ffls3d > cut_ffls3d[2] && alpha < cut_falpha0[2] && fprob > cut_fprob[2] && fdr < cut_fdr[2] && fchi2 < cut_fchi2[2] && mass_dau > cut_m_dau_low[2] && mass_dau < cut_m_dau_high[2] )
								dchargedpassingtopocuts = 1;

							dchargedtree->Fill();

            
						}
					}
				}

		}


    }
}
//

void recogenmatch::Find_dstar(TTree* T)
{
   for(int entry = 0; entry<T->GetEntries(); entry++) 
   {
	   T->GetEntry(entry);

		for(int icand = 0; icand<hfcandidate->get_nhfcand(); icand++) {
			snglhfcand* cand = hfcandidate->get_hfcand(icand);

			if(cand->get_type() != 1) continue;

                float mass = cand->get_fm();
                float fpt = cand->get_fpt();
                float feta = cand->get_feta();
                float ffls3d = cand->get_ffls3d();
                float alpha = cand->get_falpha0();
                float fprob = cand->get_fprob();
                float fdr = cand->get_fdr();
                float fchi2 = cand->get_fchi2();
                float mass_dau = cand->get_fmdau1();
           
                vector<int>  gIndex_dau2 = cand->get_gIndex_dau2();
				vector<int>  gIndex1 = cand->get_gIndex1();
				vector<int>  gIndex2 = cand->get_gIndex2();


				for( int n1 = 0; n1 < gIndex1.size(); n1++ )
				{
					for( int n2 = 0; n2 < gIndex2.size(); n2++ )
					{
						for ( int n3 = 0; n3 < gIndex_dau2.size(); n3++ )
						{

            				int dau1 = gIndex1[n1];
            				int dau2 = gIndex2[n2];
            				int dau3 = gIndex_dau2[n3];
            
            				if ( dau3 == -999 || dau1 == -999 || dau2 == -999 )     continue;

							//mathed to three different gen particles
							if ( dau1 == dau2 || dau1 == dau3 || dau2 == dau3 )     continue;
							

                            // one kaon, two pion 
						    if( !( ( abs(pdg[dau1]) == 211 && abs(pdg[dau2]) == 321 ) ) )
								continue;

							if ( abs(pdg[dau3]) != 211 )   continue;
 
                            //only one mother
							if (!( nMothers[dau1] == 1 && nMothers[dau2] == 1 && nMothers[dau3] == 1 ))   continue;

                            //mother index the same and not -999
							if ( !( motherIdx[dau1][0] == motherIdx[dau2][0] && motherIdx[dau1][0] != -999 ) )
								continue;

							int d0index = motherIdx[dau1][0];
							//is D charged and just three daughters
							if( ! (abs (pdg[d0index]) == 421 && nDaughters[d0index] == 2) )  continue;

							if( nMothers[d0index] != 1 || motherIdx[d0index][0] != motherIdx[dau3][0] || motherIdx[dau3][0] == -999 )    continue;

						    int allmotherindex = motherIdx[d0index][0];

							if( ! (abs(pdg[allmotherindex]) == 413 && nDaughters[allmotherindex] == 2) )  continue;

							cout << "candidate mathed to gen" << endl;

							dstarpt = pt[allmotherindex];
							dstareta = eta[allmotherindex];
							dstarphi = phi[allmotherindex];
							dstarpid = pdg[allmotherindex];
							dstarfm = mass;
							dstarfpt = fpt;
							dstarffls3d = ffls3d;
							dstaralpha = alpha;
							dstarfprob = fprob;
							dstarfdr = fdr;
							dstarfchi2 = fchi2;
							dstarmass_dau = mass_dau;
                            dstarpassingtopocuts = 0;
                            if(mass > cut_m_low[0] && mass < cut_m_high[0] && ffls3d > cut_ffls3d[0] && alpha < cut_falpha0[0] && fprob > cut_fprob[0] && fdr < cut_fdr[0] && fchi2 < cut_fchi2[0] && mass_dau > cut_m_dau_low[0] && mass_dau < cut_m_dau_high[0] )
								dstarpassingtopocuts = 1;

							dstartree->Fill();

            
						}
					}
				}

		}

   }

}

void recogenmatch::define_cuts()
{

    //D*->D0+pi
    cut_m_low[0] = 1.92;
    cut_m_high[0] = 2.10;
    cut_m_dau_low[0] = 1.8;
    cut_m_dau_high[0] = 1.92;
    cut_ffls3d[0] = 2.0;
    cut_falpha0[0] = 0.2;
    cut_fprob[0] = 0.05;
    cut_fdr[0] = 0.25;
    cut_fchi2[0] = 3;

    //D0->K+pi
    cut_m_low[1] = 1.70;
    cut_m_high[1] = 2.05;
    cut_m_dau_low[1] = -1;
    cut_m_dau_high[1] = 1;
    cut_ffls3d[1] = 2.0;
    cut_falpha0[1] = 0.2;
    cut_fprob[1] = 0.05;
    cut_fdr[1] = 0.25;
    cut_fchi2[1] = 3;

    //D+/- = K+pi+pi
    cut_m_low[2] = 1.70;
    cut_m_high[2] = 2.05;
    cut_m_dau_low[2] = 0.5;
    cut_m_dau_high[2] = 2.05;
    cut_ffls3d[2] = 2.0;
    cut_falpha0[2] = 0.2;
    cut_fprob[2] = 0.05;
    cut_fdr[2] = 0.25;
    cut_fchi2[2] = 3;


}
