// used to print out mass distribution
// to see the effection if subtract that
// build by Kung-Hsiang Chen

using namespace std;

float lumi = 41.5;
float xs_semi = 687.1*0.438*1000;
bool isonlytarget = true;

void run_cut(string inputname,TH1F* & hpass,TH1F* & hfail){
	TFile* fin = new TFile((TString)inputname,"READ");
	TTreeReader myRead("otree2",fin);
	TTreeReaderValue< float > N2(myRead, "Puppijet0_N2DDT");
	TTreeReaderValue< float > pt(myRead, "Puppijet0_pt");
	TTreeReaderValue< float > mass(myRead, "Puppijet0_msd");
	TH1F* h_event = (TH1F*) fin->Get("mcweight");
	while(myRead.Next()){
		if (*pt < 200) continue;
		if (*mass > 120 || *mass < 50) continue;
		if (*N2 > 0) hfail->Fill(*mass);
		else hpass->Fill(*mass);
	}
	cout << hpass->Integral() << " " << h_event->Integral() << endl;
	hpass->Scale(1.0/h_event->Integral());
	hfail->Scale(1.0/h_event->Integral());
	
}


void simple_plot(string inputname="MC_TT2e2mu.root", string outputname="2e2mu",float xs = 687.1*0.105){
	TH1F* h_pass = new TH1F("pass",(TString)outputname,20,50,120);
	TH1F* h_pass_semi = (TH1F*) h_pass->Clone("pass_semi");
	TH1F* h_fail = (TH1F*) h_pass->Clone("fail");
	TH1F* h_fail_semi = (TH1F*) h_pass->Clone("fail_semi");
	run_cut(inputname,h_pass,h_fail);
	run_cut("MC_TTsemi.root",h_pass_semi,h_fail_semi);
	
	TCanvas* c1 = new TCanvas("c1","c1");
	h_pass_semi->Scale(xs_semi*lumi);
	h_pass->Scale(xs*lumi);
	h_pass->SetLineColor(kRed);
	if (isonlytarget) h_pass->Draw("");
	else{
		if (h_pass_semi->GetMaximum() > h_pass->GetMaximum() ){
			h_pass_semi->Draw("");
			h_pass->Draw("SAME");
		}
		else {
			h_pass->Draw("");		
			h_pass_semi->Draw("SAME");
		}
	}
	//cout << h_pass->GetMaximum() << " " << h_pass_semi->GetMaximum() << endl;
	//cout << "pass inte " << h_pass->Integral() << endl;
	
	if( isonlytarget) c1->SaveAs(Form("%s_pass_nosemi.png",outputname.c_str()));
	else c1->SaveAs(Form("%s_pass.png",outputname.c_str()));
	
	TCanvas* c2 = new TCanvas("c2","c2");
	h_fail_semi->Scale(xs_semi*lumi);
	h_fail->Scale(xs*lumi);
	h_fail->SetLineColor(kRed);
	if (isonlytarget) h_fail->Draw("");
	else{
		if (h_fail_semi->GetMaximum() > h_fail->GetMaximum()){
			h_fail_semi->Draw("");
			h_fail->Draw("SAME");
		}
		else{
			h_fail->Draw("");
			h_fail_semi->Draw("SAME");
		}
	}
	if (isonlytarget) c2->SaveAs(Form("%s_fail_nosemi.png",outputname.c_str()));
	else c2->SaveAs(Form("%s_fail.png",outputname.c_str()));
}
	