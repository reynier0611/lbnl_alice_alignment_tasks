#include "TRint.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TStyle.h"
#include "TLegend.h"

#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>

const double pi = TMath::Pi();

using namespace std;

//Forward-declaring functions
void prettyTH1( TH1F * h1 , int color , bool log=false , TString ytit = "");
void prettyTH2( TH2F * h2 );
double get_pseudorapidity( double theta){return -TMath::Log(TMath::Tan(theta/2.));}
// ===============================================================================================================================
int main(int argc, char ** argv) {

#ifdef WITHRINT
	TRint *myapp = new TRint("RootSession",&argc,argv,NULL,0);
#else
	TApplication *myapp = new TApplication("myapp",0,0);
#endif

	TH1::SetDefaultSumw2();
	TH2::SetDefaultSumw2();
	gStyle -> SetOptStat(0);
	// ------------------------------------------------------------------------------
	// Loading data
	//TFile * Fin = new TFile("CheckTracks.root"); -> Now called CheckTracks_1.root
	//TFile * Fin = new TFile("pp_Check_Tracks.root"); -> Now deleted
	//TFile * Fin = new TFile("229739x_Tracks.root"); -> Now called CheckTracks_2.root
	TFile * Fin = new TFile("CheckTracks_combined.root");
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
	// Nominal histograms from branches in root file
	TH1F * h1_mcZOut     = new TH1F("h1_mcZOut"     ,";mcZOut (hardcoded as -1)"           ,70,-  2,  0 );  prettyTH1(h1_mcZOut     ,1);
	TH1F * h1_recZOut    = new TH1F("h1_recZOut"    ,";recZOut"                            ,70,-100,100 );  prettyTH1(h1_recZOut    ,2);
	TH1F * h1_mcPhiOut   = new TH1F("h1_mcPhiOut"   ,";mcPhiOut (hardcoded as -1)"         ,70,-  2,  0 );  prettyTH1(h1_mcPhiOut   ,1);
	TH1F * h1_recPhiOut  = new TH1F("h1_recPhiOut"  ,";recPhiOut"                          ,70,-  1,  8 );  prettyTH1(h1_recPhiOut  ,2);
	TH1F * h1_mcThetaOut = new TH1F("h1_mcThetaOut" ,";mcThetaOut (hardcoded as -1)"       ,70,-  2,  0 );  prettyTH1(h1_mcThetaOut ,1);
	TH1F * h1_recThetaOut= new TH1F("h1_recThetaOut",";recThetaOut"                        ,70,   0, pi );  prettyTH1(h1_recThetaOut,2);
	TH1F * h1_mcPhi      = new TH1F("h1_mcPhi"      ,";mcPhi (#phi_{gen})"                 ,70,- pi, pi );  prettyTH1(h1_mcPhi      ,1);
	TH1F * h1_recPhi     = new TH1F("h1_recPhi"     ,";recPhi (#phi_{reco})"               ,70,- pi, pi );  prettyTH1(h1_recPhi     ,2);
	TH1F * h1_mcLam      = new TH1F("h1_mcLam"      ,";mcLam (#pi/2-#theta_{gen})"         ,70,-1.3,1.3 );  prettyTH1(h1_mcLam      ,1);
	TH1F * h1_recLam     = new TH1F("h1_recLam"     ,";recLam (#pi/2-#theta_{reco})"       ,70,-1.3,1.3 );  prettyTH1(h1_recLam     ,2);
	TH1F * h1_mcPt       = new TH1F("h1_mcPt"       ,";mcPt (p_{T, gen} [GeV/#it{c}])"     ,70,-  0,  8 );  prettyTH1(h1_mcPt       ,1);
	TH1F * h1_recPt      = new TH1F("h1_recPt"      ,";recPt (p_{T, reco} [GeV/#it{c}])"   ,70,-  0,  8 );  prettyTH1(h1_recPt      ,2);
	TH1F * h1_ipD        = new TH1F("h1_ipD"        ,";ipD (transverse impact parameter)"  ,70,- .1, .1 );  prettyTH1(h1_ipD        ,2);
	TH1F * h1_ipZ        = new TH1F("h1_ipZ"        ,";ipZ (longitudinal impact parameter)",50,- 20, 20 );  prettyTH1(h1_ipZ        ,2);
	TH1F * h1_label      = new TH1F("h1_label"      ,";label (Multiplicity?)"              ,99,- 10, 5e3);  prettyTH1(h1_label      ,2,true);
	TH1F * h1_label_copy = new TH1F("h1_label_copy" ,";label (Multiplicity?)"              ,99,- 10,12e4);	prettyTH1(h1_label_copy ,2,true);
	// ---------------
	// Additional histograms
	TH1F * h1_recTheta      = new TH1F("h1_recTheta"     ,";#theta_{rec} [rad]"                   ,70,  0, pi );	prettyTH1(h1_recTheta     ,62);
	TH1F * h1_mc_min_rec_pT = new TH1F("h1_mc_min_rec_pT",";p_{T, gen} - p_{T, reco} [GeV/#it{c}]",70, -1,  1 );	prettyTH1(h1_mc_min_rec_pT, 2);
	TH1F * h1_mcEta         = new TH1F("h1_mcEta"        ,";#eta"                                 ,70, -2,  2 );	prettyTH1(h1_mcEta        , 1);
	TH1F * h1_recEta        = new TH1F("h1_recEta"       ,";#eta"                                 ,70, -2,  2 );	prettyTH1(h1_recEta       , 2);

	double pTbins[] = {0,0.5,1,2,5};
	int size_pTbins = sizeof(pTbins)/sizeof(*pTbins);
	TH1F ** h1_mcPhi_pTbins                = new TH1F * [size_pTbins];
	TH1F ** h1_recPhi_pTbins               = new TH1F * [size_pTbins];
	TH1F ** h1_mcTheta_pTbins              = new TH1F * [size_pTbins];
	TH1F ** h1_recTheta_pTbins             = new TH1F * [size_pTbins];
	TH2F ** h2_mcThetaPhi_pTbins           = new TH2F * [size_pTbins];
	TH2F ** h2_recThetaPhi_pTbins          = new TH2F * [size_pTbins];
	TH1F ** h1_mc_min_rec_Phi              = new TH1F * [size_pTbins];
	TH1F ** h1_mc_min_rec_Theta            = new TH1F * [size_pTbins];
	TH2F ** h2_mc_v_rec_Phi                = new TH2F * [size_pTbins];
	TH2F ** h2_mc_v_rec_Theta              = new TH2F * [size_pTbins];
	TH2F ** h2_mc_min_rec_Phi_v_mc_Phi     = new TH2F * [size_pTbins];
	TH2F ** h2_mc_min_rec_Theta_v_mc_Theta = new TH2F * [size_pTbins];
	for(int pT = 0 ; pT < size_pTbins-1 ; pT++){
		TString title = Form("%.1f < p^{gen}_{T} < %.1f GeV/#it{c}",pTbins[pT],pTbins[pT+1]);

		h1_mcPhi_pTbins               [pT] = new TH1F(Form("h1_mcPhi_pTbins_%i"               ,pT),title+";#phi [rad]"                                          ,50,-pi,pi);
        	h1_recPhi_pTbins              [pT] = new TH1F(Form("h1_recPhi_pTbins_%i"              ,pT),title+";#phi [rad]"                                          ,50,-pi,pi);
        	h1_mcTheta_pTbins             [pT] = new TH1F(Form("h1_mcTheta_pTbins_%i"             ,pT),title+";#theta [rad]"                                        ,90, 0 ,pi);
        	h1_recTheta_pTbins            [pT] = new TH1F(Form("h1_recTheta_pTbins_%i"            ,pT),title+";#theta [rad]"                                        ,90, 0 ,pi);
        	h2_mcThetaPhi_pTbins          [pT] = new TH2F(Form("h2_mcThetaPhi_pTbins_%i"          ,pT),title+";#phi_{gen} [rad];#theta_{gen} [rad]"                 ,90,-pi,pi,90, 0 , pi);
        	h2_recThetaPhi_pTbins         [pT] = new TH2F(Form("h2_recThetaPhi_pTbins_%i"         ,pT),title+";#phi_{reco} [rad];#theta_{reco} [rad]"               ,90,-pi,pi,90, 0 , pi);
		h1_mc_min_rec_Phi             [pT] = new TH1F(Form("h1_mc_min_rec_Phi_%i"             ,pT),title+";#phi_{gen} - #phi_{rec} [rad]"                       ,90,-.02,.02);
		h1_mc_min_rec_Theta           [pT] = new TH1F(Form("h1_mc_min_rec_Theta_%i"           ,pT),title+";#theta_{gen} - #theta_{rec} [rad]"                   ,90,-.01,.01);
		h2_mc_v_rec_Phi               [pT] = new TH2F(Form("h2_mc_v_rec_Phi_%i"               ,pT),title+";#phi_{rec} [rad];#phi_{gen} [rad]"                   ,90,- pi, pi,90,- pi, pi);
		h2_mc_v_rec_Theta             [pT] = new TH2F(Form("h2_mc_v_rec_Theta_%i"             ,pT),title+";#theta_{rec} [rad];#theta_{gen} [rad]"               ,90,   0, pi,90,   0, pi);
		h2_mc_min_rec_Phi_v_mc_Phi    [pT] = new TH2F(Form("h2_mc_min_rec_Phi_v_mc_Phi_%i"    ,pT),title+";#phi_{gen} [rad];#phi_{gen} - #phi_{rec} [rad]"      ,90,- pi, pi,90,-.02,.02);
		h2_mc_min_rec_Theta_v_mc_Theta[pT] = new TH2F(Form("h2_mc_min_rec_Theta_v_mc_Theta_%i",pT),title+";#theta_{gen} [rad];#theta_{gen} - #theta_{rec} [rad]",50,   0, pi,90,-.01,.01);
	
		prettyTH1(h1_mcPhi_pTbins    [pT],1);	h1_mcPhi_pTbins    [pT] -> SetMinimum(0);
		prettyTH1(h1_recPhi_pTbins   [pT],2);	h1_recPhi_pTbins   [pT] -> SetMinimum(0);
		prettyTH1(h1_mcTheta_pTbins  [pT],1);	h1_mcTheta_pTbins  [pT] -> SetMinimum(0);
		prettyTH1(h1_recTheta_pTbins [pT],2);	h1_recTheta_pTbins [pT] -> SetMinimum(0);
		prettyTH1(h1_mc_min_rec_Phi  [pT],1);	h1_mc_min_rec_Phi  [pT] -> SetMinimum(0);
		prettyTH1(h1_mc_min_rec_Theta[pT],1);	h1_mc_min_rec_Theta[pT] -> SetMinimum(0);
	
		prettyTH2(h2_mcThetaPhi_pTbins          [pT]);
		prettyTH2(h2_recThetaPhi_pTbins         [pT]);
		prettyTH2(h2_mc_v_rec_Phi               [pT]);
		prettyTH2(h2_mc_v_rec_Theta             [pT]);
		prettyTH2(h2_mc_min_rec_Phi_v_mc_Phi    [pT]);
		prettyTH2(h2_mc_min_rec_Theta_v_mc_Theta[pT]);
	}
	// ---
	// ------------------------------------------------------------------------------
	// Looping over data
	for(int ev = 0 ; ev < nEntries ; ev++){
		T -> GetEntry(ev);

		double mcTheta  = pi/2. - mcLam;
		double recTheta = pi/2. - recLam;
		double mcEta    = get_pseudorapidity(mcTheta );
		double recEta   = get_pseudorapidity(recTheta);

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
		h1_label_copy -> Fill(label      );

		h1_recTheta      -> Fill(recTheta);
		h1_mc_min_rec_pT -> Fill(mcPt-recPt);
		h1_mcEta         -> Fill(mcEta);
		h1_recEta        -> Fill(recEta);

		for(int pT = 0 ; pT < size_pTbins-1 ; pT++){
			if(mcPt>pTbins[pT]&&mcPt<pTbins[pT+1]){
				h1_mcPhi_pTbins               [pT] -> Fill( mcPhi  );
                		h1_recPhi_pTbins              [pT] -> Fill( recPhi );
                		h1_mcTheta_pTbins             [pT] -> Fill( mcTheta  );
                		h1_recTheta_pTbins            [pT] -> Fill( recTheta );
                		h2_mcThetaPhi_pTbins          [pT] -> Fill( mcPhi  , mcTheta  );
        	        	h2_recThetaPhi_pTbins         [pT] -> Fill( recPhi , recTheta );
				h1_mc_min_rec_Phi             [pT] -> Fill( mcPhi - recPhi );
                		h1_mc_min_rec_Theta           [pT] -> Fill( mcTheta - recTheta );
                		h2_mc_v_rec_Phi               [pT] -> Fill( recPhi , mcPhi );
                		h2_mc_v_rec_Theta             [pT] -> Fill( recTheta , mcTheta );
				h2_mc_min_rec_Phi_v_mc_Phi    [pT] -> Fill( mcPhi , mcPhi - recPhi );
                		h2_mc_min_rec_Theta_v_mc_Theta[pT] -> Fill( mcTheta , mcTheta - recTheta );
				break;
			}
		}
	}
	// ------------------------------------------------------------------------------
	// Plotting results
	TCanvas * c1 = new TCanvas("c1","c1",1300,900);
	c1 -> Divide(4,4);
	c1 -> cd( 1); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_mcZOut     -> Draw(); h1_mcZOut     -> Draw("samehist");
	c1 -> cd( 2); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_recZOut    -> Draw(); h1_recZOut    -> Draw("samehist");
	c1 -> cd( 3); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_mcPhiOut   -> Draw(); h1_mcPhiOut   -> Draw("samehist");
	c1 -> cd( 4); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_recPhiOut  -> Draw(); h1_recPhiOut  -> Draw("samehist");
	c1 -> cd( 5); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_mcThetaOut -> Draw(); h1_mcThetaOut -> Draw("samehist");
	c1 -> cd( 6); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_recThetaOut-> Draw(); h1_recThetaOut-> Draw("samehist");
	c1 -> cd( 7); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_mcPhi      -> Draw(); h1_mcPhi      -> Draw("samehist");
	c1 -> cd( 8); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_recPhi     -> Draw(); h1_recPhi     -> Draw("samehist");
	c1 -> cd( 9); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_mcLam      -> Draw(); h1_mcLam      -> Draw("samehist");
	c1 -> cd(10); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_recLam     -> Draw(); h1_recLam     -> Draw("samehist");
	c1 -> cd(11); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_mcPt       -> Draw(); h1_mcPt       -> Draw("samehist");
	c1 -> cd(12); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_recPt      -> Draw(); h1_recPt      -> Draw("samehist");
	c1 -> cd(13); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_ipD        -> Draw(); h1_ipD        -> Draw("samehist");
	c1 -> cd(14); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); h1_ipZ        -> Draw(); h1_ipZ        -> Draw("samehist");
	c1 -> cd(15); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); gPad->SetLogy(); h1_label      -> Draw(); h1_label      -> Draw("samehist");
	c1 -> cd(16); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.16); gPad->SetLogy(); h1_label_copy -> Draw(); h1_label_copy -> Draw("samehist");
	c1 -> Modified();
	c1 -> Update();
	// -------------------------------------------
	TLegend * leg1 = new TLegend(0.3,0.3,0.7,0.6);
	leg1 -> SetLineColor(0);
	leg1 -> AddEntry( h1_mcPhi_pTbins [0],"gen" );
	leg1 -> AddEntry( h1_recPhi_pTbins[0],"reco");
	// -------------------------------------------
	TCanvas ** c2 = new TCanvas * [size_pTbins-1];
	for(int pT = 0 ; pT < size_pTbins-1 ; pT++){
		c2[pT] = new TCanvas(Form("c2_%i",pT),Form("c2_%i",pT),1300,900);
		c2[pT] -> Divide(5,2);
		c2[pT] -> cd( 1); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.22);	h1_mcPhi_pTbins               [pT] -> Draw(      );	h1_recPhi_pTbins   [pT] -> Draw("same"    );	leg1 -> Draw("same");
		c2[pT] -> cd( 2); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.22);       h2_mcThetaPhi_pTbins          [pT] -> Draw("COLZ");
		c2[pT] -> cd( 3); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.22);       h1_mc_min_rec_Phi             [pT] -> Draw(      );	h1_mc_min_rec_Phi  [pT] -> Draw("samehist");
		c2[pT] -> cd( 4); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.22);       h2_mc_v_rec_Phi               [pT] -> Draw("COLZ");
		c2[pT] -> cd( 5); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.24);       h2_mc_min_rec_Phi_v_mc_Phi    [pT] -> Draw("COLZ");
		c2[pT] -> cd( 6); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.22);	h1_mcTheta_pTbins             [pT] -> Draw(      );	h1_recTheta_pTbins [pT] -> Draw("same"    );
		c2[pT] -> cd( 7); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.22);	h2_recThetaPhi_pTbins         [pT] -> Draw("COLZ");
		c2[pT] -> cd( 8); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.22);	h1_mc_min_rec_Theta           [pT] -> Draw(      );	h1_mc_min_rec_Theta[pT] -> Draw("samehist");
		c2[pT] -> cd( 9); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.22);	h2_mc_v_rec_Theta             [pT] -> Draw("COLZ");
		c2[pT] -> cd(10); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.24);	h2_mc_min_rec_Theta_v_mc_Theta[pT] -> Draw("COLZ");
		c2[pT] -> Modified();
		c2[pT] -> Update();
	}
	// -------------------------------------------
	TLegend * leg2 = new TLegend(0.3,0.3,0.7,0.6);
        leg2 -> SetLineColor(0);
        leg2 -> AddEntry( h1_recTheta   ,"calculated"    );
        leg2 -> AddEntry( h1_recThetaOut,"from root file");
        // -------------------------------------------
	TLegend * leg3 = new TLegend(0.3,0.3,0.7,0.6);
        leg3 -> SetLineColor(0);
        leg3 -> AddEntry( h1_mcEta  ,"generated"    );
        leg3 -> AddEntry( h1_recEta ,"reconstructed");
        // -------------------------------------------
	TCanvas * c3 = new TCanvas("c3","c3",1300,900);
	c3 -> Divide(2,2);
	c3 -> cd(1); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.14);
	h1_recTheta    -> Draw(      ); h1_recTheta    -> Draw("samehist");
	h1_recThetaOut -> Draw("same"); h1_recThetaOut -> Draw("samehist");
	leg2 -> Draw("same");
	c3 -> cd(2); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.14);
	h1_mcEta  -> Draw(      ); h1_mcEta  -> Draw("samehist");
	h1_recEta -> Draw("same"); h1_recEta -> Draw("samehist");
	leg3 -> Draw("same");
	c3 -> Modified();
        c3 -> Update();
	// -------------------------------------------
	TLegend * leg4 = new TLegend(0.3,0.3,0.7,0.6);
        leg4 -> SetLineColor(0);
        leg4 -> AddEntry( h1_mcPt ,"generated"    );
        leg4 -> AddEntry( h1_recPt,"reconstructed");
        // -------------------------------------------
	TCanvas * c4 = new TCanvas("c4","c4",1300,900);
	c4 -> Divide(2,2);
	c4 -> cd(1); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.14);
	TH1F * h1_mcPt_copy = (TH1F*) h1_mcPt -> Clone();
	h1_mcPt_copy -> SetName("h1_mcPt_copy");
	h1_mcPt_copy -> GetXaxis() -> SetTitle("p_{T} [GeV/#it{c}]");
        h1_mcPt_copy -> Draw(      ); h1_mcPt  -> Draw("samehist");
	h1_recPt     -> Draw("same"); h1_recPt -> Draw("samehist");
	leg4 -> Draw("same");
	c4 -> cd(2); gPad->SetBottomMargin(0.14); gPad->SetLeftMargin(0.14);
	h1_mc_min_rec_pT -> Draw("hist"); h1_mc_min_rec_pT -> Draw("same");
	c4 -> Modified();
	c4 -> Update();
	// ------------------------------------------------------------------------------
	// Saving canvases onto a pdf
	TString pdf_name = "results.pdf";
	c1 -> Print(pdf_name+"(");
	c3 -> Print(pdf_name);
	c4 -> Print(pdf_name);
	for(int pT = 0 ; pT < size_pTbins-1 ; pT++){
		if(pT==size_pTbins-2) c2[pT] -> Print(pdf_name+")");
		else c2[pT] -> Print(pdf_name);
	}
	// ------------------------------------------------------------------------------
	myapp -> Run();
	return 0;
} // End of main function
// ===============================================================================================================================
void prettyTH1( TH1F * h1 , int color , bool log , TString ytit ){
	h1 -> GetXaxis() -> CenterTitle();
	h1 -> GetXaxis() -> SetTitleSize(0.06);
	h1 -> GetXaxis() -> SetLabelSize(0.06);
	h1 -> GetXaxis() -> SetNdivisions(107);

	h1 -> GetYaxis() -> CenterTitle();
	h1 -> GetYaxis() -> SetTitleSize(0.06);
	h1 -> GetYaxis() -> SetLabelSize(0.06);
	h1 -> GetYaxis() -> SetNdivisions(107);

	h1 -> SetLineColor(color);
	h1 -> SetMarkerColor(color);

	if(ytit=="")
		h1 -> GetYaxis() -> SetTitle("tracks");
	else
		h1 -> GetYaxis() -> SetTitle(ytit);

	if (!log)
		h1 -> SetMinimum(0);
}
// ===============================================================================================================================
void prettyTH2( TH2F * h2 ){
        h2 -> GetXaxis() -> CenterTitle();
        h2 -> GetXaxis() -> SetTitleSize(0.06);
        h2 -> GetXaxis() -> SetLabelSize(0.06);
        h2 -> GetXaxis() -> SetNdivisions(107);

        h2 -> GetYaxis() -> CenterTitle();
        h2 -> GetYaxis() -> SetTitleSize(0.06);
        h2 -> GetYaxis() -> SetLabelSize(0.06);
        h2 -> GetYaxis() -> SetNdivisions(107);
}
