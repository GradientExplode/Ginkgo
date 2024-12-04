#ifndef _gkg_dmri_tractography_ProbabilisticTractographyAlgorithm_h_
#define _gkg_dmri_tractography_ProbabilisticTractographyAlgorithm_h_


#include <gkg-dmri-tractography/StreamlineTractographyAlgorithm.h>
#include <gkg-processing-algobase/Rescaler_i.h>
#include <gkg-core-pattern/Creator.h>


namespace gkg
{


class OdfCartesianField;


template < class L, class Compare = std::less< L > >
class ProbabilisticTractographyAlgorithm :
           public StreamlineTractographyAlgorithm< L, Compare >,
           public Creator4Arg< ProbabilisticTractographyAlgorithm< L, Compare >,
                               TractographyAlgorithm< L, Compare >,
                               const Vector3d< int32_t >&,
                               const Vector3d< double >&,
                               const std::vector< double >&,
                               const std::vector< std::string >& >

{

  public:

    ProbabilisticTractographyAlgorithm( const Vector3d< int32_t >& size,
                                        const Vector3d< double >& resolution,
                                        float step,
                                        int32_t storingIncrement,
                                        float minimumFiberLength,
                                        float maximumFiberLength,
                                        float apertureAngle,
                                        float temperature,
                                        float gfaLowerBoundary,
                                        float gfaUpperBoundary );
    ProbabilisticTractographyAlgorithm(
                           const Vector3d< int32_t >& size,
                           const Vector3d< double >& resolution,
                           const std::vector< double >& scalarParameters,
                           const std::vector< std::string >& stringParameters );
    virtual ~ProbabilisticTractographyAlgorithm();

    float getTemperature() const;
    float getGFALowerBoundary() const;
    float getGFAUpperBoundary() const;
    bool usingGFAThreshold() const;
    float getGFAThreshold() const;

    void prepare( OdfCartesianField& odfCartesianField,
                  const Volume< int16_t >& computingMask,
                  const Transform3d< float >&
                                            transform3dFromRoisVoxelToOdfsReal,
                  const Transform3d< float >&
                                            transform3dFromRoisVoxelToMaskVoxel,
                  const Transform3d< float >&
                                            transform3dFromOdfsRealToMaskVoxel,
                  const Transform3d< float >&
                                            transform3dFromOdfsRealToRoisVoxel,
                  bool verbose );

    static std::string getStaticName();

  protected:

    friend class Creator4Arg< ProbabilisticTractographyAlgorithm< L, Compare >,
                              TractographyAlgorithm< L, Compare >,
                              const Vector3d< int32_t >&,
                              const Vector3d< double >&,
                              const std::vector< double >&,
                              const std::vector< std::string >& >;

    bool track( const Vector3d< float >& startingPointOdfs,
                const int32_t& startingOrientationIndex,
                OdfContinuousField& odfContinuousField,
                const RCPointer< StoppingStrategy >& stoppingStrategy,
                std::list< Vector3d< float > >& trajectory,
                float& fiberLength ) const;

    float _temperature;
    float _gfaLowerBoundary;
    float _gfaUpperBoundary;
    bool _usingGFAThreshold;
    float _gfaThreshold;
    Rescaler< float, float > _gfaRescaler;  

};


}


#endif
