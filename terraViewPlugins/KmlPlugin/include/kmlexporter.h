#ifndef TDK_KMLEXPORTER_H
#define TDK_KMLEXPORTER_H

#include <string>
#include <vector>

//Forward declarations
class TeTheme;
class TeLayer;
namespace tdk 
{
class KMLObject; 
class KMLPlacemark;
class KMLDocument;
class KMLFeature;
class KMLManager;
}
class QListView;
class QListViewItem;
class QPixmap;

class TeProjection;
class TePolygon;
class TeBox;
class TeCoord2D;
class TeLine2D;
class TePoint;
class TePointSet;
class TeLineSet;
class TePolygonSet;

using namespace std;
using namespace tdk;

//vector<tdk::KMLDocument*> theme2KML(TeTheme* theme, const string& descTable, const string& descColumn);
vector<tdk::KMLFeature*> theme2KML(TeTheme* theme, const string& descTable, const string& nameColumn, const string& descColumn);

vector<tdk::KMLFeature*> layer2KML(TeLayer* layer, const string& descTable, const string& nameColumn, const string& descColumn);

int convertTransparency2Alpha(int transparency);

void changeProjection(TeProjection* fromProj, TeProjection* toProj, TeBox& box);

void changeProjection(TeProjection* fromProj, TeProjection* toProj, TeCoord2D& coord);

void changeProjection(TeProjection* fromProj, TeProjection* toProj, TePoint& point);

void changeProjection(TeProjection* fromProj, TeProjection* toProj, TePointSet& pointSet);

void changeProjection(TeProjection* fromProj, TeProjection* toProj, TeLine2D& line);

void changeProjection(TeProjection* fromProj, TeProjection* toProj, TeLineSet& lineSet);

void changeProjection(TeProjection* fromProj, TeProjection* toProj, TePolygon& poly);

void changeProjection(TeProjection* fromProj, TeProjection* toProj, TePolygonSet& polygonSet);

TeBox tetrahedronBBox(const TeCoord2D& pt0, const TeCoord2D& pt1, const TeCoord2D& pt2, const TeCoord2D& pt3);

#endif