#include <gkg-dmri-microstructure-noddi/NoddiMCMCFunction.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-core-exception/Exception.h>


gkg::NoddiMCMCFunction::NoddiMCMCFunction( 
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

    // computing lookup table
    int32_t j = 0;

    std::vector< gkg::Vector3d< float > >::const_iterator
      io = _inputOrientations.begin(),
      ioe = _inputOrientations.end();
    std::vector< gkg::Vector3d< float > >::const_iterator
      oo, ooe = _outputOrientations.end();
    std::vector< float >::const_iterator
      b = _bValues.begin();

    _minusBiTimesSquareOfDiDotDo.resize( _inputOrientationCount *
                                         _outputOrientationCount );
					 
    while ( io != ioe )
    {

      oo = _outputOrientations.begin();

      while ( oo != ooe )
      {
      
        float dotProd = io->x * oo->x +  // ix*ox
	                io->y * oo->y +  // iy*oy
			io->z * oo->z;   // iz*oz
	_minusBiTimesSquareOfDiDotDo[ j ] = -dotProd * dotProd * *b;

        ++oo;
        j++;
      
      }

      ++io;
      ++b;

    }

  }
  GKG_CATCH( "gkg::NoddiMCMCFunction::NoddiMCMCFunction( "
             "gkg::RCPointer< gkg::Volume< float > > dw, "
             "const gkg::OrientationSet* outputOrientationSet, "
             "const gkg::OrientationSet* samplerOrientationSet )" );

}


gkg::NoddiMCMCFunction::~NoddiMCMCFunction()
{
}
