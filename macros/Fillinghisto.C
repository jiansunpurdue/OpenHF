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

#include "./../data_format/hfcand_v0.hh"


using namespace std;

#define NPHI 2
//#define NPT 9
#define NPT 8
//#define NPT 4
#define MASSBIN 300
//#define MASSBIN 200
#define PI 3.1416
//#define REBIN 5
#define REBIN 10
#define kMaxEvtPlanes 1000

//double ptbins[NPT+1] = {0,5,10,15,20,25,30,40,60,200};
//double ptbins[NPT+1] = {0,5,10,15,25,40,200};
//double ptbins[NPT+1] = {0,5,10,20,40,200};
double ptbins[NPT+1] = {0,4,5,8,12,16,25,40,200};
//double ptbins[NPT+1] = {0,1,2,3,4,5,10,16,30,200};
//double ptbins[NPT+1] = {0,5,8,12,16,25,40,200};


//int hibins[15] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 20, 24, 28, 32, 36, 40}; // >= && <
float hibins[15] = {-0.5, 1.5, 3.5, 5.5, 7.5, 9.5, 11.5, 13.5, 15.5, 19.5, 23.5, 27.5, 31.5, 35.5, 39.5};

double epreso_p[14] = {0.533455, 0.723272, 0.798937, 0.832178, 0.842435, 0.837015, 0.821521, 0.798025, 0.737325, 0.609293, 0.432927, 0.251981, 0.100606, 0.055760};
double epreso_p_error[14] = {0.002401, 0.002093, 0.002021, 0.002045, 0.002047, 0.002020, 0.002034, 0.002033, 0.001448, 0.001542, 0.001939, 0.003116, 0.005831, 0.012724};

double epreso_m[14] = {0.539450, 0.722841, 0.801483, 0.831597, 0.839228, 0.835527, 0.821654, 0.796565, 0.736792, 0.609372, 0.432557, 0.244120, 0.101207, 0.046039};
double epreso_m_error[14] = {0.002428, 0.002092, 0.002027, 0.002044, 0.002039, 0.002016, 0.002035, 0.002029, 0.001446, 0.001542, 0.001937, 0.003019, 0.005866, 0.010545};


int cut_hibin_low = 0;        // centrality = hibin * 2.5%    hibin number is from 0 to 39
int cut_hibin_high = 40;      // hibin is from 0. 0 is 0% to 2.5%     0 to 3 10%     4-11 10-30%    4-19  10-50%

//int cut_hibin_low = 4;
//int cut_hibin_high = 23;

float cut_m_low[3];
float cut_m_high[3];
float cut_m_dau_low[3];
float cut_m_dau_high[3];
float cut_ffls3d[3];
float cut_falpha0[3];
float cut_fprob[3];
float cut_fdr[3];
float cut_fchi2[3];

float massbinstep = (2.05-1.70)*REBIN/MASSBIN;

float cut_ffls3d_fillhisto = 2.0;

TH1F* hfg_all[NPT];
TH1F* hfgdiff_all[NPT];
TH1F* hbgdiff_all[NPT];
TH1F* hfg_tot[NPT];
TH1F* hfgdiff_tot[NPT];
TH1F* hbgdiff_tot[NPT];
TH1F* hfg[NPT][NPHI];
TH1F* cos_2deltaphi[NPT][MASSBIN/REBIN];

