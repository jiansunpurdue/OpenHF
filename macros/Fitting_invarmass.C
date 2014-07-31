#include "Fillinghisto.C"
#include "TCanvas.h"
#include "TGraphPainter.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#define RANDOM_SIZE 10000



float mass[MASSBIN/REBIN];
float mass_error[MASSBIN/REBIN];

float signal_bkg[NPT][MASSBIN/REBIN];
float signal_bkg_error[NPT][MASSBIN/REBIN];
float mass_start[NPT][MASSBIN/REBIN];

//for random signal_bkg

float signal_bkg_random[NPT][MASSBIN/REBIN];
TH1F * v2_random_oneptbin[NPT];


TH1F * v2_pt[NPT];
TH1F * v2_preliminary = new TH1F("v2_preliminary","v2_preliminary",NPT,ptbins);
TH1F * v2_pre_diff_sum_ratio = new TH1F("v2_pre_diff_sum_ratio","v2_pre_diff_sum_ratio",NPT,ptbins);

//float rlow = 1.77875;
//float rhigh = 1.95375;
float rlow = 1.70;
float rhigh = 2.05;
float rlow_2deltaphi = 1.70;
float rhigh_2deltaphi = 2.05;
//float rlow_2deltaphi = 1.77875;
//float rhigh_2deltaphi = 1.95375;


TH1F * diff[NPT];
TH1F * sum[NPT];
TH1F * ratio[NPT];
TH1F * v2_diff_sum_ratio[NPT];
TH1F * diff_pt = new TH1F("diff_pt","diff_pt",NPT,ptbins);
TH1F * sum_pt = new TH1F("sum_pt","sum_pt",NPT,ptbins);
TH1F * tot_pt = new TH1F("tot_pt","tot_pt",NPT,ptbins);
TH1F * ratio_pt = new TH1F("ratio_pt","ratio_pt",NPT,ptbins);


void get_histo( TFile * f)
{
    char hname[100];
        for(int i = 0; i<NPT; i++) {

            sprintf(hname, "hfg_tot_%d", i);
            hfg_tot[i] = (TH1F *) f->Get(hname);

            for(int j = 0; j<NPHI; j++) {

                 sprintf(hname, "hfg_%d_%d", i, j);
                 hfg[i][j] = (TH1F *) f->Get(hname);

            }

            for(int j = 0; j<MASSBIN/REBIN; j++) {

                 sprintf(hname, "cos_2deltaphi_%d_%d", i, j);
                 cos_2deltaphi[i][j] = (TH1F *) f->Get(hname);

		   }
		}

		return;

}

void Rebin_hist()
{
	for ( int i = 0; i < NPT; i++)
	{
		hfg_tot[i]->Rebin(REBIN);
		hfg[i][0]->Rebin(REBIN);
		hfg[i][1]->Rebin(REBIN);
	}
}

void Fitting_invarmass()
{   
	gStyle->SetTitleSize(0.08,"t");
	gStyle->SetTitleSize(0.08,"x");
	gStyle->SetTitleSize(0.08,"y");
//	TFile * input = new TFile ("output_hibin_4_23_ptins_8_Rebin_10_d0_2_HFmp.root");
//    TFile * input = new TFile ("output_hibin_4_23_ptins_8_Rebin_10_d0_2_HF.root");
    TFile * input = new TFile ("D02KPi_filelist_total_rereco_cent1060_d4p0.root");
    get_histo( input );


    Rebin_hist();

	TCanvas * mass_pt = new TCanvas("mass_pt","mass_pt", 1000, 1000);
	mass_pt->Divide(3,3);

    for ( int i = 0; i < NPT; i++)
	{	bool getratio = true;
	    fitting_histo(hfg_tot[i], mass_pt, i, getratio, tot_pt);
	}


	mass_pt->SaveAs("mass_ptbins_fitting_D0.png");

	char outputfile[200];
	sprintf(outputfile,"v2_mass_%f_%f_rebin_%d_hibin_%d_%d_ptbins_%d_d0_%d.root",rlow,rhigh,REBIN,cut_hibin_low,cut_hibin_high,NPT,cut_ffls3d_fillhisto);

	TFile * output = new TFile(outputfile,"RECREATE");

	mass_pt->Write();

//    get_v2_mass_pt(output);

//	plot_signal_bkg(output);

//    random_signal_bkg ( output );

	tot_pt->Write();
	diff_pt->Write();
	sum_pt->Write();


	output->Close();

    return;

}

