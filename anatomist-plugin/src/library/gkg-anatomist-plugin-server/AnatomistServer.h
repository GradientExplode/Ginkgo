#ifndef _gkg_anatomist_plugin_server_AnatomistServer_h_
#define _gkg_anatomist_plugin_server_AnatomistServer_h_


#include <gkg-core-pattern/Singleton.h>
#include <gkg-core-pattern/RCPointer.h>
#include <gkg-core-cppext/StdInt.h>
#include <gkg-processing-container/Volume.h>

#include <cartobase/object/object.h>
#include <anatomist/processor/Processor.h>
#include <anatomist/window/qwindow.h>
#include <anatomist/volume/Volume.h>
#include <anatomist/reference/Referential.h>
#include <anatomist/surface/glcomponent.h>

#include <string>
#include <set>
#include <map>


namespace gkg
{


class MouseEvent;


class AnatomistServer : public Singleton< AnatomistServer >
{

  public:

    enum Option
    {

      Normal = 0,
      NoDecoration,
      NoBorder

    };

    enum WindowType
    {

      Axial,
      Coronal,
      Sagittal,
      ThreeD,
      Unknown

    };

    QWidget* createWindow(
                          const std::string& name,
                          const std::string& type, 
                          int32_t x0, 
                          int32_t y0,
                          int32_t width,
                          int32_t height,
                          Option option = (Option)( NoDecoration | NoBorder ) );
    void muteAxial( const std::string& windowName );
    void muteSagittal( const std::string& windowName );
    void muteCoronal( const std::string& windowName );
    void mute3D( const std::string& windowName );
    void closeWindows();

    void updateViews( bool adjustPalette = true );
    void addObjectsToViews();
    
    void removeAllObjects();
    void removeAllObjectsAndWindows();
    void adjustCamera( float posX, float posY, float posZ, float posT = 1 );
    void setPalette( const std::string name,
                     bool min1flg = false, float min1 = 0.0,
                     bool max1flg = false, float max1 = 1.0,
                     bool absmode = false );
    void setPalette( std::set< std::string > obj,
                     const std::string name,
                     bool min1flg = false, float min1 = 0.0,
                     bool max1flg = false, float max1 = 1.0,
                     bool absmode = false );

    void addObject( anatomist::AObject* aObject, const std::string& name );
    void removeObject( const std::string& name );

    void fusion2D( const std::string& vol1,
                   const std::string& vol2,
                   const std::string& method,
                   const std::string& name );

    void fusion2DParams( const std::string& obj, float rate,
                         int32_t mode = anatomist::GLComponent::glLINEAR );

    bool addObjectToWindow( std::set< std::string > obj,
                            std::set< std::string > win );

    void applyTransformation( std::set< std::string > oOrg,
                              std::set< std::string > wOrg,
                              const std::string& refOrg,
                              std::set< std::string > oDst,
                              std::set< std::string > wDst,
                              const std::string& refDest,
                              const float matrix[ 4 ][ 3 ] );
    void applyTransformation( std::set< std::string > oOrg,
                              const std::string& refOrg,
                              std::set< std::string > oDst,
                              const std::string& refDest,
                              const float matrix[ 4 ][ 3 ] );

    void createReferential( const std::string& name );
    void assignReferential( const std::string& object,
                            const std::string& referential );
    void unregisterReferentials();

    anatomist::AObject* getAnatomistObject( const std::string& name ) const;
    bool hasObject( const std::string& name ) const;

    void registerMouseEvent( MouseEvent* event );
    void setControl( const std::string& name, 
                     const std::set< std::string >& windows );

    Vector3d< float > getCurrentPosition( const std::string& referential );

    void focusView( const std::string& name );
    void activateDoubleClippingMode( const std::string& name );
    void setClippingDistance( const std::string& name, float value );

    void getViewStateInformation( const anatomist::ViewState& viewState,
                                  Vector3d< float >& position,
                                  AnatomistServer::WindowType& windowType,
                                  float& clippingDistance  ) const;

  protected:

    AnatomistServer();
    virtual ~AnatomistServer();

    friend class Singleton< AnatomistServer >;

  private:

    void updateViews( std::set< anatomist::AObject* > obj,
                      bool adjustPalette = true );
    void setPalette( std::set< anatomist::AObject* > obj,
                     const std::string name,
                     bool min1flg = false, float min1 = 0.0,
                     bool max1flg = false, float max1 = 1.0,
                     bool absmode = false );
    void applyTransformation( std::set< anatomist::AObject* > oOrg,
                              std::set< anatomist::AWindow* > wOrg,
                              anatomist::Referential* refOrg,
                              std::set< anatomist::AObject* > oDst,
                              std::set< anatomist::AWindow* > wDst,
                              anatomist::Referential* refDest,
                              const float matrix[ 4 ][ 3 ] );

    anatomist::Processor _processor;

    std::map< std::string, QAWindow* > _windows;
    std::map< std::string, anatomist::AObject* > _objects;
    std::map< std::string, anatomist::Referential* > _referentials;

};


}


#endif
