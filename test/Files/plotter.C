#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "TRandom3.h"
#include "TH1F.h"
#include "TH1.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TH2.h"
#include "TProfile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TGraph.h"
#include "TColor.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TClonesArray.h"

#include <sys/types.h>
#include <dirent.h>

#include <TMath.h>
#include <TGraph2D.h>
#include <TRandom2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TF2.h>
#include <TVirtualFitter.h>
#include <TPolyLine3D.h>
#include <Math/Vector3D.h>
#include <TVector3.h>
#include <TMultiGraph.h>
#include <TH2Poly.h>

using namespace std;using namespace ROOT::Math;

struct edata
{
	string *name;
// 	Int_t reel;
	vector <vector <Int_t>> *adc;
	vector <string> *cname;
// 	Int_t adc[576][64];
// 	string cname[576];
};
edata ed;

struct capdata
{
	string name;
	int mapind;
	float base[4];
	float baseerr[4];
	int Nevt[4];
	int badcap;
	float max[4];
	TH1D* Histo1[4];
	TH1D* Histo2[4];
	TH1D* Histo1comb;
	int valid;//1 yes 0 no
};
capdata cd;
vector <capdata> CD;

struct semapin
{
	int crate;
	int slot;
	int fiber;
	int channel;
	string PMTname;
	string Channelname;
	string winchester;
	int qiecard;
	string qiecrate;
	int qieslot;
	int PMTid;
	int BBid;
	int ieta;
	int iphi;
	int depth;
	int box;
	string boxname;
	string boxbarcode;
	int VA;
	int VB;
	int VC;
	
	float SX5gain;
	float SX5gainerr;
	float B904pedestal;
	float B904pedestalsigma;
	
	int wedge;
	string EH;
	int tower;
};
semapin semin;

struct semap
{
	int crate;
	int slot;
	int fiber;
	int channel;
	string PMTname;
	string Channelname;
	string winchester;
	int qiecard;
	int ieta;
	int iphi;
	int depth;
	int box;
	string boxname;
	
	float SX5gain;
	float SX5gainerr;
	float B904pedestal;
	
	int wedge;
	string EH;
	int tower;
};
semap sem;vector <semap> MP;

int RunNo=0;

float linADC[65]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,18,20,22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,54,56,60,64,68,72,76,80,84,88,92,96,100,104,108,112,116,120,124,128,132,136,140,148,156,164,172,180,188,196};

float adc2fC_QIE10[64]={

  // =========== RANGE 0 ===========

  // --------- subrange 1 ---------
  -14.45,-11.35,-8.25,-5.15,-2.05,1.05,4.15,7.25,10.35,13.45,16.55,19.65,22.75,25.85,28.95,32.05,
  // --------- subrange 2 ---------
  36.7,42.9,49.1,55.3,61.5,67.7,73.9,80.1,86.3,92.5,98.7,104.9,111.1,117.3,123.5,129.7,135.9,142.1,148.3,154.5,
  // --------- subrange 3 ---------
  163.8,176.2,188.6,201.0,213.4,225.8,238.2,250.6,263.0,275.4,287.8,300.2,312.6,325.0,337.4,349.8,362.2,374.6,387.0,399.4,411.8,
  // --------- subrange 4 ---------
  430.4,455.2,480.0,504.8,529.6,554.4,579.2};

