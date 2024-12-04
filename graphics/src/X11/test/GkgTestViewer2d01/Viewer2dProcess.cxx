#include "Viewer2dProcess.h"

#include <gkg-graphics-X11-core/Session.h>
#include <gkg-graphics-X11-core/LookAndFeel.h>
#include <gkg-graphics-X11-core/LayoutFactory.h>
#include <gkg-graphics-X11-core/ApplicationWindow.h>
#include <gkg-graphics-X11-core/MonoGlyph.h>
#include <gkg-graphics-X11-core/Display.h>
#include <gkg-graphics-X11-viewer2d/VolumeModel.h>
#include <gkg-graphics-X11-viewer2d/Viewer2d.h>

#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-io/Reader_i.h>

#include <gkg-processing-colorimetry/RGBComponent.h>

#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-algobase/TestFunctionFactory.h>

#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::Viewer2dProcess::Viewer2dProcess( const float& scale,
                                       const bool& verbose,
                                       int& result )
                     : gkg::Process(),
                       _scale( scale ),
                       _verbose( verbose ),
                       _result( result )
{

  try
  {

    registerProcess( "Volume", gkg::TypeOf< int8_t >::getName(),
                     &gkg::Viewer2dProcess::view< int8_t > );
    registerProcess( "Volume", gkg::TypeOf< uint8_t >::getName(),
                     &gkg::Viewer2dProcess::view< uint8_t > );
    registerProcess( "Volume", gkg::TypeOf< int16_t >::getName(),
                     &gkg::Viewer2dProcess::view< int16_t > );
    registerProcess( "Volume", gkg::TypeOf< uint16_t >::getName(),
                     &gkg::Viewer2dProcess::view< uint16_t > );
    registerProcess( "Volume", gkg::TypeOf< int32_t >::getName(),
                     &gkg::Viewer2dProcess::view< int32_t > );
    registerProcess( "Volume", gkg::TypeOf< uint32_t >::getName(),
                     &gkg::Viewer2dProcess::view< uint32_t > );
    registerProcess( "Volume", gkg::TypeOf< int64_t >::getName(),
                     &gkg::Viewer2dProcess::view< int64_t > );
    registerProcess( "Volume", gkg::TypeOf< uint64_t >::getName(),
                     &gkg::Viewer2dProcess::view< uint64_t > );
    registerProcess( "Volume", gkg::TypeOf< float >::getName(),
                     &gkg::Viewer2dProcess::view< float > );
    registerProcess( "Volume", gkg::TypeOf< double >::getName(),
                     &gkg::Viewer2dProcess::view< double > );
//    registerProcess( "Volume", gkg::TypeOf< gkg::RGBComponent >::getName(),
//                     &gkg::Viewer2dProcess::view< gkg::RGBComponent > );

  }
  GKG_CATCH( "gkg::Viewer2dProcess::Viewer2dProcess( "
             "float scale, "
             "bool verbose, "
             "int& result )" );

}


const float& gkg::Viewer2dProcess::getScale() const
{

  return _scale;

}


const bool& gkg::Viewer2dProcess::getVerbose() const
{

  return _verbose;

}


int& gkg::Viewer2dProcess::getResult() const
{

  return _result;

}


template < class T >
void gkg::Viewer2dProcess::view( gkg::Process& process,
                                 const std::string& fileNameIn,
                                 const gkg::AnalyzedObject&,
                                 const std::string& )
{

  try
  {

    gkg::Viewer2dProcess&
      viewer2dProcess = static_cast< gkg::Viewer2dProcess& >( process );

    // reading volume
    if ( viewer2dProcess.getVerbose() == true  )
    {

      std::cout << "reading " << fileNameIn << " : " << std::flush;

    }
    gkg::Volume< T > volume;
    gkg::Reader::getInstance().read( fileNameIn, volume );
    if ( viewer2dProcess.getVerbose() == true )
    {

      std::cout << "done" << std::endl;

    }

    // building volume model
    gkg::VolumeModel< T >*
      volumeModel = new gkg::VolumeModel< T >( volume,
                                               viewer2dProcess.getVerbose() );

    // Graphical User Interface initialization
    gkg::LayoutFactory& layoutFactory = gkg::LayoutFactory::getInstance();

    if ( viewer2dProcess.getVerbose() )
    {

      std::cout << "span X = " << volumeModel->getSpanX() << "mm " << std::endl;
      std::cout << "span Y = " << volumeModel->getSpanY() << "mm " << std::endl;

    }

    gkg::Viewer2d< T >*
      viewer2d = new gkg::Viewer2d< T >( volumeModel );

    // test the update function
    gkg::Volume< T > volume_out;
    gkg::TestFunction< T >* testFunction =
      gkg::TestFunctionFactory< T >::getInstance().createTestFunction( 
                                                                   "ge",
                                                                   T( 5000 ) );
    gkg::Binarizer< gkg::Volume< T >, gkg::Volume< T > > binarizer(
                                                                  *testFunction,
							          T( 32767 ),
							          T( 0 ) );
    binarizer.binarize( volume, volume_out );
    viewer2d->update( volume_out );

    gkg::MonoGlyph*
      layoutFixed = layoutFactory.createFixed(
                         viewer2d,
                         volumeModel->getSpanX() * viewer2dProcess.getScale(),
                         volumeModel->getSpanY() * viewer2dProcess.getScale() );

    gkg::ApplicationWindow*
      aWindow = new gkg::ApplicationWindow( layoutFixed );

    viewer2dProcess.getResult() = gkg::Session::getInstance().runWindow(
                                                                      aWindow );

    delete volumeModel;

  }
  GKG_CATCH( "template < class T > "
             "void gkg::Viewer2dProcess::view( gkg::Process& process, "
             "const std::string& fileNameIn, "
             "const gkg::AnalyzedObject&, "
             "const std::string& )" );

}
