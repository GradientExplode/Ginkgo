#include <gkg-dmri-microstructure-axon-mapping/AxonMappingMCMCFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>


gkg::AxonMappingMCMCFunction::AxonMappingMCMCFunction( 
                                gkg::RCPointer< gkg::Volume< float > > dw,
                                const gkg::OrientationSet& outputOrientationSet,
                                double noiseStandardDeviation )
                             : gkg::MicrostructureMCMCFunction( 
                                                        dw,
                                                        outputOrientationSet,
                                                        noiseStandardDeviation )
{

  try
  {

    // collecting dictionary of gradient characteristics
    if ( dw->getHeader().hasAttribute( "diffusion_sensitization_parameters" ) )
    {

      if ( !dw->getHeader().hasAttribute( 
                                        "diffusion_sensitization_parameters" ) )
      {

        throw std::runtime_error(
                               "missing attribute 'gradient-characteristics'" );

      }

    }
    else
    {

      throw std::runtime_error(
                     "missing attribute 'diffusion_sensitization_parameters'" );

    }

  }
  GKG_CATCH( "gkg::AxonMappingMCMCFunction::AxonMappingMCMCFunction( "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::OrientationSet* samplerOrientationSet )" );

}


gkg::AxonMappingMCMCFunction::~AxonMappingMCMCFunction()
{
}


