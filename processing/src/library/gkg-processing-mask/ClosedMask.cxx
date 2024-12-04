#include <gkg-processing-mask/ClosedMask.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/StatisticAnalyzer_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-algobase/EqualToFunction_i.h>
#include <gkg-processing-morphology/Erosion.h>
#include <gkg-processing-morphology/Dilation.h>
#include <gkg-processing-morphology/Closing.h>
#include <gkg-processing-morphology/ConnectedComponents.h>
#include <gkg-core-exception/Exception.h>


template < class IN, class OUT >
gkg::ClosedMask< IN, OUT >::ClosedMask( float radius,
                                        float thresholdRatio,
                                        const OUT& foreground,
                                        const OUT& background )
                           : gkg::MaskAlgorithm< IN, OUT >(),
                             _radius( radius ),
                             _thresholdRatio( thresholdRatio ),
                             _foreground( foreground ),
                             _background( background )
{
}


template < class IN, class OUT >
gkg::ClosedMask< IN, OUT >::~ClosedMask()
{
}


template < class IN, class OUT >
std::string gkg::ClosedMask< IN, OUT >::getName() const
{

  return getStaticName();

}


template < class IN, class OUT >
void gkg::ClosedMask< IN, OUT >::getMask( const gkg::Volume< IN >& volume,
                                          gkg::Volume< OUT >& mask,
                                          bool verbose ) const
{

  try
  {

    // automatically thresholding
    gkg::StatisticAnalyzer< gkg::Volume< IN > > statisticAnalyzer;
    IN minimum, maximum;

    statisticAnalyzer.getMinMax( volume, minimum, maximum );

    IN threshold = minimum +
                   ( IN )( ( double )( maximum - minimum ) * _thresholdRatio );

    // morphological erosion
    gkg::GreaterOrEqualToFunction< IN > greaterOrEqualToFunction( threshold );
    gkg::Volume< uint8_t > maskU8;
    gkg::Erosion< IN, uint8_t > erosion( _radius,
                                         greaterOrEqualToFunction,
                                         1U,
                                         0U );
    erosion.erode( volume, maskU8, verbose );

    // getting greatest connected component
    gkg::EqualToFunction< uint8_t > equalToFunction( 1U );
    gkg::ConnectedComponents< uint8_t, uint8_t >
      connectedComponents( equalToFunction,
                           0U,
                           1,
                           1,
                           false,
                           gkg::Neighborhood3d::Neighborhood3d_6 );
    connectedComponents.getComponents( maskU8, maskU8, 0, false );

    // morphological dilation
    gkg::Dilation< uint8_t, uint8_t > dilation( _radius,
                                                equalToFunction,
                                                1U,
                                                0U );
    dilation.dilate( maskU8, maskU8, verbose );

    // morphological closing
    gkg::Closing< uint8_t, OUT > closing( _radius,
                                          equalToFunction,
                                          _foreground,
                                          _background );
    closing.close( maskU8, mask, verbose );

  }
  GKG_CATCH( "template < class IN, class OUT > "
             "void gkg::ClosedMask< IN, OUT >::getMask( "
             "const gkg::Volume< IN >& volume, "
             "gkg::Volume< OUT >& mask, "
             "bool verbose ) const" );

}


template < class IN, class OUT >
std::string gkg::ClosedMask< IN, OUT >::getStaticName()
{

  return gkg::TypeOf< IN >::getName() + " -> " +
         gkg::TypeOf< OUT >::getName() +
         " closed mask algorithm";

}


// force instanciation for standard types

template class gkg::ClosedMask< int8_t, uint8_t >;
template class gkg::ClosedMask< int8_t, int16_t >;
template class gkg::ClosedMask< int8_t, float >;
template class gkg::ClosedMask< int8_t, double >;

template class gkg::ClosedMask< uint8_t, uint8_t >;
template class gkg::ClosedMask< uint8_t, int16_t >;
template class gkg::ClosedMask< uint8_t, float >;
template class gkg::ClosedMask< uint8_t, double >;

template class gkg::ClosedMask< int16_t, uint8_t >;
template class gkg::ClosedMask< int16_t, int16_t >;
template class gkg::ClosedMask< int16_t, float >;
template class gkg::ClosedMask< int16_t, double >;

template class gkg::ClosedMask< uint16_t, uint8_t >;
template class gkg::ClosedMask< uint16_t, int16_t >;
template class gkg::ClosedMask< uint16_t, float >;
template class gkg::ClosedMask< uint16_t, double >;

template class gkg::ClosedMask< int32_t, uint8_t >;
template class gkg::ClosedMask< int32_t, int16_t >;
template class gkg::ClosedMask< int32_t, float >;
template class gkg::ClosedMask< int32_t, double >;

template class gkg::ClosedMask< uint32_t, uint8_t >;
template class gkg::ClosedMask< uint32_t, int16_t >;
template class gkg::ClosedMask< uint32_t, float >;
template class gkg::ClosedMask< uint32_t, double >;

template class gkg::ClosedMask< int64_t, uint8_t >;
template class gkg::ClosedMask< int64_t, int16_t >;
template class gkg::ClosedMask< int64_t, float >;
template class gkg::ClosedMask< int64_t, double >;

template class gkg::ClosedMask< uint64_t, uint8_t >;
template class gkg::ClosedMask< uint64_t, int16_t >;
template class gkg::ClosedMask< uint64_t, float >;
template class gkg::ClosedMask< uint64_t, double >;

template class gkg::ClosedMask< float, uint8_t >;
template class gkg::ClosedMask< float, int16_t >;
template class gkg::ClosedMask< float, float >;
template class gkg::ClosedMask< float, double >;

template class gkg::ClosedMask< double, uint8_t >;
template class gkg::ClosedMask< double, int16_t >;
template class gkg::ClosedMask< double, float >;
template class gkg::ClosedMask< double, double >;

