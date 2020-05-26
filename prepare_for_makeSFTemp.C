/*
	this is used to prepare the TH1 in root file for the makeSFTemplates.C input
	we have 4 pt regions for the cut [200-400],[400-600],[600-800],[>800]
*/
#include <iostream>
#include <sstream>
#include <TTree.h>
#include <TFile.h>
#include <TH3D.h>
#include <TH2D.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TAxis.h>

// the options in this code
bool rune = true;
bool runmu = false;
string inputfile = "./list.txt";
string cutname="./TH3_output.root";
string outputfile = "MC.root";
// the def. in cut histo.

#define pt_r1 200
#define pt_r2 400
#define pt_r3 600
#define pt_r4 800

#define Nmass 18
#define Minmass 40
#define Maxmass 130

#define Nrho 20
#define Minrho -6
#define Maxrho -1
double d = (double)(Maxrho-Minrho)/(double)Nrho; // the width in rho histo.
bool already_sum_total_mcweight = false;
string branch1 = "monoHbb_Tope_boosted";
string branch2 = "monoHbb_Topmu_boosted";

using namespace std;
void runcut(string inputname, string branchname, TH2D* hcut, TTree& tree, float& Puppijet0_N2DDT, float& Puppijet0_Matching,  float& Puppijet0_msd, float& Puppijet0_pt, float& output_weight, TH1F*& mcweight){  
	TFile* fin = new TFile((TString)inputname,"READ");
	TTreeReader myRead((TString)branchname,fin);
	TTreeReaderValue< Double_t > N2(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	TTreeReaderValue< Double_t > mass(myRead,"FJetMass");
	TTreeReaderValue< Double_t > weight(myRead,"weight");
	TTreeReaderValue< Double_t > st_TopMatching(myRead,"st_TopMatching");
	TH1F* h_event = (TH1F*) fin->Get("h_total_mcweight");
	int N = myRead.GetEntries();
	int x,y;
	int i=0;
	if (! already_sum_total_mcweight){
		if (mcweight->GetMaximum() == 0 ){
			mcweight = (TH1F*) h_event->Clone("mcweight");
		} else{
			mcweight->Add(h_event);
		}
	}
	while(myRead.Next()){
		if (*pt >= pt_r1 && *pt < pt_r2 ) y = 1;
		else if (*pt >= pt_r2 && *pt < pt_r3) y = 2;
		else if (*pt >= pt_r3 && *pt < pt_r4) y = 3;
		else if (*pt >= pt_r4) y = 4;
		else continue;
		// decide in which rho region
		if (*rho < Minrho) continue;
		x = (ceil)(*rho - Minrho) / d;
		// get N2DDT
		double N2cut = hcut->GetBinContent(x,y);
		if (N2cut == 0) continue;
		Puppijet0_N2DDT = *N2 - N2cut;
		Puppijet0_Matching = *st_TopMatching;
		Puppijet0_msd = *mass;
		Puppijet0_pt = *pt;
		output_weight = *weight;
		tree.Fill();
	}

}


void prepare_for_makeSFTemp(string inputname = inputfile, string outputname = outputfile){
	if (! gSystem->AccessPathName((TString)outputname)){
		cout << "file exist! please check the directory ./" << endl;
		exit(0);
	}
	TFile* fcut = new TFile((TString)cutname,"READ");
	TH2D* h_cut = (TH2D*) fcut->Get("h_pt_rho");
	
	// setup output file
	float Puppijet0_N2DDT, Puppijet0_Matching, Puppijet0_msd, Puppijet0_pt, weight;
	TH1F* mcweight = new TH1F("","",10,0,10);
	TFile* fout = new TFile((TString)outputname,"NEW");
	TTree outTree("otree2","out branches");
	outTree.Branch("Puppijet0_N2DDT", &Puppijet0_N2DDT);
	outTree.Branch("Puppijet0_Matching", &Puppijet0_Matching);
	outTree.Branch("Puppijet0_msd", &Puppijet0_msd);
	outTree.Branch("Puppijet0_pt", &Puppijet0_pt);
	outTree.Branch("weight", &weight);
	// use MC hist list file as input
	int iMC=0;
	ifstream fin((TString)inputname);
	string line;
	while (getline(fin,line)){
		if (rune){
			runcut(line,branch1,h_cut,outTree,Puppijet0_N2DDT, Puppijet0_Matching, Puppijet0_msd, Puppijet0_pt, weight, mcweight); 
			already_sum_total_mcweight = true;
		}
		if (runmu){
			runcut(line,branch2,h_cut,outTree,Puppijet0_N2DDT, Puppijet0_Matching, Puppijet0_msd, Puppijet0_pt, weight, mcweight); 
		}
		already_sum_total_mcweight = false;
	}
	fout->cd();	
mcweight->Write();
	fout->Write();
	fout->Close();
	
	
}
