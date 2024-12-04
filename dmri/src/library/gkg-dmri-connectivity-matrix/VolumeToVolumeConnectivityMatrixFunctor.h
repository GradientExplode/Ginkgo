#ifndef _gkg_dmri_connectivity_matrix_VolumeToVolumeConnectivityMatrixFunctor_h_
#define _gkg_dmri_connectivity_matrix_VolumeToVolumeConnectivityMatrixFunctor_h_



#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFunctor.h>
#include <gkg-core-pattern/Creator.h>
#include <vector>


namespace gkg
{



template < class L, class Compare = std::less< L > >
class VolumeToVolumeConnectivityMatrixFunctor :
     public ConnectivityMatrixFunctor< L, Compare >,
     public Creator2Arg< VolumeToVolumeConnectivityMatrixFunctor< L, Compare >,
                         ConnectivityMatrixFunctor< L, Compare >,
                         const std::vector< double >&,
                         const std::vector< std::string >& >
{

  public:

    ~VolumeToVolumeConnectivityMatrixFunctor();

    void apply( const std::list< std::string >& fileNameBundleMaps,
                const std::list< std::string >& fileNameListOfROIs1,
                const std::string& fileNameROIs1ToBundleMapTransform3d,
                const std::list< std::string >& fileNameListOfROIs2,
                const std::string& fileNameROIs2ToBundleMapTransform3d,
                const std::list< std::string >& functorNames,
                const std::list< std::string >& outputFileNames,
                const std::string& theOutputBundleMapFormat,
                const std::string& theOutputTextureMapFormat,
                bool ascii,
                bool verbose ) const;

    static std::string getScalarParameterHelp();
    static std::string getStringParameterHelp();
    static std::string getFunctorHelp();

  protected:

    friend 
    class Creator2Arg< VolumeToVolumeConnectivityMatrixFunctor< L, Compare >,
                       ConnectivityMatrixFunctor< L, Compare >,
                       const std::vector< double >&,
                       const std::vector< std::string >& >;

    VolumeToVolumeConnectivityMatrixFunctor(
                           const std::vector< double >& scalarParameters,
                           const std::vector< std::string >& stringParameters );

};


}


#endif

