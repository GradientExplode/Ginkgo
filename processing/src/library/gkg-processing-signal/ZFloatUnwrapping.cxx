#include <gkg-processing-signal/ZFloatUnwrapping.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-numericalanalysis/NelderMeadSimplexFunction.h>
#include <gkg-processing-numericalanalysis/NumericalAnalysisSelector.h>
#include <gkg-core-exception/Exception.h>


class ZFloatUnwrappingFunction : public gkg::NelderMeadSimplexFunction
{

  public:

    ZFloatUnwrappingFunction( const gkg::Volume< float >& phaseMap,
                              const gkg::Volume< float >& qualityMap,
                              const gkg::Volume< int16_t > mask,
                              int32_t referenceSliceIndex,
                              int32_t shiftedSliceIndex );
    ~ZFloatUnwrappingFunction();


    double getValueAt( const gkg::Vector& parameters ) const;

  protected:

    const gkg::Volume< float >& _phaseMap;
    const gkg::Volume< float >& _qualityMap;
    const gkg::Volume< int16_t > _mask;
    int32_t _referenceSliceIndex;
    int32_t _shiftedSliceIndex;

};



ZFloatUnwrappingFunction::ZFloatUnwrappingFunction(
                                         const gkg::Volume< float >& phaseMap,
                                         const gkg::Volume< float >& qualityMap,
                                         const gkg::Volume< int16_t > mask,
                                         int32_t referenceSliceIndex,
                                         int32_t shiftedSliceIndex )
                         : gkg::NelderMeadSimplexFunction( 1 ),
                           _phaseMap( phaseMap ),
                           _qualityMap( qualityMap ),
                           _mask( mask ),
                           _referenceSliceIndex( referenceSliceIndex ),
                           _shiftedSliceIndex( shiftedSliceIndex )
{
}


ZFloatUnwrappingFunction::~ZFloatUnwrappingFunction()
{
}


double 
ZFloatUnwrappingFunction::getValueAt( const gkg::Vector& parameters ) const
{

  try
  {

    int32_t sizeX = _phaseMap.getSizeX();
    int32_t lineCount = _phaseMap.getSizeY();

    double difference, distance = 0.0;

    int32_t x, line;
    for ( line = 0; line < lineCount; line++ )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        difference = ( ( double )_phaseMap( x, line, _referenceSliceIndex ) -
                       ( ( double )_phaseMap( x, line, _shiftedSliceIndex ) +
                         parameters( 0 ) ) )/* /
                     ( ( double )_phaseMap( x, line, _referenceSliceIndex ) +
                       ( ( double )_phaseMap( x, line, _shiftedSliceIndex ) +
                         parameters( 0 ) ) )*/;
        if ( _mask( x, line, _referenceSliceIndex ) &&
             _mask( x, line, _shiftedSliceIndex ) )
        {

          distance += /*std::min(
                        ( double )_qualityMap( x, line, _referenceSliceIndex ),
                        ( double )_qualityMap( x, line, _shiftedSliceIndex ) ) **/
                      difference * difference;

        }

      }

    }
    return distance;

  }
  GKG_CATCH( "double "
             "ZFloatUnwrappingFunction::getValueAt( "
             "const gkg::Vector& parameters ) const" );


}



static void applyShift( gkg::Volume< float >& phaseMap,
                        int32_t shiftedSlice,
                        float shift )
{

  try
  {

    int32_t sizeX = phaseMap.getSizeX();
    int32_t lineCount = phaseMap.getSizeY();

    int32_t x, line;
    for ( line = 0; line < lineCount; line++ )
    {

      for ( x = 0; x < sizeX; x++ )
      {

        phaseMap( x, line, shiftedSlice ) += shift;

      }

    }

  }
  GKG_CATCH( "static void applyShift( gkg::Volume< float >& phaseMap, "
             "int32_t shiftedSlice, "
             "float shift )" );

}


void gkg::ZFloatUnwrapping( gkg::Volume< float >& phaseMap,
                            const gkg::Volume< float >& qualityMap,
                            const gkg::Volume< int16_t >& mask,
                            int32_t referenceSlice )
{

  try
  {


    gkg::NumericalAnalysisImplementationFactory* factory =
    gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();



    int32_t sliceCount = phaseMap.getSizeZ();
    int32_t slice;

    // unwrapping from reference to top of volume
    for ( slice = referenceSlice + 1; slice < sliceCount; slice++ )
    {

      ZFloatUnwrappingFunction
        zFloatUnwrappingFunction( phaseMap,
                                  qualityMap,
                                  mask,
                                  slice - 1,
                                  slice );

      gkg::Vector parameters( 1 );
      gkg::Vector deltaParameters( 1 );
      parameters( 0 ) = 0.0;
      deltaParameters( 0 ) = 10 * M_PI;
      double testSize = 0.0;
      int32_t maximumIterationCount = 1000;
      double maximumTestSize = 1e-7;
      factory->getNelderMeadSimplexOptimumParameters(
                                             parameters,
                                             deltaParameters,
                                             testSize,
                                             zFloatUnwrappingFunction,
                                             maximumIterationCount,
                                             maximumTestSize,
                                             true );

      applyShift( phaseMap, slice, ( float )parameters( 0 ) );

    }


    // unwrapping from reference to bottom of volume
    for ( slice = referenceSlice - 1; slice >= 0; slice-- )
    {

      ZFloatUnwrappingFunction
        zFloatUnwrappingFunction( phaseMap,
                                  qualityMap,
                                  mask,
                                  slice + 1,
                                  slice );

      gkg::Vector parameters( 1 );
      gkg::Vector deltaParameters( 1 );
      parameters( 0 ) = 0.0;
      deltaParameters( 0 ) = 10 * M_PI;
      double testSize = 0.0;
      int32_t maximumIterationCount = 1000;
      double maximumTestSize = 1e-7;
      factory->getNelderMeadSimplexOptimumParameters(
                                             parameters,
                                             deltaParameters,
                                             testSize,
                                             zFloatUnwrappingFunction,
                                             maximumIterationCount,
                                             maximumTestSize,
                                             true );

      applyShift( phaseMap, slice, ( float )parameters( 0 ) );

    }

  }
  GKG_CATCH( "void gkg::ZFloatUnwrapping( gkg::Volume< float >& phaseMap, "
             "const gkg::Volume< float >& qualityMap, "
             "const gkg::Volume< int16_t >& mask, "
             "int32_t referenceSlice )" );

}
