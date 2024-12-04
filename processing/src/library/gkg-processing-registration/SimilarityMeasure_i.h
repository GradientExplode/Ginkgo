#ifndef _gkg_processing_registration_SimilarityMeasure_i_h_
#define _gkg_processing_registration_SimilarityMeasure_i_h_


#include <gkg-processing-registration/SimilarityMeasure.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-histogram/HistogramAnalyzer.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-processing-algobase/Binarizer_i.h>
#include <gkg-processing-algobase/GreaterOrEqualToFunction_i.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-processing-transform/IdentityTransform3d.h>
#include <gkg-core-exception/Exception.h>
#include <iostream>


template < class T1, class T2 >
inline
gkg::SimilarityMeasure< T1, T2 >::SimilarityMeasure(
           gkg::RCPointer< gkg::Volume< T1 > > reference,
           gkg::RCPointer< gkg::Volume< T2 > > floating,
           const T1& referenceLowerThreshold,
           const T2& floatingLowerThreshold,
           int32_t resamplingOrder,
           int32_t subSamplingMaximumSize,
           bool verbose )
                                 : _reference( reference ),
                                   _floating( floating ),
                                   _referenceLowerThreshold(
                                                      referenceLowerThreshold ),
                                   _floatingLowerThreshold(
                                                       floatingLowerThreshold ),
                                   _resamplingOrder( resamplingOrder ),
                                   _subSamplingMaximumSize(
                                                       subSamplingMaximumSize ),
                                   _verbose( verbose )
{

  try
  {

    ////////////////////////////////////////////////////////////////////////////
    // performing histogram analysis on reference
    ////////////////////////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "histogram analysis on reference : " << std::flush;

    }

    T1 referenceLowerBoundary;
    T1 referenceUpperBoundary;
    gkg::HistogramAnalyzer::getInstance().getPercentileUpperBoundaries(
                                                       *reference,
                                                       1024,
                                                       100.0,
                                                       referenceLowerBoundary,
                                                       referenceUpperBoundary );
    if ( _verbose )
    {

      std::cout << referenceLowerBoundary << "->"
                << referenceUpperBoundary << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // performing histogram analysis on floating
    ////////////////////////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "histogram analysis on floating : " << std::flush;

    }
    T2 floatingLowerBoundary;
    T2 floatingUpperBoundary;
    gkg::HistogramAnalyzer::getInstance().getPercentileUpperBoundaries(
                                                        *floating,
                                                        1024,
                                                        100.0,
                                                        floatingLowerBoundary,
                                                        floatingUpperBoundary );
    if ( _verbose )
    {

      std::cout << floatingLowerBoundary << "->"
                << floatingUpperBoundary << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // converting reference to uint8_t type
    ////////////////////////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "converting reference to uint8_t : " << std::flush;

    }
    gkg::Volume< uint8_t >* 
      tmpReferenceU8 = new gkg::Volume< uint8_t >;
    gkg::Rescaler< gkg::Volume< T1 >, gkg::Volume< uint8_t > >
      referenceRescaler( std::max( referenceLowerBoundary,
                                   _referenceLowerThreshold ),
                         referenceUpperBoundary );
    referenceRescaler.rescale( *reference, *tmpReferenceU8 );
    if ( _verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // converting floating to uint8_t type
    ////////////////////////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "converting floating to uint8_t : " << std::flush;

    }
    gkg::Volume< uint8_t >*
      tmpFloatingU8 = new gkg::Volume< uint8_t >;
    gkg::Rescaler< gkg::Volume< T2 >, gkg::Volume< uint8_t > >
      floatingRescaler( std::max( floatingLowerBoundary,
                                  _floatingLowerThreshold ),
                        floatingUpperBoundary );
    floatingRescaler.rescale( *floating, *tmpFloatingU8 );
    if ( _verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building temporary reference mask
    ////////////////////////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "building temporary reference mask : " << std::flush;

    }
    gkg::Volume< uint8_t >* tmpReferenceMask = new gkg::Volume< uint8_t >;
    gkg::GreaterOrEqualToFunction< T1 > 
      referenceGreaterOrEqualToFunction( referenceLowerThreshold );
    gkg::Binarizer< gkg::Volume< T1 >, gkg::Volume< uint8_t > >
      referenceBinarizer( referenceGreaterOrEqualToFunction, 1, 0 );
    referenceBinarizer.binarize( *reference, *tmpReferenceMask );
    if ( _verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // building temporary floating mask
    ////////////////////////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "building temporary floating mask : " << std::flush;

    }
    gkg::Volume< uint8_t >* tmpFloatingMask = new gkg::Volume< uint8_t >;
    gkg::GreaterOrEqualToFunction< T2 > 
      floatingGreaterOrEqualToFunction( floatingLowerThreshold );
    gkg::Binarizer< gkg::Volume< T2 >, gkg::Volume< uint8_t > >
      floatingBinarizer( floatingGreaterOrEqualToFunction, 1, 0 );
    floatingBinarizer.binarize( *floating, *tmpFloatingMask );
    if ( _verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // determing sub-sampling size and resolution of reference
    ////////////////////////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "determining sub-sampling reference size and resolution : "
                << std::flush;

    }
    gkg::Vector3d< int32_t > referenceOldSize;
    gkg::Vector3d< double > referenceOldResolution;

    tmpReferenceU8->getSize( referenceOldSize );
    tmpReferenceU8->getResolution( referenceOldResolution );

    gkg::Vector3d< int32_t > referenceNewSize = referenceOldSize;
    gkg::Vector3d< double > referenceNewResolution = referenceOldResolution;

    if ( referenceOldSize.x > _subSamplingMaximumSize )
    {

      double ratio = ( double )referenceOldSize.x /
                    ( double )_subSamplingMaximumSize;
      referenceNewSize.x = _subSamplingMaximumSize;
      referenceNewResolution.x *= ratio;

    }    
    if ( referenceOldSize.y  > _subSamplingMaximumSize )
    {

      double ratio = ( double )referenceOldSize.y  /
                     ( double )_subSamplingMaximumSize;
      referenceNewSize.y = _subSamplingMaximumSize;
      referenceNewResolution.y *= ratio;

    }    
    if ( referenceOldSize.z > _subSamplingMaximumSize )
    {

      double ratio = ( double )referenceOldSize.z /
                     ( double )_subSamplingMaximumSize;
      referenceNewSize.z = _subSamplingMaximumSize;
      referenceNewResolution.z *= ratio;

    }
    if ( _verbose )
    {

      std::cout << referenceOldSize << "->"
                << referenceNewSize << " / "
                << referenceOldResolution << "->"
                << referenceNewResolution << " / " 
                << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // determing sub-sampling size and resolution of floating
    ////////////////////////////////////////////////////////////////////////////
    if ( _verbose )
    {

      std::cout << "determining sub-sampling floating size and resolution : "
                << std::flush;

    }
    gkg::Vector3d< int32_t > floatingOldSize;
    gkg::Vector3d< double > floatingOldResolution;

    tmpFloatingU8->getSize( floatingOldSize );
    tmpFloatingU8->getResolution( floatingOldResolution );

    gkg::Vector3d< int32_t > floatingNewSize = floatingOldSize;
    gkg::Vector3d< double > floatingNewResolution = floatingOldResolution;

    if ( floatingOldSize.x > _subSamplingMaximumSize )
    {

      double ratio = ( double )floatingOldSize.x /
                    ( double )_subSamplingMaximumSize;
      floatingNewSize.x = _subSamplingMaximumSize;
      floatingNewResolution.x *= ratio;

    }    
    if ( floatingOldSize.y  > _subSamplingMaximumSize )
    {

      double ratio = ( double )floatingOldSize.y  /
                     ( double )_subSamplingMaximumSize;
      floatingNewSize.y = _subSamplingMaximumSize;
      floatingNewResolution.y *= ratio;

    }    
    if ( floatingOldSize.z > _subSamplingMaximumSize )
    {

      double ratio = ( double )floatingOldSize.z /
                     ( double )_subSamplingMaximumSize;
      floatingNewSize.z = _subSamplingMaximumSize;
      floatingNewResolution.z *= ratio;

    }
    if ( _verbose )
    {

      std::cout << floatingOldSize << "->"
                << floatingNewSize << " / "
                << floatingOldResolution << "->"
                << floatingNewResolution << " / " 
                << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // undersampling reference to sub-sampling maximum size
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "resampling reference : " << std::flush;

    }
    gkg::Resampler< uint8_t >* resampler = 
      gkg::ResamplerFactory< uint8_t >::getInstance().getResampler(
                                                             _resamplingOrder );
    _resampledReferenceU8.reset(
                             new gkg::Volume< uint8_t >( referenceNewSize.x,
                                                         referenceNewSize.y,
                                                         referenceNewSize.z ) );
    _resampledReferenceU8->getHeader().addAttribute( "resolutionX",
                                                     referenceNewResolution.x );
    _resampledReferenceU8->getHeader().addAttribute( "resolutionY",
                                                     referenceNewResolution.y );
    _resampledReferenceU8->getHeader().addAttribute( "resolutionZ",
                                                     referenceNewResolution.z );
    gkg::IdentityTransform3d< double > identityTransform3d;
    resampler->resample( *tmpReferenceU8,
                         identityTransform3d,
                         0U,
                         *_resampledReferenceU8,
                         _verbose );
    delete tmpReferenceU8;
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // undersampling floating to sub-sampling maximum size
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "resampling floating : " << std::flush;

    }
    _resampledFloatingU8.reset(
                              new gkg::Volume< uint8_t >( floatingNewSize.x,
                                                          floatingNewSize.y,
                                                          floatingNewSize.z ) );
    _resampledFloatingU8->getHeader().addAttribute( "resolutionX",
                                                    floatingNewResolution.x );
    _resampledFloatingU8->getHeader().addAttribute( "resolutionY",
                                                    floatingNewResolution.y );
    _resampledFloatingU8->getHeader().addAttribute( "resolutionZ",
                                                    floatingNewResolution.z );
    resampler->resample( *tmpFloatingU8,
                         identityTransform3d,
                         0U,
                         *_resampledFloatingU8,
                         _verbose );
    delete tmpFloatingU8;
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // undersampling reference mask to sub-sampling maximum size
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "resampling reference mask : " << std::flush;

    }
    resampler = 
      gkg::ResamplerFactory< uint8_t >::getInstance().getResampler( 0 );
    _resampledReferenceMask.reset(
                             new gkg::Volume< uint8_t >( referenceNewSize.x,
                                                         referenceNewSize.y,
                                                         referenceNewSize.z ) );
    _resampledReferenceMask->getHeader().addAttribute(
                                                     "resolutionX",
                                                     referenceNewResolution.x );
    _resampledReferenceMask->getHeader().addAttribute(
                                                     "resolutionY",
                                                     referenceNewResolution.y );
    _resampledReferenceMask->getHeader().addAttribute(
                                                     "resolutionZ",
                                                     referenceNewResolution.z );
    resampler->resample( *tmpReferenceMask,
                         identityTransform3d,
                         0U,
                         *_resampledReferenceMask,
                         _verbose );
    delete tmpReferenceMask;
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    ////////////////////////////////////////////////////////////////////////////
    // undersampling floating mask to sub-sampling maximum size
    ////////////////////////////////////////////////////////////////////////////
    if ( verbose )
    {

      std::cout << "resampling floating mask : " << std::flush;

    }
    _resampledFloatingMask.reset(
                              new gkg::Volume< uint8_t >( floatingNewSize.x,
                                                          floatingNewSize.y,
                                                          floatingNewSize.z ) );
    _resampledFloatingMask->getHeader().addAttribute( "resolutionX",
                                                      floatingNewResolution.x );
    _resampledFloatingMask->getHeader().addAttribute( "resolutionY",
                                                      floatingNewResolution.y );
    _resampledFloatingMask->getHeader().addAttribute( "resolutionZ",
                                                      floatingNewResolution.z );
    resampler->resample( *tmpFloatingMask,
                         identityTransform3d,
                         0U,
                         *_resampledFloatingMask,
                         _verbose );
    delete tmpFloatingMask;
    if ( verbose )
    {

      std::cout << "done" << std::endl;

    }

    _resampler =
      gkg::ResamplerFactory< uint8_t >::getInstance().getResampler( 1 );
    _transformedFloatingU8.reset( new gkg::Volume< uint8_t >(
                                   this->_resampledReferenceU8->getSizeX(),
                                   this->_resampledReferenceU8->getSizeY(),
                                   this->_resampledReferenceU8->getSizeZ() ) );
    gkg::Vector3d< double > resampledReferenceResolutionU8;
    this->_resampledReferenceU8->getResolution(
                                              resampledReferenceResolutionU8 );
    _transformedFloatingU8->getHeader().addAttribute(
                                           "resolutionX",
                                           resampledReferenceResolutionU8.x );
    _transformedFloatingU8->getHeader().addAttribute(
                                           "resolutionY",
                                           resampledReferenceResolutionU8.y );
    _transformedFloatingU8->getHeader().addAttribute(
                                           "resolutionZ",
                                           resampledReferenceResolutionU8.z );

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "gkg::SimilarityMeasure< T1, T2 >::SimilarityMeasure( "
             "gkg::RCPointer< gkg::Volume< T1 > > reference, "
             "gkg::RCPointer< gkg::Volume< T2 > > floating, "
             "gkg::RCPointer<gkg:: ParameterizedTransform3d< float > > "
             "transform3d, "
             "int32_t resamplingOrder, "
             "int32_t subSamplingMaximumSize )" );

}


