#ifndef __TVPS_PLUGIN_DISPLAY_SHORTCUT_H_
#define __TVPS_PLUGIN_DISPLAY_SHORTCUT_H_

// tvps
#include <Shortcut.h>

// Qt
#include <qobject.h>

// forward declarations
class TerraViewBase;
class DisplayWindow;

class DisplayShortcut : public Shortcut
{
    Q_OBJECT

public:

    DisplayShortcut(TerraViewBase* tv);

	virtual ~DisplayShortcut();

protected slots:

   virtual void autoPromote();
   virtual void graphicScaleVisual();
   virtual void showGraphicScale();
   virtual void geographicalGridVisual();
   virtual void showGeographicalGrid();
   virtual void insertSymbol();
   virtual void insertText();
   virtual void deletePointedObjects();
   virtual void drawSelectedOnCenter();
   virtual void animation();
   virtual void displaySize();

protected:

    DisplayWindow* _display;

};

#endif // __TVPS_PLUGIN_DISPLAY_SHORTCUT_H_
