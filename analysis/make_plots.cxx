#include "TRint.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"

#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>

using namespace std;
// ===============================================================================================================================
int main(int argc, char ** argv) {

#ifdef WITHRINT
	TRint *myapp = new TRint("RootSession",&argc,argv,NULL,0);
#else
	TApplication *myapp = new TApplication("myapp",0,0);
#endif

	// ------------------------------------------------------------------------------
	// Loading data
	TFile * Fin = new TFile("CheckTracks.root");
	TTree * T = (TTree*) Fin -> Get("ntt");
	float mcZOut,recZOut,mcPhiOut,recPhiOut,mcThetaOut,recThetaOut,mcPhi,recPhi,mcLam,recLam,mcPt,recPt,ipD,ipZ,label;
	T -> SetBranchAddress("mcZOut"     ,&mcZOut     );
	T -> SetBranchAddress("recZOut"    ,&recZOut    );
	T -> SetBranchAddress("mcPhiOut"   ,&mcPhiOut   );
	T -> SetBranchAddress("recPhiOut"  ,&recPhiOut  );
	T -> SetBranchAddress("mcThetaOut" ,&mcThetaOut );
	T -> SetBranchAddress("recThetaOut",&recThetaOut);
	T -> SetBranchAddress("mcPhi"      ,&mcPhi      );
	T -> SetBranchAddress("recPhi"     ,&recPhi     );
	T -> SetBranchAddress("mcLam"      ,&mcLam      );
	T -> SetBranchAddress("recLam"     ,&recLam     );
	T -> SetBranchAddress("mcPt"       ,&mcPt       );
	T -> SetBranchAddress("recPt"      ,&recPt      );
	T -> SetBranchAddress("ipD"        ,&ipD        );
	T -> SetBranchAddress("ipZ"        ,&ipZ        );
	T -> SetBranchAddress("label"      ,&label      );
	int nEntries = T -> GetEntries();
	// ------------------------------------------------------------------------------
	// Creating histograms
	TH1F * h1_mcZOut     = new TH1F("h1_mcZOut"     ,";h1_mcZOut"     ,100,-  2,  0);
	TH1F * h1_recZOut    = new TH1F("h1_recZOut"    ,";h1_recZOut"    ,100,-100,100);
	TH1F * h1_mcPhiOut   = new TH1F("h1_mcPhiOut"   ,";h1_mcPhiOut"   ,100,-  2,  0);
	TH1F * h1_recPhiOut  = new TH1F("h1_recPhiOut"  ,";h1_recPhiOut"  ,100,-  1,  8);
	TH1F * h1_mcThetaOut = new TH1F("h1_mcThetaOut" ,";h1_mcThetaOut" ,100,-  2,  0);
	TH1F * h1_recThetaOut= new TH1F("h1_recThetaOut",";h1_recThetaOut",100,   0,  4);
	TH1F * h1_mcPhi      = new TH1F("h1_mcPhi"      ,";h1_mcPhi"      ,100,-  6,  6);
	TH1F * h1_recPhi     = new TH1F("h1_recPhi"     ,";h1_recPhi"     ,100,-  6,  6);
	TH1F * h1_mcLam      = new TH1F("h1_mcLam"      ,";h1_mcLam"      ,100,-  2,  2);
	TH1F * h1_recLam     = new TH1F("h1_recLam"     ,";h1_recLam"     ,100,-  2,  2);
	TH1F * h1_mcPt       = new TH1F("h1_mcPt"       ,";h1_mcPt"       ,100,-  2,  6);
	TH1F * h1_recPt      = new TH1F("h1_recPt"      ,";h1_recPt"      ,100,-  2,  6);
	TH1F * h1_ipD        = new TH1F("h1_ipD"        ,";h1_ipD"        ,100,-  2,  2);
	TH1F * h1_ipZ        = new TH1F("h1_ipZ"        ,";h1_ipZ"        , 50,- 20, 20);
	TH1F * h1_label      = new TH1F("h1_label"      ,";h1_label"      ,100,- 10,2e5);
	// ------------------------------------------------------------------------------
	// Looping over data
	for(int ev = 0 ; ev < nEntries ; ev++){
		T -> GetEntry(ev);

		h1_mcZOut     -> Fill(mcZOut     );
		h1_recZOut    -> Fill(recZOut    );
		h1_mcPhiOut   -> Fill(mcPhiOut   );
		h1_recPhiOut  -> Fill(recPhiOut  );
		h1_mcThetaOut -> Fill(mcThetaOut );
		h1_recThetaOut-> Fill(recThetaOut);
		h1_mcPhi      -> Fill(mcPhi      );
		h1_recPhi     -> Fill(recPhi     );
		h1_mcLam      -> Fill(mcLam      );
		h1_recLam     -> Fill(recLam     );
		h1_mcPt       -> Fill(mcPt       );
		h1_recPt      -> Fill(recPt      );
		h1_ipD        -> Fill(ipD        );
		h1_ipZ        -> Fill(ipZ        );
		h1_label      -> Fill(label      );
	}
	// ------------------------------------------------------------------------------
	// Plotting results
	TCanvas * c1 = new TCanvas("c1","c1",1300,900);
	c1 -> Divide(5,3);

	c1 -> cd( 1);	h1_mcZOut     -> Draw();
	c1 -> cd( 2);   h1_recZOut    -> Draw();
	c1 -> cd( 3);   h1_mcPhiOut   -> Draw();
	c1 -> cd( 4);   h1_recPhiOut  -> Draw();
	c1 -> cd( 5);   h1_mcThetaOut -> Draw();
	c1 -> cd( 6);   h1_recThetaOut-> Draw();
	c1 -> cd( 7);   h1_mcPhi      -> Draw();
	c1 -> cd( 8);   h1_recPhi     -> Draw();
	c1 -> cd( 9);   h1_mcLam      -> Draw();
	c1 -> cd(10);   h1_recLam     -> Draw();
	c1 -> cd(11);   h1_mcPt       -> Draw();
	c1 -> cd(12);   h1_recPt      -> Draw();
	c1 -> cd(13);   h1_ipD        -> Draw();
	c1 -> cd(14);   h1_ipZ        -> Draw();
	c1 -> cd(15);   h1_label      -> Draw();

	c1 -> Modified();
	c1 -> Update();
	// ------------------------------------------------------------------------------
	myapp -> Run();
	return 0;
} // End of main function
// ===============================================================================================================================
