#include <gkg-processing-mask/AutomaticThresholdMask.h>
#include <gkg-processing-algobase/StatisticAnalyzer_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-morphology/HoleFilling2d.h>
#include <gkg-core-exception/Exception.h>


template < class IN, class OUT >
gkg::AutomaticThresholdMask< IN, OUT >::AutomaticThresholdMask( double noisePosition,
                                                      double noiseRatio,
                                                      int32_t thrown,
                                                      int32_t kept,
                                                      const OUT& foreground,
                                                      const OUT& background )
                                  : gkg::MaskAlgorithm< IN, OUT >(),
                                    _noisePosition( noisePosition ),
                                    _noiseRatio( noiseRatio ),
                                    _thrown( thrown ),
                                    _kept( kept ),
                                    _foreground( foreground ),
                                    _background( background )
{
}


template < class IN, class OUT >
gkg::AutomaticThresholdMask< IN, OUT >::~AutomaticThresholdMask()
{
}


template < class IN, class OUT >
std::string gkg::AutomaticThresholdMask< IN, OUT >::getName() const
{

  return getStaticName();

}


template < class IN, class OUT >
void gkg::AutomaticThresholdMask< IN, OUT >::getMask(
                                                const gkg::Volume< IN >& volume,
                                                gkg::Volume< OUT >& mask,
                                                bool /*verbose*/ ) const
{

  try
  {

    gkg::StatisticAnalyzer< gkg::Volume< IN > > statisticAnalyzer;
    gkg::GreaterOrEqualToFunction< IN >
      greaterOrEqualToFunction( statisticAnalyzer.getAutomaticThreshold(
                                                                 volume,
                                                                 _noisePosition,
                                                                 _noiseRatio,
                                                                 _thrown,
                                                                 _kept ) );
    gkg::Binarizer< gkg::Volume< IN >, gkg::Volume< OUT > >
      binarizer( greaterOrEqualToFunction, _foreground, _background );

    binarizer.binarize( volume, mask );

    gkg::HoleFilling2d< OUT > holeFilling2d( _foreground, _background );
    holeFilling2d.fill( mask, mask );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::AutomaticThresholdMask< IN, OUT >::getMask( "
             "const gkg::Volume< IN >& volume, "
             "gkg::Volume< OUT >& mask, "
             "bool verbose ) const" );

}


template < class IN, class OUT >
std::string gkg::AutomaticThresholdMask< IN, OUT >::getStaticName()
{

  return gkg::TypeOf< IN >::getName() + " -> " +
         gkg::TypeOf< OUT >::getName() +
         " automatic threshold mask algorithm";

}


// force instanciation for standard types

template class gkg::AutomaticThresholdMask< int8_t, uint8_t >;
template class gkg::AutomaticThresholdMask< int8_t, int16_t >;
template class gkg::AutomaticThresholdMask< int8_t, float >;
template class gkg::AutomaticThresholdMask< int8_t, double >;

template class gkg::AutomaticThresholdMask< uint8_t, uint8_t >;
template class gkg::AutomaticThresholdMask< uint8_t, int16_t >;
template class gkg::AutomaticThresholdMask< uint8_t, float >;
template class gkg::AutomaticThresholdMask< uint8_t, double >;

template class gkg::AutomaticThresholdMask< int16_t, uint8_t >;
template class gkg::AutomaticThresholdMask< int16_t, int16_t >;
template class gkg::AutomaticThresholdMask< int16_t, float >;
template class gkg::AutomaticThresholdMask< int16_t, double >;

template class gkg::AutomaticThresholdMask< uint16_t, uint8_t >;
template class gkg::AutomaticThresholdMask< uint16_t, int16_t >;
template class gkg::AutomaticThresholdMask< uint16_t, float >;
template class gkg::AutomaticThresholdMask< uint16_t, double >;

template class gkg::AutomaticThresholdMask< int32_t, uint8_t >;
template class gkg::AutomaticThresholdMask< int32_t, int16_t >;
template class gkg::AutomaticThresholdMask< int32_t, float >;
template class gkg::AutomaticThresholdMask< int32_t, double >;

template class gkg::AutomaticThresholdMask< uint32_t, uint8_t >;
template class gkg::AutomaticThresholdMask< uint32_t, int16_t >;
template class gkg::AutomaticThresholdMask< uint32_t, float >;
template class gkg::AutomaticThresholdMask< uint32_t, double >;

template class gkg::AutomaticThresholdMask< int64_t, uint8_t >;
template class gkg::AutomaticThresholdMask< int64_t, int16_t >;
template class gkg::AutomaticThresholdMask< int64_t, float >;
template class gkg::AutomaticThresholdMask< int64_t, double >;

template class gkg::AutomaticThresholdMask< uint64_t, uint8_t >;
template class gkg::AutomaticThresholdMask< uint64_t, int16_t >;
template class gkg::AutomaticThresholdMask< uint64_t, float >;
template class gkg::AutomaticThresholdMask< uint64_t, double >;

template class gkg::AutomaticThresholdMask< float, uint8_t >;
template class gkg::AutomaticThresholdMask< float, int16_t >;
template class gkg::AutomaticThresholdMask< float, float >;
template class gkg::AutomaticThresholdMask< float, double >;

template class gkg::AutomaticThresholdMask< double, uint8_t >;
template class gkg::AutomaticThresholdMask< double, int16_t >;
template class gkg::AutomaticThresholdMask< double, float >;
template class gkg::AutomaticThresholdMask< double, double >;

