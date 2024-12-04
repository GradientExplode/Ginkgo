#ifndef _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_AnatomistViewer_h_
#define _gkg_anatomist_plugin_GkgDwiGlobalTractographyViewer_AnatomistViewer_h_


#include "AGlobalTractographyObject.h"

#include <gkg-core-cppext/StdInt.h>

#include <QtGui/QWidget>
#include <QtGui/QLayout>
#include <QtCore/QEvent>

#include <string>


namespace gkg
{


class AnatomistViewer : public QWidget
{


  public:

    enum Type
    {

      Axial,
      Sagittal,
      Coronal,
      ThreeD

    };

    AnatomistViewer( int32_t windowWidth, QWidget* parent = 0 );
    virtual ~AnatomistViewer();

    void reset();
  
    void createWindow( Type type, 
                       uint32_t division, 
                       uint32_t row, 
                       uint32_t column,
                       const std::string& name );

    void addGlobalTractographyObject(
                         AGlobalTractographyObject* aGlobalTractographyObject );

  private:

    int32_t _windowWidth;
    
    bool _grid1;
    bool _grid4;

    QGridLayout* _grid2Layout;
    QGridLayout* _grid4Layout[ 2 ][ 2 ];

};


}


#endif

