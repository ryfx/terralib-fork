#include <LoadParams.h>
#include <TeGUIUtils.h> 
#include <TeQuerier.h>
#include <TeQuerierParams.h>
#include <TeDatabaseUtils.h>
#include <TeWaitCursor.h>
#include <TeSTEFunctionsDB.h>
#include <TeDataTypes.h>
#include <TeSTElementSet.h>

#include <qpushbutton.h>
#include <qcombobox.h>
#include <qcheckbox.h> 
#include <qradiobutton.h>
#include <qmessagebox.h>
#include <qbuttongroup.h> 
#include <qtabwidget.h>
#include <qtextbrowser.h> 
#include <qspinbox.h> 

///This function load the necessary parameters for the use of the Kfunction from the 
///TerraView database
void LoadParams(TeTheme *theme, std::vector<double> &x, std::vector<double> &y)
{
			TeQuerierParams params;
			
			params.setFillParams(true,false);

			params.setParams(theme);
			
			TeQuerier querier(params);
			if(!querier.loadInstances())
			{
				//QMessageBox::critical(this, tr("Error"), tr("Not possible to load data"));
				return;
			}

			TeSTInstance obj;
			TeSTElementSet objSet(theme);
			while(querier.fetchInstance(obj))
			{
				//armazenar as instancias do querier
				objSet.insertSTInstance(obj);
				//armazenar os identificadores
				//objIds.push_back(obj.objectId());
				
				//pegar o centroide
				TeCoord2D coord;
				obj.centroid(coord);
				x.push_back(coord.x());
				y.push_back(coord.y());

			}
}
TeBox BoxBiVariate(TeTheme *theme1,TeTheme *theme2)
{
//Descobre o Box entre eventos pontuais 1 e 2 e translada para (0,0))
			TeBox box1 = theme1->getThemeBox(); 
			TeBox box2 = theme2->getThemeBox();
			double lfx,lfy,a,b;
			TeBox boxFinal;
			(box1.x1() < box2.x1() ? lfx=box1.x1() : lfx=box2.x1());
			(box1.x2() < box2.x2() ? a=box1.x2() : a=box2.x2());
			(box1.y1() < box2.y1() ? lfy=box1.y1() : lfy=box2.y1());
			(box1.y2() < box2.y2() ? b=box1.y2() : b=box2.y2());

			boxFinal = makeBox(lfx,lfy,a,b);
			return(boxFinal);
}
/*
void BoxBiVariate(TeTheme *theme1,TeTheme *theme2, vector<double> &coordx1, vector<double> &coordy1,
				  vector<double> &coordx2, vector<double> &coordy2, double &a, double &b)
{
//Descobre o Box entre eventos pontuais 1 e 2 e translada para (0,0))
			TeBox box1 = theme1->getThemeBox();
			TeBox box2 = theme2->getThemeBox();
			int lfx,lfy;
			(box1.x1() < box2.x1() ? lfx=box1.x1() : lfx=box2.x1());
			(box1.x2() < box2.x2() ? a=box1.x2() : a=box2.x2());
			(box1.y1() < box2.y1() ? lfy=box1.y1() : lfy=box2.y1());
			(box1.y2() < box2.y2() ? b=box1.y2() : b=box2.y2());

			a -= lfy;
			b -= lfx;

			for(unsigned int i=0; i<coordx1.size(); ++i)
			{
				coordx1[i] -= lfx;
				coordy1[i] -= lfy;
			}
			for(i=0; i<coordx2.size(); ++i)
			{
				coordx2[i] -= lfx;
				coordy2[i] -= lfy;
			}
}*/
/*
void BoxUniVariate(TeTheme *theme1, vector<double> &coordx1, vector<double> &coordy1, double &a, double &b)
{
			//Descobre o Box entre eventos pontuais 1 e 2 e translada para (0,0))
			TeBox box1 = theme1->getThemeBox();
			
			for(unsigned int i=0; i<coordx1.size(); ++i)
			{
				coordx1[i] -= box1.x1();
				coordy1[i] -= box1.y1();
			}
			a = box1.x2()-box1.x1();
			b = box1.y2()-box1.y1();
}*/