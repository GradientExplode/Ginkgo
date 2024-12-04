#ifndef _gkg_dmri_connectivity_matrix_ConnectivityMatrixFunctor_h_
#define _gkg_dmri_connectivity_matrix_ConnectivityMatrixFunctor_h_


#include <list>
#include <string>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class ConnectivityMatrixFunctor
{

  public:

    virtual ~ConnectivityMatrixFunctor();

    virtual void apply( const std::list< std::string >& fileNameBundleMaps,
                        const std::list< std::string >& fileNameListOfROIs1,
                        const std::string& fileNameROIs1ToBundleMapTransform3d,
                        const std::list< std::string >& fileNameListOfROIs2,
                        const std::string& fileNameROIs2ToBundleMapTransform3d,
                        const std::list< std::string >& functorNames,
                        const std::list< std::string >& outputFileNames,
                        const std::string& theOutputBundleMapFormat,
                        const std::string& theOutputTextureMapFormat,
                        bool ascii,
                        bool verbose ) const = 0;


  protected:

    ConnectivityMatrixFunctor();

};


}


#endif


