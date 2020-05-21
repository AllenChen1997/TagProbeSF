//this code is used to run the code with many sets
using namespace std;
#include "simple_plot.C"
void mutiapply(){
	string filename[] = {"MC_wjet_HT100to200.root","MC_wjet_HT200to400.root","MC_wjet_HT400to600.root",
	"MC_wjet_HT600to800.root","MC_wjet_HT800to1200.root","MC_wjet_HT1200to2500.root","MC_wjet_HT2500toInf.root"};
	string outputname[] = {"wjet_100to200","wjet_200to400","wjet_400to600","wjet_600to800",
	"wjet_800to1200","wjet_1200to2500","wjet_2500toInf"};
	float xs[] = {1395.0, 407.9, 57.48, 12.87, 5.366, 1.074, 0.008001};
	int i =0;
	for (auto x : xs){
		string ifilename = (string) filename[i];
		string ioutputname = (string) outputname[i];
		cout << ifilename << " " << ioutputname << " " << x << endl;
		simple_plot(ifilename, ioutputname, x);
		i++;
	}
}