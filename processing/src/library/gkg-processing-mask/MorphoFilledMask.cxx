#include <gkg-processing-mask/MorphoFilledMask.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/StatisticAnalyzer_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-morphology/Closing.h>
#include <gkg-processing-morphology/Dilation.h>
#include <gkg-processing-morphology/ConnectedComponents.h>
#include <gkg-core-exception/Exception.h>


template < class IN, class OUT >
gkg::MorphoFilledMask< IN, OUT >::MorphoFilledMask( float closingRadius,
                                                    double noisePosition,
                                                    double noiseRatio,
                                                    int32_t thrown,
                                                    int32_t kept,
                                                    const OUT& foreground,
                                                    const OUT& background )
                                 : gkg::MaskAlgorithm< IN, OUT >(),
                                   _closingRadius( closingRadius ),
                                   _noisePosition( noisePosition ),
                                   _noiseRatio( noiseRatio ),
                                   _thrown( thrown ),
                                   _kept( kept ),
                                   _foreground( foreground ),
                                   _background( background )
{
}


template < class IN, class OUT >
gkg::MorphoFilledMask< IN, OUT >::~MorphoFilledMask()
{
}


template < class IN, class OUT >
std::string gkg::MorphoFilledMask< IN, OUT >::getName() const
{

  return getStaticName();

}


template < class IN, class OUT >
void gkg::MorphoFilledMask< IN, OUT >::getMask( const gkg::Volume< IN >& volume,
                                                gkg::Volume< OUT >& mask,
                                                bool verbose ) const
{

  try
  {

    // automatically thresholding
    gkg::StatisticAnalyzer< gkg::Volume< IN > > statisticAnalyzer;
    IN threshold = statisticAnalyzer.getAutomaticThreshold( volume,
                                                            _noisePosition,
                                                            _noiseRatio,
                                                            _thrown,
                                                            _kept );

    // morphological closing
    gkg::GreaterOrEqualToFunction< IN > greaterOrEqualToFunction( threshold );
    gkg::Closing< IN, OUT > closing( _closingRadius,
                                     greaterOrEqualToFunction,
                                     _foreground,
                                     _background );
    closing.close( volume, mask, verbose );

    // processing background mask
    gkg::EqualToFunction< OUT > equalToFunction1( _background );
    gkg::ConnectedComponents< OUT, int32_t >
      connectedComponents1( equalToFunction1,
                            0,
                            1,
                            1,
                            false,
                            gkg::Neighborhood3d::Neighborhood3d_6 );
    gkg::Volume< int32_t > backgroundMask;
    connectedComponents1.getComponents( mask, backgroundMask, 0, false );

    // processing final mask
    gkg::EqualToFunction< int32_t > equalToFunction2( 0 );
    gkg::ConnectedComponents< int32_t, int32_t >
      connectedComponents2( equalToFunction2,
                            0,
                            1,
                            1,
                            false,
                            gkg::Neighborhood3d::Neighborhood3d_6 );
    gkg::Volume< int32_t > foregroundMask;
    connectedComponents2.getComponents( backgroundMask,
                                        foregroundMask,
                                        0,
                                        false );

    gkg::Volume< int32_t >::const_iterator f = foregroundMask.begin(),
                                           fe = foregroundMask.end();
    typename gkg::Volume< OUT >::iterator m = mask.begin();

    while ( f != fe )
    {


      *m = ( *f == 1 ? _foreground : _background );
      ++ f;
      ++ m;

    }

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::MorphoFilledMask< IN, OUT >::getMask( "
             "const gkg::Volume< IN >& volume, "
             "gkg::Volume< OUT >& mask, "
             "bool verbose ) const" );

}


template < class IN, class OUT >
std::string gkg::MorphoFilledMask< IN, OUT >::getStaticName()
{

  return gkg::TypeOf< IN >::getName() + " -> " +
         gkg::TypeOf< OUT >::getName() +
         " morphological closed single component threshold mask algorithm";

}


// force instanciation for standard types

template class gkg::MorphoFilledMask< int8_t, uint8_t >;
template class gkg::MorphoFilledMask< int8_t, int16_t >;
template class gkg::MorphoFilledMask< int8_t, float >;
template class gkg::MorphoFilledMask< int8_t, double >;

template class gkg::MorphoFilledMask< uint8_t, uint8_t >;
template class gkg::MorphoFilledMask< uint8_t, int16_t >;
template class gkg::MorphoFilledMask< uint8_t, float >;
template class gkg::MorphoFilledMask< uint8_t, double >;

template class gkg::MorphoFilledMask< int16_t, uint8_t >;
template class gkg::MorphoFilledMask< int16_t, int16_t >;
template class gkg::MorphoFilledMask< int16_t, float >;
template class gkg::MorphoFilledMask< int16_t, double >;

template class gkg::MorphoFilledMask< uint16_t, uint8_t >;
template class gkg::MorphoFilledMask< uint16_t, int16_t >;
template class gkg::MorphoFilledMask< uint16_t, float >;
template class gkg::MorphoFilledMask< uint16_t, double >;

template class gkg::MorphoFilledMask< int32_t, uint8_t >;
template class gkg::MorphoFilledMask< int32_t, int16_t >;
template class gkg::MorphoFilledMask< int32_t, float >;
template class gkg::MorphoFilledMask< int32_t, double >;

template class gkg::MorphoFilledMask< uint32_t, uint8_t >;
template class gkg::MorphoFilledMask< uint32_t, int16_t >;
template class gkg::MorphoFilledMask< uint32_t, float >;
template class gkg::MorphoFilledMask< uint32_t, double >;

template class gkg::MorphoFilledMask< int64_t, uint8_t >;
template class gkg::MorphoFilledMask< int64_t, int16_t >;
template class gkg::MorphoFilledMask< int64_t, float >;
template class gkg::MorphoFilledMask< int64_t, double >;

template class gkg::MorphoFilledMask< uint64_t, uint8_t >;
template class gkg::MorphoFilledMask< uint64_t, int16_t >;
template class gkg::MorphoFilledMask< uint64_t, float >;
template class gkg::MorphoFilledMask< uint64_t, double >;

template class gkg::MorphoFilledMask< float, uint8_t >;
template class gkg::MorphoFilledMask< float, int16_t >;
template class gkg::MorphoFilledMask< float, float >;
template class gkg::MorphoFilledMask< float, double >;

template class gkg::MorphoFilledMask< double, uint8_t >;
template class gkg::MorphoFilledMask< double, int16_t >;
template class gkg::MorphoFilledMask< double, float >;
template class gkg::MorphoFilledMask< double, double >;

