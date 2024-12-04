#include "AnatomicalPriorsCallback.h"
#include "GlobalTractographyCallbackIds.h"
#include "GlobalTractographyService.h"
#include <gkg-dmri-global-tractography/SpinGlass.h>
#include <gkg-dmri-global-tractography/GlobalTractographyAlgorithm.h>
#include <gkg-dmri-container/SpinGlassAndConnectionMap.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


gkg::AnatomicalPriorsCallback::AnatomicalPriorsCallback( void* parameter,
                                                         bool verbose )
                             : gkg::RpcCallback(
                                                gkg::ANATOMICAL_PRIORS_CALLBACK,
                                                parameter ),
                               _verbose( verbose )
{
}


gkg::AnatomicalPriorsCallback::~AnatomicalPriorsCallback()
{
}


void gkg::AnatomicalPriorsCallback::receive( gkg::SocketStream& stream )
{

  try
  {

    gkg::GlobalTractographyService* globalTractographyService =
        reinterpret_cast< gkg::GlobalTractographyService* >( this->_parameter );

    const gkg::RCPointer< gkg::SpinGlassAndConnectionMap >&
      spinGlassAndConnectionMap =
                      globalTractographyService->getSpinGlassAndConnectionMap();

    // receiving the ROI size
    gkg::Vector3d< int32_t > roiSize;
    stream.read( ( char* )&( roiSize.x ), sizeof( int32_t ) );
    stream.read( ( char* )&( roiSize.y ), sizeof( int32_t ) );
    stream.read( ( char* )&( roiSize.z ), sizeof( int32_t ) );

    // receiving the ROI resolution
    gkg::Vector3d< double > roiResolution;
    stream.read( ( char* )&( roiResolution.x ), sizeof( double ) );
    stream.read( ( char* )&( roiResolution.y ), sizeof( double ) );
    stream.read( ( char* )&( roiResolution.z ), sizeof( double ) );

    // receving the spin glass density
    float spinGlassDensity;
    stream.read( ( char* )&spinGlassDensity, sizeof( float ) );

    // receving the spin glass length
    float spinGlassLength;
    stream.read( ( char* )&spinGlassLength, sizeof( float ) );

    // receving the connection likelihood
    float connectionLikelihood;
    stream.read( ( char* )&connectionLikelihood, sizeof( float ) );

    if ( globalTractographyService )
    {

      spinGlassAndConnectionMap->setAnatomicalPriors( roiSize,
                                                      roiResolution,
                                                      spinGlassDensity,
                                                      spinGlassLength,
                                                      connectionLikelihood );

    }

    ///////// display //////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "-------------- Anatomical Priors Callback -----------------"
                << std::endl;
      std::cout << "  roiSize : " << roiSize << std::endl;
      std::cout << "  roiResolution : " << roiResolution << std::endl;
      std::cout << "  spinGlassDensity : " << spinGlassDensity << std::endl;
      std::cout << "  spinGlassLength : " << spinGlassLength << std::endl;
      std::cout << "  connectionLikelihood : " << connectionLikelihood
                << std::endl;

    }

  }
  GKG_CATCH( "void gkg::AnatomicalPriorsCallback::receive( "
             "gkg::SocketStream& stream )" );

}
