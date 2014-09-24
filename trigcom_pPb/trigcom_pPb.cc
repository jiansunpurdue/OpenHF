#include <fstream>
#include <iostream>
#include "TH1F.h"
#include "TFile.h"
#include "TSystem.h"
#include "TRandom.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TBranch.h"
#include "TMath.h"
#include <TF1.h>
#include "trigcom_pPb.hh"
#include <iomanip>

#define NBIN 35
#define MASS_LOW 1.70
#define MASS_HIGH 2.05

//
trigcom_pPb::trigcom_pPb()
{
}

//
trigcom_pPb::~trigcom_pPb()
{
//    delete filename;
    delete result;

}
//
void trigcom_pPb::Init(int startFile, int endFile, char *filelist)
{//.. called right after constructor 
    char * outfilename = new char[1000];
    sprintf(outfilename, "trigcom_pPb_%s_%dTo%d.root",  filelist, startFile, endFile);
    result = new TFile(outfilename, "recreate");

    //
	hftree = 0;
	HltTree = 0;
	jetObjTree = 0;
	genparticletree = 0;

	hfcandidate = new hfcand_v0;

	define_cuts();
	book_hist();


}

void trigcom_pPb::book_hist()
{
	TH1::SetDefaultSumw2();
	char* hname = new char[1000];
	char* pt_range = new char[1000];

	for ( int ih = 0; ih < NPT; ih++ )
	{
		float pt_low = ptbins[ih];
		float pt_high = ptbins[ih+1];
		sprintf(pt_range, "pt: %f %f", pt_low, pt_high);

        sprintf(hname, "hfg_all_%d", ih);
        hfg_all[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
        sprintf(hname, "hfg_jettrigfired_%d", ih);
        hfg_jettrigfired[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);

        sprintf(hname, "hfg_jet_combined_%d", ih);
        hfg_jet_combined[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
        sprintf(hname, "hfg_jet_combined_fill_%d", ih);
		hfg_jet_combined_fill[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
        sprintf(hname, "hfg_jet_mb_combined_%d", ih);
        hfg_jet_mb_combined[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);

		sprintf(hname, "hfg_zerobias_raw_%d", ih);
        hfg_zerobias_raw[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
        sprintf(hname, "hfg_jet20_raw_%d", ih);
        hfg_jet20_raw[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
        sprintf(hname, "hfg_jet40_raw_%d", ih);
        hfg_jet40_raw[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
        sprintf(hname, "hfg_jet60_raw_%d", ih);
        hfg_jet60_raw[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
        sprintf(hname, "hfg_jet80_raw_%d", ih);
        hfg_jet80_raw[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
        sprintf(hname, "hfg_jet100_raw_%d", ih);
        hfg_jet100_raw[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);

        sprintf(hname, "hfg_zerobias_%d", ih);
        hfg_zerobias[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
        sprintf(hname, "hfg_jet20_%d", ih);
        hfg_jet20[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet40_%d", ih);
        hfg_jet40[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet60_%d", ih);
        hfg_jet60[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet80_%d", ih);
        hfg_jet80[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet100_%d", ih);
        hfg_jet100[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);

        sprintf(hname, "hfg_mb_jetpt20_raw_%d", ih);
        hfg_mb_jetpt20_raw[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet20_jetpt40_raw_%d", ih);
        hfg_jet20_jetpt40_raw[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet40_jetpt60_raw_%d", ih);
        hfg_jet40_jetpt60_raw[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet60_jetpt80_raw_%d", ih);
        hfg_jet60_jetpt80_raw[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet80_jetpt100_raw_%d", ih);
        hfg_jet80_jetpt100_raw[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet100_jetpt200_raw_%d", ih);
        hfg_jet100_jetpt200_raw[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);


        sprintf(hname, "hfg_mb_jetpt20_%d", ih);
        hfg_mb_jetpt20[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet20_jetpt40_%d", ih);
        hfg_jet20_jetpt40[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet40_jetpt60_%d", ih);
        hfg_jet40_jetpt60[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet60_jetpt80_%d", ih);
        hfg_jet60_jetpt80[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet80_jetpt100_%d", ih);
        hfg_jet80_jetpt100[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);
		sprintf(hname, "hfg_jet100_jetpt200_%d", ih);
        hfg_jet100_jetpt200[ih] = new TH1F( hname, pt_range, NBIN, MASS_LOW, MASS_HIGH);

	}

}


//
//
void trigcom_pPb::LoopOverFile(int startFile, int endFile, char *filelist, int decaychannel) 
{//.. loop over files in the "*filelist" from startFile to endFile in sequence
    //
	TH1::SetDefaultSumw2();

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

		hftree = (TTree*)f->Get("HFtree/hftree");
		HltTree = (TTree*)f->Get("hltanalysis/HltTree");
		jetObjTree = (TTree*)f->Get("hltobject/jetObjTree");
		genparticletree = (TTree*)f->Get("HiGenParticleAna/hi");


        if(!hftree || !HltTree || !jetObjTree || !genparticletree) {
            cout<<"==> empty tree <=="<<endl;
            continue;
        }

		hftree->AddFriend(HltTree);
		hftree->AddFriend(jetObjTree);
		hftree->AddFriend(genparticletree);
		hftree->SetBranchAddress("hfcandidate", &hfcandidate);

		get_trg_info();

		LoopOverEvt(hftree);


        f->Close();
    }


    write();
}

void trigcom_pPb::get_trg_info()
{
    HltTree->SetBranchAddress("HLT_PAJet20_NoJetID_v1",&Jet20);
	HltTree->SetBranchAddress("HLT_PAJet20_NoJetID_v1_Prescl",&Jet20_Prescl);
	HltTree->SetBranchAddress("HLT_PAJet40_NoJetID_v1",&Jet40);
	HltTree->SetBranchAddress("HLT_PAJet40_NoJetID_v1_Prescl",&Jet40_Prescl);
	HltTree->SetBranchAddress("HLT_PAJet60_NoJetID_v1",&Jet60);
	HltTree->SetBranchAddress("HLT_PAJet60_NoJetID_v1_Prescl",&Jet60_Prescl);
	HltTree->SetBranchAddress("HLT_PAJet80_NoJetID_v1",&Jet80);
	HltTree->SetBranchAddress("HLT_PAJet80_NoJetID_v1_Prescl",&Jet80_Prescl);
	HltTree->SetBranchAddress("HLT_PAJet100_NoJetID_v1",&Jet100);
	HltTree->SetBranchAddress("HLT_PAJet100_NoJetID_v1_Prescl",&Jet100_Prescl);
	HltTree->SetBranchAddress("HLT_PAZeroBiasPixel_SingleTrack_v1",&ZeroBias);
	HltTree->SetBranchAddress("HLT_PAZeroBiasPixel_SingleTrack_v1_Prescl",&ZeroBias_Prescl);
	HltTree->SetBranchAddress("L1_ZeroBias",&L1_ZeroBias);
	HltTree->SetBranchAddress("L1_ZeroBias_Prescl",&L1_ZeroBias_Prescl);
	HltTree->SetBranchAddress("L1_SingleJet12_BptxAND",&L1_SingleJet12);
	HltTree->SetBranchAddress("L1_SingleJet12_BptxAND_Prescl",&L1_SingleJet12_Prescl);
	HltTree->SetBranchAddress("L1_SingleJet16_BptxAND",&L1_SingleJet16);
	HltTree->SetBranchAddress("L1_SingleJet16_BptxAND_Prescl",&L1_SingleJet16_Prescl);
	HltTree->SetBranchAddress("L1_SingleJet36",&L1_SingleJet36);
	HltTree->SetBranchAddress("L1_SingleJet36_Prescl",&L1_SingleJet36_Prescl);

    //information from hltobject
    jetObjTree->SetBranchAddress("pt", &maxJetTrgPt);
    jetObjTree->SetBranchAddress("eta", &maxJetTrgEta);
    jetObjTree->SetBranchAddress("phi", &maxJetTrgPhi);
    jetObjTree->SetBranchAddress("mass", &maxJetTrgMass);

    //gen particle tree
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


}

void trigcom_pPb::LoopOverEvt( TTree * T)
{
	for(int i = 0; i<T->GetEntries(); i++) {
//    for(int i = 0; i<10000; i++) {

		T->GetEntry(i);
//		cout << "ZeroBias: " << ZeroBias << "             ZeroBias_Prescl: " << ZeroBias << endl;

		LoopOverHFCandidate();
		
		}

}

//
void trigcom_pPb::LoopOverHFCandidate()
{//.. loop over Heavy Flavor candidate in a event
    for(int i = 0; i<hfcandidate->get_nhfcand(); i++) {
        snglhfcand* cand = hfcandidate->get_hfcand(i);

		if ( cand->get_type() != 2 )   continue;

        type = cand->get_type();
        mass = cand->get_fm();
        fpt = cand->get_fpt();
        feta = cand->get_feta();
        ffls3d = cand->get_ffls3d();
        alpha = cand->get_falpha0();
        fprob = cand->get_fprob();
        fdr = cand->get_fdr();
        fchi2 = cand->get_fchi2();
        mass_dau = cand->get_fmdau1();
        fpt1 = cand->get_fpt1();
        fpt2 = cand->get_fpt2();
		fq1  = cand->get_fq1();
		fq2  = cand->get_fq2();

        gIndex1 = cand->get_gIndex1();
        gIndex2 = cand->get_gIndex2();
        gIndex_dau2 = cand->get_gIndex_dau2();

		if( feta < -2 || feta > 2 )  continue;
		if( fq1 * fq2 > 0 )   continue;

		if( ! ( mass > cut_m_low[1] && mass < cut_m_high[1] && ffls3d > cut_ffls3d[1] && alpha < cut_falpha0[1] && fprob > cut_fprob[1] && fdr < cut_fdr[1] && fchi2 < cut_fchi2[1] ))  continue; 

		if ( !Find_d0() )  continue;
//		cout << "matched D0 " << endl;

		int ipt = decideptbin(fpt);

//		cout << "ipt : " << ipt << endl;

		hfg_all[ipt]->Fill(mass);

		if ( ! ( ZeroBias || Jet20 || Jet40 || Jet60 || Jet80 || Jet100 ) )   continue;


		if( Jet20 || Jet40 || Jet60 || Jet80 || Jet100 )   hfg_jettrigfired[ipt]->Fill(mass);

		if ( ZeroBias )    hfg_zerobias_raw[ipt]->Fill(mass);
		if( Jet20 )   hfg_jet20_raw[ipt]->Fill(mass);
		if( Jet40 )   hfg_jet40_raw[ipt]->Fill(mass);
		if( Jet60 )   hfg_jet60_raw[ipt]->Fill(mass);
		if( Jet80 )   hfg_jet80_raw[ipt]->Fill(mass);
		if( Jet100 )   hfg_jet100_raw[ipt]->Fill(mass);

        if( ZeroBias && maxJetTrgPt < 20 )                       hfg_mb_jetpt20_raw[ipt]->Fill(mass);
		if( Jet20 && maxJetTrgPt >= 20 && maxJetTrgPt < 40 )     hfg_jet20_jetpt40_raw[ipt]->Fill(mass);
		if( Jet40 && maxJetTrgPt >= 40 && maxJetTrgPt < 60 )     hfg_jet40_jetpt60_raw[ipt]->Fill(mass);
		if( Jet60 && maxJetTrgPt >= 60 && maxJetTrgPt < 80 )     hfg_jet60_jetpt80_raw[ipt]->Fill(mass);
		if( Jet80 && maxJetTrgPt >= 80 && maxJetTrgPt < 100 )    hfg_jet80_jetpt100_raw[ipt]->Fill(mass);
		if( Jet100 && maxJetTrgPt >= 100 )                       hfg_jet100_jetpt200_raw[ipt]->Fill(mass);

		float prob = gRandom->Uniform(0 , 1.0);

		if( prob > (1.0 - 1.0/Prescl[0]) && ZeroBias )    hfg_zerobias[ipt]->Fill(mass,Prescl[0]);
		if( prob > (1.0 - 1.0/Prescl[1]) && Jet20 )       hfg_jet20[ipt]->Fill(mass,Prescl[1]);
		if( prob > (1.0 - 1.0/Prescl[2]) && Jet40 )       hfg_jet40[ipt]->Fill(mass,Prescl[2]);
		if( prob > (1.0 - 1.0/Prescl[3]) && Jet60 )       hfg_jet60[ipt]->Fill(mass,Prescl[3]);
		if( prob > (1.0 - 1.0/Prescl[4]) && Jet80 )       hfg_jet80[ipt]->Fill(mass,Prescl[4]);
		if( prob > (1.0 - 1.0/Prescl[5]) && Jet100 )      hfg_jet100[ipt]->Fill(mass,Prescl[5]);
		
		if( prob > (1.0 - 1.0/Prescl[0]) && ZeroBias && maxJetTrgPt < 20 )           hfg_mb_jetpt20[ipt]->Fill(mass, Prescl[0]);

		if( prob > (1.0 - 1.0/Prescl[1]) && Jet20 && maxJetTrgPt >= 20 && maxJetTrgPt < 40 )  
		{ 
		    hfg_jet20_jetpt40[ipt]->Fill(mass,Prescl[1]); 
		    hfg_jet_combined_fill[ipt]->Fill(mass,Prescl[1]); 
		}
		
		if( prob > (1.0 - 1.0/Prescl[2]) && Jet40 && maxJetTrgPt >= 40 && maxJetTrgPt < 60 )  
		{
		    hfg_jet40_jetpt60[ipt]->Fill(mass,Prescl[2]);
		    hfg_jet_combined_fill[ipt]->Fill(mass,Prescl[2]);
		}
		
		if( prob > (1.0 - 1.0/Prescl[3]) && Jet60 && maxJetTrgPt >= 60 && maxJetTrgPt < 80 )  
		{
			hfg_jet60_jetpt80[ipt]->Fill(mass,Prescl[3]);
			hfg_jet_combined_fill[ipt]->Fill(mass,Prescl[3]);
		}
		
		if( prob > (1.0 - 1.0/Prescl[4]) && Jet80 && maxJetTrgPt >= 80 && maxJetTrgPt < 100 ) 
		{
			hfg_jet80_jetpt100[ipt]->Fill(mass,Prescl[4]);
			hfg_jet_combined_fill[ipt]->Fill(mass,Prescl[4]);
		}
		
		if( prob > (1.0 - 1.0/Prescl[5]) && Jet100 && maxJetTrgPt >= 100 )      
		{
			hfg_jet100_jetpt200[ipt]->Fill(mass,Prescl[5]);
			hfg_jet_combined_fill[ipt]->Fill(mass,Prescl[5]);
		}

//		cout << "prob : " << prob << endl;

    }

    hfcandidate->Reset();
}

//


int trigcom_pPb::decideptbin( float dpt )
{
        int ipt = -1;
        for ( int i = 0 ; i < NPT; i++)
        {
            if (dpt >= ptbins[i] && dpt < ptbins[i+1])  { ipt = i; break; }
        }
        if ( dpt > ptbins[NPT] ) ipt = NPT-1;
        return ipt;
}


int trigcom_pPb::Find_d0()
{
	for( unsigned int n1 = 0; n1 < gIndex1.size(); n1++ )
	{
		for( unsigned int n2 = 0; n2 < gIndex2.size(); n2++ )
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
			//is D0 and just two daughters
			if( ! (abs (pdg[allmotherindex]) == 421 && nDaughters[allmotherindex] == 2) )  continue;

//			cout << "candidate mathed to gen" << endl;

			return 1;

		}
	}

	return 0;

}


//
void trigcom_pPb::write()
{//.. results 

    TH1::SetDefaultSumw2(0);

	for ( int ih = 0; ih < NPT; ih++ )
	{
		hfg_jet_combined[ih]->Add(hfg_jet20_jetpt40[ih]);
		hfg_jet_combined[ih]->Add(hfg_jet40_jetpt60[ih]);
		hfg_jet_combined[ih]->Add(hfg_jet60_jetpt80[ih]);
		hfg_jet_combined[ih]->Add(hfg_jet80_jetpt100[ih]);
		hfg_jet_combined[ih]->Add(hfg_jet100_jetpt200[ih]);
		hfg_jet_mb_combined[ih]->Add(hfg_jet_combined[ih]);
		hfg_jet_mb_combined[ih]->Add(hfg_mb_jetpt20[ih]);

	}
    
	TH1F * N_mb_raw = new TH1F("N_mb_raw","N_mb_raw",NPT,ptbins);
	TH1F * N_jet_combined = new TH1F("N_jet_combined","N_jet_combined",NPT,ptbins);
    TH1F * N_mb_jet_combined = new TH1F("N_mb_jet_combined","N_mb_jet_combined",NPT,ptbins);
    TH1F * N_mb = new TH1F("N_mb","N_mb",NPT,ptbins);
	TH1F * N_jet20 = new TH1F("N_jet20","N_jet20",NPT,ptbins);
    TH1F * N_jet40 = new TH1F("N_jet40","N_jet40",NPT,ptbins);
    TH1F * N_jet60 = new TH1F("N_jet60","N_jet60",NPT,ptbins);
    TH1F * N_jet80 = new TH1F("N_jet80","N_jet80",NPT,ptbins);
    TH1F * N_jet100 = new TH1F("N_jet100","N_jet100",NPT,ptbins);

    for ( int ih = 0; ih < NPT; ih++ )
	{
		double error;
		N_mb_raw->SetBinContent(ih+1,hfg_zerobias_raw[ih]->IntegralAndError(0,-1,error));
		N_mb_raw->SetBinError(ih+1,error);
	
	    N_jet_combined->SetBinContent(ih+1,hfg_jet_combined[ih]->IntegralAndError(0,-1,error));
		N_jet_combined->SetBinError(ih+1,error);

		N_mb_jet_combined->SetBinContent(ih+1,hfg_jet_mb_combined[ih]->IntegralAndError(0,-1,error));
		N_mb_jet_combined->SetBinError(ih+1,error);

		N_mb->SetBinContent(ih+1,hfg_zerobias[ih]->IntegralAndError(0,-1,error));
		N_mb->SetBinError(ih+1,error);

		N_jet20->SetBinContent(ih+1,hfg_jet20[ih]->IntegralAndError(0,-1,error));
		N_jet20->SetBinError(ih+1,error);

		N_jet40->SetBinContent(ih+1,hfg_jet40[ih]->IntegralAndError(0,-1,error));
		N_jet40->SetBinError(ih+1,error);

		N_jet60->SetBinContent(ih+1,hfg_jet60[ih]->IntegralAndError(0,-1,error));
		N_jet60->SetBinError(ih+1,error);

		N_jet80->SetBinContent(ih+1,hfg_jet80[ih]->IntegralAndError(0,-1,error));
		N_jet80->SetBinError(ih+1,error);

		N_jet100->SetBinContent(ih+1,hfg_jet100[ih]->IntegralAndError(0,-1,error));
		N_jet100->SetBinError(ih+1,error);

	}


    result->cd();
	
    N_mb_raw->Write();
	N_jet_combined->Write();
	N_mb_jet_combined->Write();
	N_mb->Write();
	N_jet20->Write();
	N_jet40->Write();
	N_jet60->Write();
	N_jet80->Write();
	N_jet100->Write();


	for ( int ih = 0; ih < NPT; ih++ )
	{

        hfg_all[ih]->Write();
        hfg_jettrigfired[ih]->Write();

        hfg_jet_combined[ih]->Write();
		hfg_jet_combined_fill[ih]->Write();
        hfg_jet_mb_combined[ih]->Write();

        hfg_zerobias_raw[ih]->Write();
        hfg_jet20_raw[ih]->Write();
        hfg_jet40_raw[ih]->Write();
        hfg_jet60_raw[ih]->Write();
        hfg_jet80_raw[ih]->Write();
        hfg_jet100_raw[ih]->Write();

        hfg_zerobias[ih]->Write();
        hfg_jet20[ih]->Write(); 
        hfg_jet40[ih]->Write();
        hfg_jet60[ih]->Write();
        hfg_jet80[ih]->Write();
        hfg_jet100[ih]->Write();

        hfg_mb_jetpt20_raw[ih]->Write();
        hfg_jet20_jetpt40_raw[ih]->Write();
        hfg_jet40_jetpt60_raw[ih]->Write();
        hfg_jet60_jetpt80_raw[ih]->Write();
        hfg_jet80_jetpt100_raw[ih]->Write();
        hfg_jet100_jetpt200_raw[ih]->Write();


        hfg_mb_jetpt20[ih]->Write();
        hfg_jet20_jetpt40[ih]->Write();
        hfg_jet40_jetpt60[ih]->Write();
        hfg_jet60_jetpt80[ih]->Write();
        hfg_jet80_jetpt100[ih]->Write();
        hfg_jet100_jetpt200[ih]->Write();

	}
    result->Close();
}


void trigcom_pPb::define_cuts()
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


//
//

