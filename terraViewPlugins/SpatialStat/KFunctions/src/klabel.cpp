#include <KFunctionPluginWindow.h>
#include <klabel.h>
#include <math.h>
#include <vector>
#include <valarray>

////////////////////////////////////////////////////////////////////////////
int Klabel_Stoyan(const TeBox &box, int n1, int n2, int nh, double dmax,const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h, std::valarray<double> &khat)
{
	std::valarray<double> khat1, khat2;
	unsigned int i;

	for( i=0; i<nh+1; i++)
		khat[i] = 0.0;

	khat1.resize(nh+1,0.0);
	khat2.resize(nh+1,0.0);

	KStoyan(box,n1,nh,dmax,x1,y1,h,khat1);
	KStoyan(box,n2,nh,dmax,x2,y2,h,khat2);
	
	for(i=0; i<nh+1 ; i++)
		khat[i] = khat1[i] - khat2[i];

return 0;
}

////////////////////////////////////////////////////////////////////////////
int Klabel_Ripley(const TeBox &box, int n1, int n2, int nh, double dmax,const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h, std::valarray<double> &khat)
{
	std::valarray<double> khat1, khat2;
	unsigned int i;

	for( i=0; i<nh+1; i++)
		khat[i] = 0.0;

	khat1.resize(nh+1,0.0);
	khat2.resize(nh+1,0.0);

	KRipley(box,n1,nh,dmax,x1,y1,h,khat1);
	KRipley(box,n2,nh,dmax,x2,y2,h,khat2);
	
	for(i=0; i<nh+1 ; i++)
		khat[i] = khat1[i] - khat2[i];

return 0;
}

////////////////////////////////////////////////////////////////////////////

int KlabelM(const TeBox &box, int n1, int n2, int nh, double dmax,const std::vector<double> &x1,const std::vector<double> &y1, 
			const std::vector<double> &x2, const std::vector<double> &y2, const std::vector<double> &h,  
			const std::vector<double> &m1,  const std::vector<double> &m2, std::valarray<double> &khat, int funcId, bool L)
{
	std::valarray<double> khat1, khat2;
	unsigned int i;

	for( i=0; i<nh+1; i++)
		khat[i] = 0.0;

	khat1.resize(nh+1,0.0);
	khat2.resize(nh+1,0.0);

			switch(funcId)
			{
				case 0:
					{
						KStoyanM(box,n1,nh,dmax,x1,y1,h,m1,khat1);
						KStoyanM(box,n2,nh,dmax,x2,y2,h,m2,khat2);
						break;
					}
			}
	
	for(i=0; i<nh+1; i++)
		khat[i] = khat1[i] - khat2[i];

return 0;
}