template < class T1, class T2 >
inline
gkg::SimilarityMeasure< T1, T2 >::~SimilarityMeasure()
{
}


template < class T1, class T2 >
inline
gkg::RCPointer< gkg::Volume< T1 > > 
gkg::SimilarityMeasure< T1, T2 >::getReference() const
{

  return _reference;

}


template < class T1, class T2 >
inline
gkg::RCPointer< gkg::Volume< T2 > > 
gkg::SimilarityMeasure< T1, T2 >::getFloating() const
{

  return _floating;

}


template < class T1, class T2 >
inline
const T1& gkg::SimilarityMeasure< T1, T2 >::getReferenceLowerThreshold() const
{

  return _referenceLowerThreshold;

}


template < class T1, class T2 >
inline
const T2& gkg::SimilarityMeasure< T1, T2 >::getFloatingLowerThreshold() const
{

  return _floatingLowerThreshold;

}


template < class T1, class T2 >
inline
int32_t gkg::SimilarityMeasure< T1, T2 >::getResamplingOrder() const
{

  return _resamplingOrder;

}


template < class T1, class T2 >
inline
int32_t gkg::SimilarityMeasure< T1, T2 >::getSubSamplingMaximumSize() const
{

  return _subSamplingMaximumSize;

}


template < class T1, class T2 >
inline
gkg::RCPointer< gkg::Volume< uint8_t > > 
gkg::SimilarityMeasure< T1, T2 >::getResampledReferenceU8() const
{

  return _resampledReferenceU8;

}


