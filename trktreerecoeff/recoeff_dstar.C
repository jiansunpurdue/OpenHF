#define NPT 8
double ptbins[NPT+1] = {0,5,10,15,20,30,40,60,140};

void recoeff_dstar()
{
  TH1::SetDefaultSumw2();
  TFile * gen = new TFile("gensignalget_d0dstar_genana_bugfixed_0732.root");
  TTree * gendstartree = (TTree *) gen->Get("dstartree");
  TFile * reco = new TFile("trktreerecoeff_d0dstar_genana_bugfixed_hiregittrack_0732_regit.root");
  TTree * recodstartree = (TTree *) reco->Get("dstartree");

  TH1F * dstargenpt = new TH1F("dstargenpt","dstargenpt",NPT,ptbins);
  TH1F * dstargenpt_reco = new TH1F("dstargenpt_reco","dstargenpt_reco",NPT,ptbins);
  TH1F * dstargenpt_reco_qua = new TH1F("dstargenpt_reco_qua","dstargenpt_reco_qua",NPT,ptbins);


  gendstartree->Draw("dstarpt>>dstargenpt","dstareta > -2. && dstareta < 2.","goff");
  recodstartree->Draw("dstarpt>>dstargenpt_reco","dstareta > -2. && dstareta < 2.","goff");
  recodstartree->Draw("dstarpt>>dstargenpt_reco_qua","dstareta > -2. && dstareta < 2. && dstarallhighpurity > 0.5","goff");

  TH1F * recoeff_dstar = new TH1F("recoeff_dstar","recoeff_dstar",NPT,ptbins);
  recoeff_dstar->Divide(dstargenpt_reco,dstargenpt,1,1,"B");

  TH1F * recoeff_dstar_qua = new TH1F("recoeff_dstar_qua","recoeff_dstar_qua",NPT,ptbins);
  recoeff_dstar_qua->Divide(dstargenpt_reco_qua,dstargenpt,1,1,"B");

  TCanvas * dstarrecoeff = new TCanvas("dstarrecoeff","dstarrecoeff");

  recoeff_dstar->SetMarkerSize(0.7);
  recoeff_dstar->SetMarkerStyle(20);
  recoeff_dstar->SetTitle("");
  recoeff_dstar->GetXaxis()->SetTitle("D pt (GeV)");
  recoeff_dstar->GetYaxis()->SetTitle("(N with all dau reco)/N_{Gen}");
  recoeff_dstar->GetYaxis()->SetRangeUser(0,1.0);
  recoeff_dstar->SetStats(kFALSE);
  recoeff_dstar->Draw("ep");
  recoeff_dstar_qua->SetLineColor(2);
  recoeff_dstar_qua->SetMarkerSize(0.7);
  recoeff_dstar_qua->SetMarkerStyle(20);
  recoeff_dstar_qua->SetMarkerColor(2);
  recoeff_dstar_qua->Draw("epsame");


  TLegend * t = new TLegend(0.3,0.1,0.85,0.4);
  t->AddEntry(recoeff_dstar, "All Dau Reco");
  t->AddEntry(recoeff_dstar_qua, "All Dau Highpurity Reco Track");
  t->SetFillColor(0);
  t->Draw();


  TFile * output = new TFile("recoeff_dstar.root","RECREATE");
  dstargenpt->Write();
  dstargenpt_reco->Write();
  dstargenpt_reco_qua->Write();
  recoeff_dstar->Write();
  recoeff_dstar_qua->Write();
  dstarrecoeff->Write();

  output->Close();

}