void get_v2_mass_pt(TFile * output)
{

	for( int i = 0; i < NPT; i++)
	{
		char hname[200],pt_range[200];
		sprintf(hname,"v2_pt_%d",i);
		sprintf(pt_range,"pt: %f %f", ptbins[i],ptbins[i+1]);

		v2_pt[i] = new TH1F(hname,pt_range,MASSBIN/REBIN,cut_m_low,cut_m_high);

		for ( int j = 0; j < MASSBIN/REBIN; j++)
		{
			v2_pt[i]->SetBinContent(j+1,cos_2deltaphi[i][j]->GetMean(1));
			v2_pt[i]->SetBinError(j+1,cos_2deltaphi[i][j]->GetMeanError(1));
//		    cout << "v2 =====" << v2[i][j] << endl;
		}
	}

	for ( int j = 0; j < MASSBIN/REBIN; j++)
	{
		mass[j] = 1.70 + (j+0.5) * massbinstep;
//        mass[j] = 1.70 + j* massbinstep;
		mass_error[j] = massbinstep/2;
    }

	TCanvas * v2_mass_pt_hist = new TCanvas("v2_hist","v2_hist", 1000, 1000);

	v2_mass_pt_hist->Divide(3,3);

	for ( int i = 0; i < NPT; i++)
	{
	    v2_mass_pt_hist->cd(i+1);
        v2_pt[i]->GetXaxis()->SetRangeUser(rlow,rhigh);
//	    TF1 * liner = new TF1("liner","pol1(0)",rlow,1.95);
	    TF1 * fit = new TF1("fit",fitfunction,rlow,rhigh,4);
		fit->SetLineColor(2);
		fit->SetLineWidth(1);
		fit->FixParameter(0, i);
		v2_pt[i]->SetMarkerColor(1);
		v2_pt[i]->SetMarkerStyle(20);
//		v2_pt[i]->SetStats(0);
		v2_pt[i]->SetMarkerSize(0.7);
//        TF1 * liner = new TF1("liner","[0] + [1] * x",rlow,1.95);
		v2_pt[i]->Fit(fit,"","",rlow,rhigh);
		v2_pt[i]->Draw();
		v2_preliminary->SetBinContent(i+1,fit->GetParameter(1));
		v2_preliminary->SetBinError(i+1,fit->GetParError(1));
	}

    v2_mass_pt_hist->SaveAs("v2_pt_histo.png");


	TCanvas * v2_pre =  new TCanvas("v2_pre","v2_pre");

	v2_pre->cd();


    v2_preliminary->GetXaxis()->SetRangeUser(0,35);
	v2_preliminary->GetYaxis()->SetRangeUser(-0.4,0.4);
	v2_preliminary->SetMarkerColor(1);
	v2_preliminary->SetMarkerStyle(20);
	v2_preliminary->SetMarkerSize(1.0);
	v2_preliminary->SetStats(kFALSE);
	v2_preliminary->Draw("EP");

	v2_pre->SaveAs("v2_pre_invarmass.png");

	output->cd();
    v2_mass_pt_hist->Write();
	v2_pre->Write();
    v2_preliminary->Write();

	return;

}

Double_t fitfunction( Double_t * x, Double_t *par)
{
	return signalweight(x,par)*par[1] + (1 - signalweight(x,par)) * bkg_liner(x,&par[2]);
}

Double_t signalweight( Double_t * x, Double_t *par)
{
    Double_t xx = x[0];
	int mass_bin = (x[0]-1.70)/massbinstep;
	//cout << "par_0 : " << par[0] << endl;
	return signal_bkg[par[0]][mass_bin];
}

Double_t bkg_liner( Double_t * x, Double_t *par)
{
    Double_t xx = x[0];
    Double_t arg = par[0] + par[1] * xx;
	return arg;

}