int getmap()
{
	ifstream semapinf("semapex_src.txt");
	while(!semapinf.eof())
	{
		semapinf>>semin.crate>>semin.slot>>semin.fiber>>semin.channel>>semin.PMTname>>semin.Channelname>>semin.winchester>>semin.qiecard>>semin.qiecrate>>semin.qieslot>>semin.PMTid>>semin.BBid>>semin.ieta>>semin.iphi>>semin.depth>>semin.box>>semin.boxname>>semin.boxbarcode>>semin.VA>>semin.VB>>semin.VC>>semin.SX5gain>>semin.SX5gainerr>>semin.B904pedestal>>semin.B904pedestalsigma>>semin.wedge>>semin.EH>>semin.tower;
		
// 		cout<<semin.crate<<" "<<semin.slot<<" "<<semin.fiber<<" "<<semin.channel<<endl;
		
		sem.crate=semin.crate;
		sem.slot=semin.slot;
		sem.fiber=semin.fiber;
		sem.channel=semin.channel;
		sem.PMTname=semin.PMTname;
		sem.Channelname=semin.Channelname;
		sem.winchester=semin.winchester;
		sem.qiecard=semin.qiecard;
		sem.ieta=semin.ieta;
		sem.iphi=semin.iphi;
		sem.depth=semin.depth;
		sem.box=semin.box;
		sem.boxname=semin.boxname;
		sem.SX5gain=semin.SX5gain;
		sem.SX5gainerr=semin.SX5gainerr;
		sem.B904pedestal=semin.B904pedestal;
		sem.wedge=semin.wedge;
		sem.EH=semin.EH;
		sem.tower=semin.tower;
		MP.push_back(sem);
	}
	semapinf.close();
	
// 	for(int i1=0;i1<SEM.size();i1++)
// 	{
// 		cout<<SEM[i1].crate<<" "<<SEM[i1].slot<<" "<<SEM[i1].fiber<<" "<<SEM[i1].channel<<" "<<SEM[i1].ieta<<" "<<SEM[i1].iphi<<" "<<SEM[i1].depth<<" "<<SEM[i1].wedge<<" "<<SEM[i1].EH<<" "<<SEM[i1].tower<<endl;
// 	}
}

vector<int> findLocation(string sample, char findIt)
{
    vector<int> characterLocations;
    for(int i =0; i < sample.size(); i++)
        if(sample[i] == findIt)
            characterLocations.push_back(i);

    return characterLocations;
}

string between(string const &in, string const &before, string const &after)
{
	return in.substr(in.find(before)+before.size(), 2);
}

