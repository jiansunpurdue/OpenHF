#define NPT 8
double ptbins[NPT+1] = {0,5,10,15,20,30,40,60,140};

void recoeff_d0()
{
  TH1::SetDefaultSumw2();
  TFile * gen = new TFile("gensignalget_d0dstar_genana_bugfixed_0732.root");
  TTree * gend0tree = (TTree *) gen->Get("d0tree");
  TFile * reco = new TFile("trktreerecoeff_d0dstar_genana_bugfixed_hiregittrack_0732_regit.root");
  TTree * recod0tree = (TTree *) reco->Get("d0tree");

  TH1F * d0genpt = new TH1F("d0genpt","d0genpt",NPT,ptbins);
  TH1F * d0genpt_reco = new TH1F("d0genpt_reco","d0genpt_reco",NPT,ptbins);
  TH1F * d0genpt_reco_qua = new TH1F("d0genpt_reco_qua","d0genpt_reco_qua",NPT,ptbins);


  gend0tree->Draw("d0pt>>d0genpt","d0eta > -2. && d0eta < 2.","goff");
  recod0tree->Draw("d0pt>>d0genpt_reco","d0eta > -2. && d0eta < 2.","goff");
  recod0tree->Draw("d0pt>>d0genpt_reco_qua","d0eta > -2. && d0eta < 2. && d0allhighpurity > 0.5","goff");

  TH1F * recoeff_d0 = new TH1F("recoeff_d0","recoeff_d0",NPT,ptbins);
  recoeff_d0->Divide(d0genpt_reco,d0genpt,1,1,"B");

  TH1F * recoeff_d0_qua = new TH1F("recoeff_d0_qua","recoeff_d0_qua",NPT,ptbins);
  recoeff_d0_qua->Divide(d0genpt_reco_qua,d0genpt,1,1,"B");

  TCanvas * d0recoeff = new TCanvas("d0recoeff","d0recoeff");

  recoeff_d0->SetMarkerSize(0.7);
  recoeff_d0->SetMarkerStyle(20);
  recoeff_d0->SetTitle("");
  recoeff_d0->GetXaxis()->SetTitle("D pt (GeV)");
  recoeff_d0->GetYaxis()->SetTitle("(N with all dau reco)/N_{Gen}");
  recoeff_d0->GetYaxis()->SetRangeUser(0,1.0);
  recoeff_d0->SetStats(kFALSE);
  recoeff_d0->Draw("ep");
  recoeff_d0_qua->SetLineColor(2);
  recoeff_d0_qua->SetMarkerSize(0.7);
  recoeff_d0_qua->SetMarkerStyle(20);
  recoeff_d0_qua->SetMarkerColor(2);
  recoeff_d0_qua->Draw("epsame");


  TLegend * t = new TLegend(0.3,0.1,0.85,0.4);
  t->AddEntry(recoeff_d0, "All Dau Reco");
  t->AddEntry(recoeff_d0_qua, "All Dau Highpurity Reco Track");
  t->SetFillColor(0);
  t->Draw();


  TFile * output = new TFile("recoeff_d0.root","RECREATE");
  d0genpt->Write();
  d0genpt_reco->Write();
  d0genpt_reco_qua->Write();
  recoeff_d0->Write();
  recoeff_d0_qua->Write();
  d0recoeff->Write();

  output->Close();

}