void plot_signal_bkg ( TFile * output )
{
   TCanvas * plot_signal_bkg = new TCanvas("plot_signal_bkg","plot_signal_bkg", 1000, 1000);
   plot_signal_bkg->Divide(3,3);

   TGraphErrors* sig_bkg[NPT];

   for ( int i = 0; i < NPT; i++)
   {
      plot_signal_bkg->cd(i+1);
	  sig_bkg[i] = new TGraphErrors(MASSBIN/REBIN, mass, signal_bkg[i], mass_error, signal_bkg_error[i]);
      char graphname[200];
	  sprintf(graphname, "#alpha: pt %f %f",  ptbins[i], ptbins[i+1]);
	  sig_bkg[i]->SetTitle(graphname);
      sig_bkg[i]->SetFillColor(kRed);
//	  cout << "before******" << endl;
	  sig_bkg[i]->GetXaxis()->SetTitle("m_{#piK} (GeV)");
//	  cout << "after******" << endl;
//	  sig_bkg[i]->GetXaxis()->CenterTitle();
      sig_bkg[i]->SetFillStyle(3010);
      sig_bkg[i]->Draw("a2");
	  sig_bkg[i]->Draw("p");
   }

   output->cd();
   plot_signal_bkg->Write();

   return;

}


void random_signal_bkg ( TFile * output )
{

   for ( int i = 0; i < NPT; i++ )
   {
	   char hname[200];
	   sprintf(hname, "v2_signal_bkg_random_pt_%f_%f", ptbins[i], ptbins[i+1]);
	   v2_random_oneptbin[i] = new TH1F(hname, hname, 1000, -0.1, 0.3);
   }

   TCanvas * v2_mass_pt_hist_random[RANDOM_SIZE];
   for ( int size = 0; size < RANDOM_SIZE; size++ )
   {
	   char canvasname[200];
	   sprintf(canvasname,"fitting_v2_signal_bkg_random_sample_%i",size);
	   v2_mass_pt_hist_random[size] = new TCanvas(canvasname, canvasname , 1000, 1000);
	   v2_mass_pt_hist_random[size]->Divide(3,3);
       
	   
	   for( int i = 0; i < NPT; i++ )
       {
           for( int j = 0; j < MASSBIN/REBIN; j++ )
           {
        	   signal_bkg_random[i][j] = gRandom->Gaus(signal_bkg[i][j],signal_bkg_error[i][j]);
//      	   cout << " *********** i : " << i << "************* j : " << j << " *********** signal_bkg_random : " << signal_bkg_random[i][j] << endl;
           }
       }

       for( int i = 0; i < NPT; i++ )
	   {
		   v2_mass_pt_hist_random[size]->cd(i+1);
		   v2_pt[i]->GetXaxis()->SetRangeUser(rlow,rhigh);
		   TF1 * fit_random = new TF1("fit_random",fitfunction_random,rlow,rhigh,4);
		   fit_random->SetLineColor(2);
           fit_random->SetLineWidth(1);
           fit_random->FixParameter(0, i);
           v2_pt[i]->SetMarkerColor(1);
           v2_pt[i]->SetMarkerStyle(20);
           v2_pt[i]->SetMarkerSize(0.7);
           v2_pt[i]->Fit(fit_random,"","",rlow,rhigh);
           v2_pt[i]->Draw();
           v2_random_oneptbin[i]->Fill(fit_random->GetParameter(1));
	   }

	   output->cd();
	   v2_mass_pt_hist_random[size]->Write();
	   v2_mass_pt_hist_random[size]->Close();

   }

   for ( int i = 0; i < NPT; i++)
   {
	   output->cd();
	   v2_random_oneptbin[i]->Write();
   }

   return;

}

Double_t fitfunction_random( Double_t * x, Double_t *par)
{
	return signalweight_random(x,par)*par[1] + (1 - signalweight_random(x,par)) * bkg_liner(x,&par[2]);
}

Double_t signalweight_random( Double_t * x, Double_t *par)
{
    Double_t xx = x[0];
	int mass_bin = (x[0]-1.70)/massbinstep;
	//cout << "par_0 : " << par[0] << endl;
	return signal_bkg_random[par[0]][mass_bin];
}