int reNTuple()
{
	cout<<"Run :"<<RunNo<<endl;
	char hname[500];
	
	sprintf(hname,"H_%d.root",RunNo);
	TFile* outroot2=new TFile(hname,"recreate");
	TH1F* NormChi2=new TH1F("NormChi2","NormChi2",100,0.,20.);
	TH1F* MeanADCSpec=new TH1F("MeanQSpec","MeanQSpec",2000,0.,20.);
	TH1F* MeanQSpecAll=new TH1F("MeanQSpecAll","MeanQSpecAll",2000,0.,20.);
	
	sprintf(hname,"../NTuples/N_%d.root",RunNo);
	TFile* inroot=new TFile(hname);
	TTree *tree = (TTree*)inroot->Get("Events");
	tree->SetBranchAddress("name",&ed.name);
// 	tree->SetBranchAddress("reel",&ed.reel);
	tree->SetBranchAddress("adc",&ed.adc);
	tree->SetBranchAddress("cname",&ed.cname);
	
	TH1F* QperEventAll=new TH1F("QperEventAll","QperEventAll",tree->GetEntries(),-0.5,tree->GetEntries()-0.5);
	
	string name;vector <int> loc;
	int crate=0;int slot=0;int fiber=0;int channel=0;
	int MI=-1;
	
	TCanvas* cc1=new TCanvas("cc1","cc1",600,600);
	TF1* tf1=new TF1("tf1","gaus",0.,35.);
	TF1* tf2=new TF1("tf2","[0]",0.,30000.);
	
	tree->GetEntry(0);
	string hnamepre="";
	int capind=0;int Nevt=0;int cdind=0;
	int cieta=0;int ciphi=0;
	for(int i1=0;i1<ed.cname->size();i1++)
	{
		name=ed.cname->at(i1);
		loc=findLocation(name,'_');
		crate=atoi(name.substr(loc[0]+1,loc[1]-loc[0]-1).c_str());
		slot=atoi(name.substr(loc[1]+1,loc[2]-loc[1]-1).c_str());
		fiber=atoi(name.substr(loc[2]+1,loc[2]-loc[2]-1).c_str());
		channel=atoi(name.substr(loc[3]+1,name.size()-loc[3]-1).c_str());
		loc.clear();
		MI=-1;
		for(int i2=0;i2<MP.size();i2++)
		{
			if(MP[i2].crate==crate && MP[i2].slot==slot && MP[i2].fiber==fiber && MP[i2].channel==channel)
			{
				MI=i2;break;
			}
		}
		if(MI==-1) continue;
		cdind=-1;
		for(int is1=0;is1<CD.size();is1++)
		{
			if(name==CD[is1].name){cdind=is1;break;}
		}
		if(cdind==-1)
		{
			cd.name=name;
			cd.mapind=MI;
			cd.badcap=-1;
			cd.valid=0;
// 			cd.tname=(*ed.name);
			for(int i4=0;i4<4;i4++)
			{
// 				cd.startbin[i4]=0;
				cd.base[i4]=0;
				cd.baseerr[i4]=0;
				cd.Nevt[i4]=0;
				cd.max[i4]=0;
				
				sprintf(hname,"Histo1_%d_%d_%d_cap%d",MP[MI].ieta,MP[MI].iphi,MP[MI].depth,i4);
				cd.Histo1[i4]=new TH1D(hname,hname,63,adc2fC_QIE10);
				cd.Histo1[i4]->GetXaxis()->SetTitle("Charge (fC)");cd.Histo1[i4]->GetXaxis()->CenterTitle();
				sprintf(hname,"Events / Charge Bin");cd.Histo1[i4]->GetYaxis()->SetTitle(hname);cd.Histo1[i4]->GetYaxis()->CenterTitle();
				
				sprintf(hname,"Histo2_%d_%d_%d_cap%d",MP[MI].ieta,MP[MI].iphi,MP[MI].depth,i4);
				cd.Histo2[i4]=new TH1D(hname,hname,63,adc2fC_QIE10);
				cd.Histo2[i4]->GetXaxis()->SetTitle("Charge (fC)");cd.Histo2[i4]->GetXaxis()->CenterTitle();
				sprintf(hname,"Events / Charge Bin");cd.Histo2[i4]->GetYaxis()->SetTitle(hname);cd.Histo2[i4]->GetYaxis()->CenterTitle();
			}
			sprintf(hname,"Histo1comb_%d_%d_%d",MP[MI].ieta,MP[MI].iphi,MP[MI].depth);
			cd.Histo1comb=new TH1D(hname,hname,63,adc2fC_QIE10);
			cd.Histo1comb->GetXaxis()->SetTitle("Charge (fC)");cd.Histo1comb->GetXaxis()->CenterTitle();
			sprintf(hname,"Events / Charge Bin");cd.Histo1comb->GetYaxis()->SetTitle(hname);cd.Histo1comb->GetYaxis()->CenterTitle();
			CD.push_back(cd);
			cdind=CD.size()-1;
			
// 			cout<<cd.name<<" "<<(*ed.name)<<" "<<MP[MI].ieta<<" "<<MP[MI].iphi<<endl;
		}
		if(name!=hnamepre) {capind=0;}
		else capind++;
		Nevt=0;
		bool badcap=false;
		for(int i2=0;i2<ed.adc->at(i1).size();i2++)
		{
			Nevt+=ed.adc->at(i1)[i2];
			if(i2>59 && ed.adc->at(i1)[i2]>0) badcap=true;
		}
		CD[cdind].Nevt[capind]=Nevt;
		if(badcap) CD[cdind].badcap=capind;
// 		cout<<name<<" "<<capind<<" "<<Nevt<<endl;
		hnamepre=name;
	}
	cout<<endl<<"... 1/3 ..."<<endl<<endl;
	
	for(int i=0;i<tree->GetEntries();i++)
	{
		tree->GetEntry(i);
// 		if(ed.reel>20) continue;
		string hnamepre="";int capind=0;
// 		cout<<i<<" "<<ed.cname->at(0)<<endl;
		for(int i1=0;i1<ed.cname->size();i1++)
		{
			name=ed.cname->at(i1);
			cdind=-1;
			for(int i2=0;i2<CD.size();i2++)
			{
				if(CD[i2].name==name){cdind=i2;break;}
			}
			if(cdind==-1)continue;
			
			if(name!=hnamepre) capind=0;
			else capind++;
			if(capind!=CD[cdind].badcap)
			{
				MI=CD[cdind].mapind;
				for(int iz2=0;iz2<60;iz2++)
				{
					CD[cdind].Histo1[capind]->Fill(adc2fC_QIE10[iz2],ed.adc->at(i1)[iz2]);
					CD[cdind].Histo1comb->Fill(adc2fC_QIE10[iz2],ed.adc->at(i1)[iz2]);
// 					if(i<=836) CD[cdind].Histo1[capind]->Fill(adc2fC_QIE10[iz2],ed.adc->at(i1)[iz2]);
// 					else CD[cdind].Histo2[capind]->Fill(adc2fC_QIE10[iz2],ed.adc->at(i1)[iz2]);
				}
			}
			hnamepre=name;
		}
		if(i%100==0) cout<<"Event :"<<i<<" / "<<tree->GetEntries()<<endl;
	}
// 	TF1* tf3=new TF1("tf3","gaus",0.,200.);
// 	for(int i2=0;i2<CD.size();i2++)
// 	{
// 		for(int i4=0;i4<4;i4++)
// 		{
// 			if(i4==CD[i2].badcap) continue;
// 			tf3->SetParameter(1,4.);
// 			tf3->SetParameter(2,0.8);
// 			CD[i2].PPedDist[i4]->Fit(tf3,"q","q",0.,100.);
// // 			cout<<i2<<" "<<i4<<" "<<tf3->GetParameter(1)<<" "<<tf3->GetParError(1)<<" "<<tf3->GetParameter(2)<<" "<<tf3->GetParError(2)<<" "<<((int)ceil(tf3->GetParameter(1)+3.*tf3->GetParameter(2)))<<endl;
// // 			outroot2->cd();
// // 			CD[i2].PPedDist[i4]->Write();
// // 			CD[i2].startbin=((int)ceil(tf3->GetParameter(1)+3.5*tf3->GetParameter(2)));
// 
// 			CD[i2].startbin[i4]=((int)ceil(tf3->GetParameter(1)+3.5*tf3->GetParameter(2)));
// // 			CD[i2].startbin[i4]=((int)ceil(tf3->GetParameter(1)+0.5+3.5*tf3->GetParameter(2)));
// 			
// // 			cout<<i2<<" "<<i4<<" "<<(tf3->GetParameter(1)+3.5*tf3->GetParameter(2))<<" "<<CD[i2].startbin[i4]<<endl;
// // 			if(RunNo==289146 && MP[CD[i2].mapind].box==-17 && MP[CD[i2].mapind].PMTname=="B5")
// // 			{
// // 				CD[i2].startbin[i4]=6;
// // 				cout<<CD[i2].name<<" "<<MP[CD[i2].mapind].PMTname<<" "<<MP[CD[i2].mapind].Channelname<<" "<<(tf3->GetParameter(1)+3.5*tf3->GetParameter(2))<<" "<<CD[i2].startbin[i4]<<endl;
// // 			}
// 		}
// 	}
	
	
	outroot2->cd();
	for(int i2=0;i2<CD.size();i2++)
	{
		CD[i2].Histo1comb->Write();
// 		for(int i4=0;i4<4;i4++)
// 		{
// 			if(i4!=CD[i2].badcap)
// 			{
// 				CD[i2].Histo1[i4]->Write();
// 	// 			CD[i2].Histo2[i4]->Write();
// 			}
// 		}
	}
	
	outroot2->Close();
	inroot->Close();
}

int main(int argc, char *argv[])
{
	RunNo=atoi(argv[1]);
	getmap();
	
	reNTuple();
}

















