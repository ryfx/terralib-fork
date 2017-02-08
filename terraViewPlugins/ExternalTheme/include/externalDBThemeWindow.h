
#ifndef EXTERNALDBPLUGINWINDOW_H
#define EXTERNALDBPLUGINWINDOW_H

#include <externalDBTheme.h>
#include <PluginParameters.h>
#include <help.h>

//Forward declarations
class QAction;
class QPopupMenu;

class externalDBThemeWindow : public externalDBTheme
{

Q_OBJECT

public:
	externalDBThemeWindow(QWidget* parent);
	~externalDBThemeWindow();

	virtual void clearDBAndThemeVector();
	virtual void fillDatabaseTable();
    virtual bool connectByPool();
    virtual bool connectByParameters();
    virtual void initTreeView();
    virtual std::string checkRemoteTheme(const int& externalDatabaseId, const int& externalThemeId);
    virtual void getThemeProperties(TeTheme* item );
    virtual TeTheme* getTheme( std::string& viewName, std::string& themeName );
    virtual bool getDatabaseStatus(TeDatabase* remoteDb, bool & status );
	
public slots:

	virtual void updateForm(PluginParameters* params);
    virtual void init(TeDatabase* currentDB, int viewId);
    virtual void createPushButton_clicked();
    virtual void nextOnePushButton_clicked();
    virtual void databaseTable_clicked( int row, int, int, const QPoint & );
	virtual void databaseTypeComboBox_activated( const QString & value );
    virtual void dbComboBox_activated( const QString & dbName );
    virtual void directoryPushButton_clicked();
    virtual void previousTwoPushButton_clicked();
    virtual void nextTwoPushButton_clicked();
    virtual void externalListView_clicked( QListViewItem * themeItem );
    virtual void previousThreePushButton_clicked();
    virtual void okPushButton_clicked();
    virtual void databaseClickedVHeader( int row );
    virtual void removePushButton_clicked();
	virtual void helpPushButton_clicked();

//	virtual void show();

protected:

	QPopupMenu* findThemeMenu();

	virtual void hideEvent(QHideEvent*);

    virtual bool themeNameExistsInView(TeView* view, const std::string& themeName);

	PluginParameters*	plugIngParams_;
	int					externalDatabaseId_;
    bool				selectedDb_;
    TeDatabase*			currentDatabase_;
    TeDatabase*			newDb_;
    int					viewId_;
    std::vector<TeAppTheme*> themeVec_;
    TeQtCanvas*			canvas_;
	Help*				help_;

    QAction* pluginAction_;   

	std::vector<int> mnuIdxs_;
};

#endif

