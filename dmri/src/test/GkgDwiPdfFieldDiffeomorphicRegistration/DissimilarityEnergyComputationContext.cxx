#include <gkg-communication-thread/LoopContext.h>
#include "ProbabilityDensityFunction.h"
#include <gkg-dmri-container/PdfCartesianField.h>
#include <gkg-processing-transform/VectorFieldTransform3d.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "DissimilarityEnergyComputationContext.h"



gkg::DissimilarityEnergyComputationContext::
           DissimilarityEnergyComputationContext( 
                gkg::PdfCartesianField& reference,
                gkg::PdfCartesianField& floating,
                std::vector< bool >& referenceHasSiteBoolVector,
                std::vector< gkg::Vector3d< int32_t > >& floatingSites,
                float& dissimilarityEnergy,
                int32_t& size )
                     : gkg::LoopContext< int32_t >(),
                      _reference( reference ),
                      _floating( floating ),
                      _referenceHasSiteBoolVector( referenceHasSiteBoolVector ),
                      _floatingSites( floatingSites ),
                      _dissimilarityEnergy( dissimilarityEnergy ),
                      _size( size )
                 
{
}

void gkg::DissimilarityEnergyComputationContext::doIt( 
                                            int32_t startIndex, int32_t count )
{

  try
  {

    int32_t stopIndex = startIndex + count;

    //double current = 0.0;
    //double K = 0.0;
    double dissimilarityEnergy = 0.0;
    double tmpDissimilarity = 0.0;

    int32_t o = 0;
    for( o = startIndex; o < stopIndex; ++o  )
    {

      const gkg::Vector3d< int32_t >& site = _floatingSites[ o ];
  
      if( _referenceHasSiteBoolVector[ o ] )
      {

        tmpDissimilarity = _reference.getItemFast( site )->getDissimilarity(
                                *( _floating.getItemFast( site ) ),
                                0.0f, 100.0f );  
     
        if ( !std::isnan( tmpDissimilarity ) )
        {

          dissimilarityEnergy += tmpDissimilarity;

        } 
                                    
      }

    }

    lock();

    _dissimilarityEnergy += ( float )dissimilarityEnergy / ( float )_size;

    unlock();

  }
  GKG_CATCH( "void gkg::DissimilarityEnergyComputationContext::doIt(" 
             "int32_t startIndex, int32_t count )" );

}



