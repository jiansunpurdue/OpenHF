void draw_trigcomb()
{
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);
//	TFile * input1 = new TFile("trigcom_pPb_ForestD02Kpi_pthat10_Merged.root");
//    TFile * input1 = new TFile("trigcom_pPb_ForestD02Kpi_pthat30_Merged.root");
    TFile * input1 = new TFile("trigcom_pPb_ForestD02Kpi_pthat80_unMerged.root");
//    TFile * input1 = new TFile("trigcom_pPb_ForestD02Kpi_combine.root");

	TH1F * N_mb_raw = (TH1F *) input1->Get("N_mb_raw");
	TH1F * N_jet_combined = (TH1F *) input1->Get("N_jet_combined");
	TH1F * N_mb_jet_combined = (TH1F *) input1->Get("N_mb_jet_combined");
	TH1F * N_mb = (TH1F *) input1->Get("N_mb");
	TH1F * N_jet20 = (TH1F *) input1->Get("N_jet20");
	TH1F * N_jet40 = (TH1F *) input1->Get("N_jet40");
	TH1F * N_jet60 = (TH1F *) input1->Get("N_jet60");
	TH1F * N_jet80 = (TH1F *) input1->Get("N_jet80");
	TH1F * N_jet100 = (TH1F *) input1->Get("N_jet100");

	TCanvas * cfg = new TCanvas("cfg","cfg",800,800);
	cfg->Divide(2,1);

	cfg->cd(1);
	gPad->SetLogy();

	N_mb_raw->SetLineWidth(2);
	N_mb_jet_combined->SetLineWidth(2);
	N_jet_combined->SetLineWidth(2);
	N_jet20->SetLineWidth(2);
	N_jet40->SetLineWidth(2);
	N_jet60->SetLineWidth(2);
	N_jet80->SetLineWidth(2);
	N_jet100->SetLineWidth(2);

	N_mb_raw->SetLineColor(1);
	N_mb_jet_combined->SetLineColor(2);
	N_jet_combined->SetLineColor(4);
	N_jet20->SetLineColor(3);
	N_jet40->SetLineColor(6);
	N_jet60->SetLineColor(7);
	N_jet80->SetLineColor(9);
	N_jet100->SetLineColor(50);
	N_mb_raw->SetMinimum(1);
//	N_mb_raw->SetMarkerColor(2);
//	N_mb_raw->SetLineWidth(0.5);
//	N_mb_raw->SetMarkerSize(0.8);
//	N_mb_raw->SetMarkerStyle(20);
	N_mb_raw->DrawCopy("ep");
	N_mb_jet_combined->DrawCopy("epsame");
	N_jet_combined->DrawCopy("epsame");
	N_jet20->DrawCopy("epsame");
	N_jet40->DrawCopy("epsame");
	N_jet60->DrawCopy("epsame");
	N_jet80->DrawCopy("epsame");
	N_jet100->DrawCopy("epsame");

	TLegend * t = new TLegend(0.5,0.5,0.85,0.85);
	t->SetFillColor(0);
	t->SetTextSize(0.05);
	t->AddEntry(N_mb_raw,"Minbias");
	t->AddEntry(N_mb_jet_combined,"Jet&Mb Combined");
	t->AddEntry(N_jet_combined,"Jet Combined");
	t->AddEntry(N_jet20,"Jet20");
	t->AddEntry(N_jet40,"Jet40");
	t->AddEntry(N_jet60,"Jet60");
	t->AddEntry(N_jet80,"Jet80");
	t->AddEntry(N_jet100,"Jet100");

	t->Draw();

	cfg->cd(2);
	N_mb_raw->Divide(N_mb_jet_combined);
	N_jet20->Divide(N_mb_jet_combined);
	N_jet40->Divide(N_mb_jet_combined);
	N_jet60->Divide(N_mb_jet_combined);
	N_jet80->Divide(N_mb_jet_combined);
	N_jet100->Divide(N_mb_jet_combined);

	N_mb_raw->GetYaxis()->SetRangeUser(0,2.0);

    N_mb_raw->DrawCopy("ep");
    N_jet20->DrawCopy("epsame");
    N_jet40->DrawCopy("epsame");
    N_jet60->DrawCopy("epsame");
    N_jet80->DrawCopy("epsame");
    N_jet100->DrawCopy("epsame");
	

}
