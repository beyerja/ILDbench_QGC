#include "TRandom3.h"
#include "TH3.h"
#include "TF3.h"
#include "TMath.h"

double myfunc3(double *x, double *par) {
double xx = (x[0] - par[1])/par[2];
double yy = (x[1] - par[3])/par[4];
double zz = (x[2] - par[5])/par[6];
double rx = TMath::Exp(-xx*xx);
double ry = TMath::Exp(-yy*yy);
double rz = TMath::Exp(-zz*zz);
double result = par[0]*rx*ry*rz;
return result;
}

void other() {
TH3F *h3 = new TH3F("h3","test",40,-3,3,40,-3,3,40,-4,4);
TRandom3 r;
for (int i=0;i<100000;i++) {
double x = r.Gaus(-0.2,0.6);
double y = r.Gaus(0.3,0.5);
double z = r.Gaus(0,1);
h3->Fill(x,y,z);
}
TF3 *func3 = new TF3("func3",myfunc3,-3,3,-3,3,-4,4,7);
func3->SetParameters(h3->GetMaximum(),0,0.5,0,0.5,0,0.5);
h3->Fit(func3);
h3->Draw();
func3->Draw("same");
}