void book_hist( int decaytype)
{

    char hname[100], pt_range[1000], pt_range_phi_range[2000], pt_range_mass_range[2000];
        for(int i = 0; i<NPT; i++) {

            float pt_low = ptbins[i];
            float pt_high = ptbins[i+1];

            sprintf(pt_range, "pt: %f %f", pt_low, pt_high);

			cout << "low: " << cut_m_low[decaytype] << "   high: " << cut_m_high[decaytype] << endl;

            sprintf(hname, "hfg_tot_%d", i);
            hfg_tot[i] = new TH1F(hname, pt_range, MASSBIN, cut_m_low[decaytype], cut_m_high[decaytype]);
			sprintf(hname, "hfg_all_%d", i);
            hfg_all[i] = new TH1F(hname, pt_range, MASSBIN, cut_m_low[decaytype], cut_m_high[decaytype]);

			sprintf(hname, "hfgdiff_all_%d", i);
            hfgdiff_all[i] = new TH1F(hname, pt_range, MASSBIN, 0.135, 0.154);
			sprintf(hname, "hfgdiff_tot_%d", i);
			hfgdiff_tot[i] = new TH1F(hname, pt_range, MASSBIN, 0.135, 0.154);
			
			sprintf(hname, "hbgdiff_all_%d", i);
			hbgdiff_all[i] = new TH1F(hname, pt_range, MASSBIN, 0.135, 0.154);
			sprintf(hname, "hbgdiff_tot_%d", i);
			hbgdiff_tot[i] = new TH1F(hname, pt_range, MASSBIN, 0.135, 0.154);
            //.. track error ...
            hfg_tot[i]->Sumw2();
			hfg_all[i]->Sumw2();
            hfgdiff_all[i]->Sumw2();
			hfgdiff_tot[i]->Sumw2();
			hbgdiff_all[i]->Sumw2();
			hbgdiff_tot[i]->Sumw2();

            for(int j = 0; j<NPHI; j++) {
				 
                 if( j == 0 )
                 sprintf(pt_range_phi_range, " pt: %f %f, In_plane", pt_low, pt_high);
				 else if ( j == 1 ) sprintf(pt_range_phi_range, "pt: %f %f, Out_of_plane", pt_low, pt_high);

                 sprintf(hname, "hfg_%d_%d", i, j);
                 hfg[i][j] = new TH1F(hname, pt_range_phi_range, MASSBIN, cut_m_low[decaytype], cut_m_high[decaytype]);

                 //.. track error ...
                 hfg[i][j]->Sumw2();

            }
           
           for(int j = 0; j<MASSBIN/REBIN; j++) {
                 
                 float mass_low = 1.70+massbinstep*j;
                 float mass_high = 1.70+massbinstep*(j+1);

                 sprintf(pt_range_mass_range, " pt: %f %f, mass: %f %f", pt_low, pt_high, mass_low, mass_high);

                 sprintf(hname, "cos_2deltaphi_%d_%d", i, j);
                 cos_2deltaphi[i][j] = new TH1F(hname, pt_range_mass_range, MASSBIN/REBIN, -1, 1);
				 cos_2deltaphi[i][j]->StatOverflows(kTRUE);

                 //.. track error ...
                 cos_2deltaphi[i][j]->Sumw2();

            }

    			
        }

}

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

int decidephibin( float phi_diff)
{
	  int iphi = -1;
      if(phi_diff < -1 * PI/4)  phi_diff = phi_diff + 2 * PI;
      if( (phi_diff >= -1 * PI/4 && phi_diff < PI/4) || (phi_diff >= 3 * PI/4 && phi_diff < 5 * PI/4) )  iphi = 0;
      else if( (phi_diff >= PI/4 && phi_diff < 3 * PI/4) || (phi_diff >= 5 * PI/4 && phi_diff < 7 * PI/4) )  iphi = 1;
      
      return iphi;

}

int decidemassbin( float mass)
{
	int imass = -1;
	imass = (mass - 1.70)/massbinstep;
	return imass;

}

double decideepreso( int hibin, float feta)
{
	double eventplanereso = -999.;
	for(int i = 0; i < 14; i++)
	{
		if ( hibin > hibins[i] && hibin < hibins[i+1] )  
		 {  if ( feta < 0 ) 
			    eventplanereso = epreso_p[i] ; 
			else   
				eventplanereso = epreso_m[i] ;
			break; 
		 }
	}

	return eventplanereso;
}


void write_histo(TFile * output)
{
	output->cd();
	for (int i = 0; i < NPT; i++)
	{
		hfg_tot[i]->Write();
		hfg_all[i]->Write();
		hfgdiff_all[i]->Write();
		hfgdiff_tot[i]->Write();
		hbgdiff_all[i]->Write();
		hbgdiff_tot[i]->Write();
		for ( int j = 0; j < NPHI; j++)
			hfg[i][j]->Write();
		for ( int j = 0; j < MASSBIN/REBIN; j++)
			cos_2deltaphi[i][j]->Write();
	}
    return;
}

