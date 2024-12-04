#ifndef _gkg_dmri_tractography_DeterministicTractographyAlgorithm_h_
#define _gkg_dmri_tractography_DeterministicTractographyAlgorithm_h_


#include <gkg-dmri-tractography/StreamlineTractographyAlgorithm.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class DeterministicTractographyAlgorithm :
           public StreamlineTractographyAlgorithm< L, Compare >,
           public Creator4Arg< DeterministicTractographyAlgorithm< L, Compare >,
                               TractographyAlgorithm< L, Compare >,
                               const Vector3d< int32_t >&,
                               const Vector3d< double >&,
                               const std::vector< double >&,
                               const std::vector< std::string >& >
{

  public:

    DeterministicTractographyAlgorithm( const Vector3d< int32_t >& size,
                                        const Vector3d< double >& resolution,
                                        float step,
                                        int32_t storingIncrement,
                                        float minimumFiberLength,
                                        float maximumFiberLength,
                                        float apertureAngle );
    DeterministicTractographyAlgorithm(
                           const Vector3d< int32_t >& size,
                           const Vector3d< double >& resolution,
                           const std::vector< double >& scalarParameters,
                           const std::vector< std::string >& stringParameters );
    virtual ~DeterministicTractographyAlgorithm();

    static std::string getStaticName();

  protected:

    friend class Creator4Arg< DeterministicTractographyAlgorithm< L, Compare >,
                              TractographyAlgorithm< L, Compare >,
                              const Vector3d< int32_t >&,
                              const Vector3d< double >&,
                              const std::vector< double >&,
                              const std::vector< std::string >& >;

    bool track( const Vector3d< float >& startingPoint,
                const int32_t& startingOrientationIndex,
                OdfContinuousField& odfContinuousField,
                const RCPointer< StoppingStrategy >& stoppingStrategy,
                std::list< Vector3d< float > >& trajectory,
                float& fiberLength ) const;

};


}


#endif