template < class T1, class T2 >
inline
gkg::RCPointer< gkg::Volume< uint8_t > > 
gkg::SimilarityMeasure< T1, T2 >::getResampledFloatingU8() const
{

  return _resampledFloatingU8;

}


template < class T1, class T2 >
inline
gkg::RCPointer< gkg::Volume< uint8_t > > 
gkg::SimilarityMeasure< T1, T2 >::getResampledReferenceMask() const
{

  return _resampledReferenceMask;

}


template < class T1, class T2 >
inline
gkg::RCPointer< gkg::Volume< uint8_t > > 
gkg::SimilarityMeasure< T1, T2 >::getResampledFloatingMask() const
{

  return _resampledFloatingMask;

}


template < class T1, class T2 >
inline
gkg::RCPointer< gkg::Volume< uint8_t > > 
gkg::SimilarityMeasure< T1, T2 >::getTransformedFloatingU8() const
{

  return _transformedFloatingU8;

}


template < class T1, class T2 >
inline
void
gkg::SimilarityMeasure< T1, T2 >::computeTransformedFloatingU8(
                           const gkg::Transform3d< float >& transform3d,
                           gkg::Volume< uint8_t >& transformedFloatingU8 ) const
{

  try
  {

    // resampling and transforming accordingly the floating volume
    this->_resampler->resample( *this->_resampledFloatingU8,
                                transform3d,
                                0U,
                                transformedFloatingU8,
                                false );

  }
  GKG_CATCH( "template < class T1, class T2 > "
             "inline "
             "void "
             "gkg::SimilarityMeasure< T1, T2 >::computeTransformedFloatingU8( "
             "const gkg::Transform3d< float >& transform3d, "
             "gkg::Volume< uint8_t >& transformedFloatingU8 ) const" );

}


template < class T1, class T2 >
inline
bool
gkg::SimilarityMeasure< T1, T2 >::hasDerivative() const
{

  return false;

}


#endif
