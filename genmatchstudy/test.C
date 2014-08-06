{
	TFile * f =  new TFile("recogenmatch_d0dstar_cut.root");
	TTree * d0tree = (TTree *) f->Get("d0tree");
	TH1F * d0mass =  new TH1F("d0mass","d0mass",35,1.70,2.05);
	d0tree->Draw("d0fm>>d0mass","d0pt>40 && d0passingtopocuts > 0.5 && d0eta > -2 && d0eta < 2", "goff");

	TFile * fd =  new TFile("recogenmatch_d0dstar_cut_double.root");
	TTree * d0treed = (TTree *) fd->Get("d0tree");
	TH1F * d0massd =  new TH1F("d0massd","d0massd",35,1.70,2.05);
	d0treed->Draw("d0fm>>d0massd","d0pt>40 && d0passingtopocuts > 0.5 && d0eta > -2 && d0eta < 2", "goff");

	TH1F * diff = new TH1F("diff","diff",35,1.70,2.05);
	diff->Add(d0massd,d0mass,1.0,-1.0);
//	TF1* fun = new TF1("fun", "[0]*(1/[2]/sqrt(6.28)*exp(-0.5*pow((x-[1])/[2], 2)))", 1.70, 2.05);
//	fun->SetParameter(1, 1.86);
//	fun->SetParameter(2, 0.01);
//	d0mass->Fit(fun, "L");
    diff->SetLineColor(2);
	d0massd->Draw();
	diff->Draw("same");
}