void define_cuts()
{

    cut_m_low[0] = -1.0;
    cut_m_high[0] = -1.0;
    cut_m_dau_low[0] = -1.0;
    cut_m_dau_high[0] = -1.0;
    cut_ffls3d[0] = -1.0;
    cut_falpha0[0] = -1.0;
    cut_fprob[0] = -1.0;
    cut_fdr[0] = -1.0;
    cut_fchi2[0] = -1.0;

    //D*->D0+pi
    cut_m_low[1] = 1.92;
    cut_m_high[1] = 2.10;
    cut_m_dau_low[1] = 1.8;
    cut_m_dau_high[1] = 1.92;
    cut_ffls3d[1] = 3.0;
    cut_falpha0[1] = 0.2;
    cut_fprob[1] = 0.05;
    cut_fdr[1] = 0.25;
    cut_fchi2[1] = 3;

    //D0->K+pi
    cut_m_low[2] = 1.70;
    cut_m_high[2] = 2.05;
    cut_m_dau_low[2] = -1;
    cut_m_dau_high[2] = 1;
    cut_ffls3d[2] = 2.0;
    cut_falpha0[2] = 0.2;
    cut_fprob[2] = 0.05;
    cut_fdr[2] = 0.25;
    cut_fchi2[2] = 3;

}

