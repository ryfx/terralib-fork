// tvps
#include <QueryShortcut.h>

// TerraLib
#include <TeAppTheme.h>
#include <TerraViewBase.h>

// Qt
#include <qaction.h>
#include <qmessagebox.h>

QueryShortcut::QueryShortcut(TerraViewBase* tv)
: Shortcut(tv)
{}

QueryShortcut::~QueryShortcut()
{}

bool QueryShortcut::isQueriedTheme()
{
    TeAppTheme* appTheme = _tv->currentTheme();
    if(appTheme == 0)
        return false;

    TeAbstractTheme* abst = appTheme->getTheme();
    TeTheme* theme = dynamic_cast<TeTheme*>(abst);
    if(theme == 0 || theme->layer()->hasGeometry(TeRASTER))
    {
        QMessageBox::information(_tv, tr("Information"), tr("This operation is not allowed for this current theme type!"));	
        return false; 
    }

    return true;
}
    
void QueryShortcut::spatialQuery()
{
    if(isQueriedTheme())
         _tv->spatialQuery();
}

void QueryShortcut::attributeQuery()
{
    if(isQueriedTheme())
        _tv->attributesQuery();  
}
