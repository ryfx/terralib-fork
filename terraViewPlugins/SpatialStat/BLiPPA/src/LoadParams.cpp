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

void LoadParams(TeTheme *theme, std::vector<double> &x, std::vector<double> &y, std::vector<string> &objIds)
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
				objIds.push_back(obj.objectId());
				
				//pegar o centroide
				TeCoord2D coord;
				obj.centroid(coord);
				x.push_back(coord.x());
				y.push_back(coord.y());
			}
}