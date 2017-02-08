#ifndef __WCSCLIENT_UTILS_H_
#define __WCSCLIENT_UTILS_H_

//TerraLib include files
#include <TeBox.h>

//STL include files
#include <string>
#include <vector>

namespace TeOGC
{
  class TeWCSCoverageDescription;
  class TeWCSCoverageDescriptions;
};

class WCSServInfo;
class WCSLayersInfo;

class QListView;
class QString;
class QLabel;
class QWidget;

struct WCSCoverageParams
{
  WCSCoverageParams()
  {
  }

  WCSCoverageParams(const WCSCoverageParams& other)
  {
    img_crs_ = other.img_crs_;
    img_format_ = other.img_format_;
    img_id_ = other.img_id_;
    img_box_ = other.img_box_;
  }

  std::string img_crs_,
    img_format_,
    img_id_,
    serv_version_;

  TeBox img_box_;
};


/**
  \brief fills the list view with informations about the service.
  \param [in]  serv_url WCS service url.
  \param [out] infoList List with the information.
  \param [out] serv_title Label to be used to show title as info. If NOT null, update service title label.
  \param [out] layersList List with the available layers. If NOT null, fill layers information list.
  \return A vector of the coverage identifiers.
 */
std::vector<std::string> fillServiceInformation ( const QString& serv_url, std::string& serv_version, WCSServInfo* serverInfo, WCSLayersInfo* layersInfo );

/**
  \brief
  \param
  \param
 */
TeOGC::TeWCSCoverageDescriptions* describeCoverages ( const std::string& serv_url, const std::string& serv_version, const std::vector<std::string>& cover_ids );

/**
  \brief
  \param oid
  \param descs
 */
TeOGC::TeWCSCoverageDescription* findDescription( const std::string& oid, const std::vector<TeOGC::TeWCSCoverageDescription*>& descs );

/**
  \brief
  \param
  \param
  \param
 */
std::string fillCoverageInfo( const std::vector<TeOGC::TeWCSCoverageDescription*>& descs, const QString& cov_id, WCSLayersInfo* layersInfo );

/**
  \brief
  \param
  \param
 */
void getImage(const std::string& serv_url, const WCSCoverageParams& par, QWidget* wid);

/**
  \brief
  \param crsFrom
  \param crsTo
  \param box
 */
void remapBox(const std::string& crsFrom, const std::string& crsTo, TeBox& box);

/**
  \brief
  \param desc
  \param crs
  \param box
  \return
 */
int getBoundingRect (const TeOGC::TeWCSCoverageDescription& desc, std::string& crs, TeBox& box);

#endif //__WCSCLIENT_UTILS_H_