void Fillinghisto(int startFile, int endFile, char *filelist, int decaychannel)
{
  
//    decaytype = decaychannel;

//	cout << "decaytype : " << decaytype << endl;

	define_cuts();
    
	int run;
    int evt;
    int hibin;
    float hiEvtPlane_21;
	float hiEvtPlane_22;
	float hiEvtPlane_23;
    int type;
    float mass;
    float fpt;
	float fpt1;
	float fpt2;
    float fphi;
	float fphi1;
	float fphi2;
	float feta;
	float feta1;
	float feta2;
    float ffls3d;
    float alpha;
	float fq1;
	float fq2;

    float fprob;
    float fdr;
    float fchi2;
    float mass_dau;

	int nEvtPlanes;
	float hiEvtPlane[kMaxEvtPlanes];

    TTree * HiTree;
	TTree * hftree;

	book_hist(decaychannel);

	hfcand_v0* hfcandidate = new hfcand_v0;

	char* filename = new char[1000];
	char* mesonName = new char[100];

    if(decaychannel==1)
        mesonName = "Dstar2D0Pi";
    else if(decaychannel==2)
        mesonName = "D02KPi";



    sprintf(filename, "ana_%s_%s_%dTo%d.root", mesonName, filelist, startFile, endFile);
    TFile * output =  new TFile(filename, "recreate");

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
		HiTree  = (TTree*)f->Get("hiEvtAnalyzer/HiTree");
//		hftree = (TTree*)f->Get("tree/hftree");
        hftree = (TTree*)f->Get("OpenHF/hftree");
//	    hftree = (TTree*)f->Get("RegitOpenHF/hftree");
        if(!hftree || !HiTree) {
            cout<<"==> empty tree <=="<<endl;
            continue;
        }


        HiTree->AddFriend(hftree);

        HiTree->SetBranchAddress("run", &run);
        HiTree->SetBranchAddress("evt", &evt);
        HiTree->SetBranchAddress("hiNevtPlane", &nEvtPlanes);
        HiTree->SetBranchAddress("hiEvtPlanes", hiEvtPlane);
        hftree->SetBranchAddress("hfcandidate", &hfcandidate);
        HiTree->SetBranchAddress("hiBin",&hibin);

		for(int i = 0; i<HiTree->GetEntries(); i++) {
           for (int j = 0; j < 50 ; j++)   //reset event planes
            {
                hiEvtPlane[j] = -999;
            }

			HiTree->GetEntry(i);
			for(int k = 0; k<hfcandidate->get_nhfcand(); k++)
			{
				snglhfcand* cand = hfcandidate->get_hfcand(k);
				if(cand->get_type() != decaychannel) continue;
                hiEvtPlane_21 = hiEvtPlane[21];
                hiEvtPlane_22 = hiEvtPlane[22];
                hiEvtPlane_23 = hiEvtPlane[23];
                type = cand->get_type();
                mass = cand->get_fm();
                fpt = cand->get_fpt();
                fphi = cand->get_fphi();
                fphi1 = cand->get_fphi1();
                fphi2 = cand->get_fphi2();
                feta = cand->get_feta();
                feta1 = cand->get_feta1();
                feta2 = cand->get_feta2();
                ffls3d = cand->get_ffls3d();
                alpha = cand->get_falpha0();
                fprob = cand->get_fprob();
                fdr = cand->get_fdr();
                fchi2 = cand->get_fchi2();
                mass_dau = cand->get_fmdau1();
                fpt1 = cand->get_fpt1();
                fpt2 = cand->get_fpt2();
                fq1 = cand->get_fq1();
                fq2 = cand->get_fq2();
//				cout << "ffls3d: " << ffls3d << endl;
       //       cout << "fq1 = " << cand->get_fq1() << "fq2 = " << cand->get_fpt2() << endl;
                if(!(mass > cut_m_low[decaychannel] && mass < cut_m_high[decaychannel] && ffls3d > cut_ffls3d[decaychannel] && alpha < cut_falpha0[decaychannel] && fprob > cut_fprob[decaychannel] && fdr < cut_fdr[decaychannel] && fchi2 < cut_fchi2[decaychannel])) continue;

                if( fq1 + fq2 != 0 ) continue;

        		if( TMath::Abs( feta ) > 2.0 ) continue;
				
				int ipt = decideptbin(fpt);

				if ( decaychannel == 2)
				{
				   hfg_all[ipt]->Fill(mass);
				}
				else
				{
					if(mass_dau > cut_m_dau_low[decaychannel] && mass_dau < cut_m_dau_high[decaychannel])
					{
						hfg_all[ipt]->Fill(mass);
						if( mass-mass_dau > 0.135 && mass-mass_dau < 0.154)
						hfgdiff_all[ipt]->Fill(mass-mass_dau);
					}
					else
						if( mass-mass_dau > 0.135 && mass-mass_dau < 0.154)
					    	hbgdiff_all[ipt]->Fill(mass-mass_dau);
				}


//				cout << "passing cuts" << endl;


                if( hiEvtPlane_22 < -4.0 || hiEvtPlane_23 < -4.0 )  continue;
                if( hibin  > cut_hibin_high || hibin < cut_hibin_low )   continue;
        
        
//        		int ipt = decideptbin(fpt);
        		float diff;
        
        
        		if ( feta < 0 ) diff = fphi - hiEvtPlane_22;
        		else diff = fphi - hiEvtPlane_23;
        
        		int iphi = decidephibin(diff);
        		int imass = decidemassbin(mass);
        		double evtplanereso = decideepreso( hibin, feta);        
         
				if ( decaychannel == 2)
				{
				   hfg_tot[ipt]->Fill(mass);
				}
				else
				{
					if(mass_dau > cut_m_dau_low[decaychannel] && mass_dau < cut_m_dau_high[decaychannel])
					{
						hfg_tot[ipt]->Fill(mass);
						if( mass-mass_dau > 0.135 && mass-mass_dau < 0.154)
						hfgdiff_tot[ipt]->Fill(mass-mass_dau);
					}
					else
						if( mass-mass_dau > 0.135 && mass-mass_dau < 0.154)
						   hbgdiff_tot[ipt]->Fill(mass-mass_dau);
				}

                hfg[ipt][iphi]->Fill(mass);
        
        		float cos2deltaphi_pm = TMath::Cos(2*diff);
        
        		float corrected_cos2deltaphi = cos2deltaphi_pm/evtplanereso;
        
        		cos_2deltaphi[ipt][imass]->Fill(corrected_cos2deltaphi);
			}
            hfcandidate->Reset();
		}

		f->Close();
		delete f;

     }

	write_histo(output);
	output->Close();
    
}
