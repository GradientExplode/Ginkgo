#ifndef _gkg_processing_registration_SimilarityMeasure_h_
#define _gkg_processing_registration_SimilarityMeasure_h_


#include <gkg-processing-container/Volume.h>
#include <gkg-processing-transform/Transform3d.h>
#include <gkg-processing-resampling/ResamplerFactory.h>
#include <gkg-core-pattern/RCPointer.h>


namespace gkg
{


template < class T1, class T2 >
class SimilarityMeasure
{

  public:

    virtual ~SimilarityMeasure();

    RCPointer< Volume< T1 > > getReference() const;
    RCPointer< Volume< T2 > > getFloating() const;

    const T1& getReferenceLowerThreshold() const;
    const T2& getFloatingLowerThreshold() const;
    int32_t getResamplingOrder() const;
    int32_t getSubSamplingMaximumSize() const;

    RCPointer< Volume< uint8_t > > getResampledReferenceU8() const;
    RCPointer< Volume< uint8_t > > getResampledFloatingU8() const;
    RCPointer< Volume< uint8_t > > getResampledReferenceMask() const;
    RCPointer< Volume< uint8_t > > getResampledFloatingMask() const;
    RCPointer< Volume< uint8_t > > getTransformedFloatingU8() const;


    void computeTransformedFloatingU8(
                               const Transform3d< float >& transform3d,
                               Volume< uint8_t >& transformedFloatingU8 ) const;

    virtual double getValueAt(
                            const Transform3d< float >& transform3d ) const = 0;

    virtual bool hasDerivative() const;

  protected:

    SimilarityMeasure( RCPointer< Volume< T1 > > reference,
                       RCPointer< Volume< T2 > > floating,
                       const T1& referenceLowerThreshold,
                       const T2& floatingLowerThreshold,
                       int32_t resamplingOrder,
                       int32_t subSamplingMaximumSize,
                       bool verbose );

    RCPointer< Volume< T1 > > _reference;
    RCPointer< Volume< T2 > > _floating;

    T1 _referenceLowerThreshold;
    T2 _floatingLowerThreshold;
    int32_t _resamplingOrder;
    int32_t _subSamplingMaximumSize;
    bool _verbose;

    RCPointer< Volume< uint8_t > > _resampledReferenceU8;
    RCPointer< Volume< uint8_t > > _resampledFloatingU8;
    RCPointer< Volume< uint8_t > > _resampledReferenceMask;
    RCPointer< Volume< uint8_t > > _resampledFloatingMask;

    Resampler< uint8_t >* _resampler;
    RCPointer< Volume< uint8_t > > _transformedFloatingU8;

};


}


#endif