void fitting_histo( TH1F * histo, TCanvas *cfg, int i, bool getratio = false, TH1F * signalnum)
{

    cfg->cd(i+1);
    
	histo->SetMarkerSize(0.8);
    
    histo->SetLineColor(2);
    histo->SetMarkerColor(2);
    histo->SetMarkerStyle(20);
    histo->GetXaxis()->SetNdivisions(505);
    histo->GetXaxis()->SetTitle("m_{#piK} (GeV)");
    histo->GetYaxis()->SetTitle("Counts");

    histo->GetXaxis()->SetRangeUser(rlow, rhigh);
    //.. fit with a Gaussian and pol
    TF1* fit_fun = new TF1("fit_fun", "gausn(0) + gausn(3) + pol2(6)", rlow, rhigh);
//    TF1* fit_fun = new TF1("fit_fun", "gausn(0) + gausn(3) + expo(6)", rlow, rhigh);
    float max = histo->GetMaximum();

//.. ------------------------- For D0->kaon+pi ------------------------------
//     initial value for all histograms:
//     root>  .x draw_hist.C+ ("ana_D0_tot.root", false, "ana_d0.gif", 5, 1.78, 1.95);
//      1000 1.87 0.02 0 0 0
//     constraints for all histograms:
//      1.84 1.9 0 0.05 0 0 0 0 0 0
//     scanf("%f%f%f%f%f%f%f%f%f%f", &p1_L, &p1_H, &p2_L, &p2_H, &p3_L, &p3_H, &p4_L, &p4_H, &p5_L, &p5_H);
    float p0 = 1000, p1 = 1.87, p2 = 0.02;
    float p3 = 0, p4 = 0, p5 = 0;
    float p0_L = 0, p1_L = 1.84, p2_L = 0;
    float p0_H = 2*max, p1_H = 1.9, p2_H = 0.05;
    float p3_L = 0, p4_L = 0, p5_L = 0;
    float p3_H = 0, p4_H = 0, p5_H = 0;
//            //.. initial value
//            float p0 = max, p1 = 1.88, p2 = 0.02;
//            float p3 = max, p4 = 1.97, p5 = 0.02;
//            float p0_L = 0, p1_L = 1.80, p2_L = 0.001;
//            float p0_H = 2*max, p1_H = 2.1, p2_H = 0.5;
//            float p3_L = 0, p4_L = 1.80, p5_L = 0.001;
//            float p3_H = 2*max, p4_H = 2.1, p5_H = 0.5;
//
    int pass = 0;
    int fittingtry = 0;
    while (!pass) {

        fit_fun->SetParameter(0, p0);
        fit_fun->SetParameter(1, p1);
        fit_fun->SetParameter(2, p2);
        fit_fun->SetParameter(3, p3);
        fit_fun->SetParameter(4, p4);
        fit_fun->SetParameter(5, p5);

        //.. fit constraint ..
        fit_fun->SetParLimits(0, p0_L, p0_H);
        fit_fun->SetParLimits(1, p1_L, p1_H);
        fit_fun->SetParLimits(2, p2_L, p2_H);

        if(p3_L==0 && p3_H==0) {
            //.. asked to fix all parameters of 2nd gaussian to zero
            fit_fun->FixParameter(3, 0);
            fit_fun->FixParameter(4, 0);
            fit_fun->FixParameter(5, 0);
        } else {
            fit_fun->SetParLimits(3, p3_L, p3_H);
            fit_fun->SetParLimits(4, p4_L, p4_H);
            fit_fun->SetParLimits(5, p5_L, p5_H);
        }

        //.. fit

//		if(i == 0 || i == 1)
        histo->Fit(fit_fun,"","", rlow, rhigh);
//		else histo->Fit(fit_fun,"L","", rlow, rhigh);

        //.. draw foreground and background ..
        histo->Draw();

        TF1* fit_fun_1st = (TF1*)fit_fun->Clone("fit_fun_1st");
        fit_fun_1st->SetParameter(3, 0);
        fit_fun_1st->SetParameter(4, 0);
        fit_fun_1st->SetParameter(5, 0);
   //             fit_fun_1st->SetParameter(6, 0);
   //             fit_fun_1st->SetParameter(7, 0);
   //             fit_fun_1st->SetParameter(8, 0);
   //             fit_fun_1st->SetLineColor(6);
//                fit_fun_1st->Draw("same");


        TF1* fit_fun_2nd = (TF1*)fit_fun->Clone("fit_fun_2nd");
        fit_fun_2nd->SetParameter(0, 0);
        fit_fun_2nd->SetParameter(1, 0);
        fit_fun_2nd->SetParameter(2, 0);
		fit_fun_2nd->SetLineColor(4);
        fit_fun_2nd->Draw("same");

        TF1* fit_fun_bg = (TF1*)fit_fun->Clone("fit_fun_bg");
        fit_fun_bg->SetParameter(0, 0);
        fit_fun_bg->SetParameter(1, 0);
        fit_fun_bg->SetParameter(2, 0);
        fit_fun_bg->SetParameter(3, 0);
        fit_fun_bg->SetParameter(4, 0);
        fit_fun_bg->SetParameter(5, 0);

        fit_fun_bg->SetLineColor(8);
        fit_fun_bg->Draw("same");
        

        fittingtry++;


//		pass = 0;

		if (fittingtry == 2)  pass = 1;

		double binwidth = histo->GetBinWidth(3);
		double fittingarea = fit_fun->GetParameter(0);
		double areaerror = fit_fun->GetParError(0);

		signalnum->SetBinContent(i+1,fittingarea/binwidth);
		signalnum->SetBinError(i+1,areaerror/binwidth);

		if(getratio)
		{

      		for ( int j = 0; j < MASSBIN/REBIN ; j++)
      		{

      			mass_start[i][j] = 1.70+massbinstep*j;

      			float total = fit_fun->Integral(1.70+massbinstep*j,1.70+massbinstep*(j+1));
      			float total_error = fit_fun->IntegralError(1.70+massbinstep*j,1.70+massbinstep*(j+1));
      			float bkg = fit_fun_bg->Integral(1.70+massbinstep*j,1.70+massbinstep*(j+1));
//      			float bkg_error = fit_fun_bg->IntegralError(1.70+massbinstep*j,1.70+massbinstep*(j+1));
                float bkg_error = 0.0;
                cout << "bkg error: " << bkg_error << endl;
					
					
					float bincontent = histo->GetBinContent(j+1);
					float bkgnum = bkg/binwidth;
					float totalnum = total/binwidth;
//            		signal_bkg[i][j] = (bincontent-bkgnum)/bincontent;
			        signal_bkg[i][j] = (totalnum-bkgnum)/totalnum;
					signal_bkg_error[i][j] = TMath::Sqrt(signal_bkg[i][j]*(1.0-signal_bkg[i][j])/totalnum);
      	//		    signal_bkg_error[i][j] = bkg * total_error / (total * total) + bkg_error/total;
      	    //	    signal_bkg_error[i][j] = histo->GetBinError(j+1)/bincontent;
			        cout << "bincontent : " << bincontent << " total: " << total << endl;
      		    	cout << "signal / bkg :" << signal_bkg[i][j] << "signal / bkg error : " << signal_bkg_error[i][j] << endl;
      		}
   
		}
           
        float totalnum_allbin = fit_fun->Integral(1.70, 2.05)/binwidth;
		float bkgnum_allbin = fit_fun_bg->Integral(1.70, 2.05)/binwidth;

        cout << " ==========================pt bin: " << i << endl;
    
		cout << "*************** totalnum_allbin : " << totalnum_allbin << "  ******bkgnum_allbin : " << bkgnum_allbin << "*************signalnum : " << totalnum_allbin - bkgnum_allbin << endl;


		if(!pass) {
            p0 = 1000, p1 = 1.87, p2 = 0.02;
            p3 = 0, p4 = 0, p5 = 0;
            p1_L = 1.84, p2_L = 0;
            p1_H = 1.9, p2_H = 0.05;
            p3_L = 0, p4_L = 0, p5_L = 0;
            p3_H = 0, p4_H = 0, p5_H = 0;


        }
    }


	return;

}

