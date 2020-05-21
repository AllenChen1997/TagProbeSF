//this code is used to run the code with many sets
using namespace std;
#include "simple_plot.C"
void mutiapply_ST(){
	string filename[] = {"MC_ST_s.root","MC_ST_t_top.root","MC_ST_t_anti.root","MC_ST_tw_top.root","MC_ST_tw_anti.root"};
	string outputname[] = {"ST_s","ST_t_top","ST_t_anti","ST_tw_top","ST_tw_anti"};
	float xs[] = {3.74, 113.3, 67.91, 34.91, 34.97};
	int i =0;
	for (auto x : xs){
		string ifilename = (string) filename[i];
		string ioutputname = (string) outputname[i];
		cout << ifilename << " " << ioutputname << " " << x << endl;
		simple_plot(ifilename, ioutputname, x*1000);
		i++;
	}
}