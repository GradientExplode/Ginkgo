#ifndef _gkg_dmri_connectivity_matrix_ConnectivityMatrixFactory_h_
#define _gkg_dmri_connectivity_matrix_ConnectivityMatrixFactory_h_


#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFunctor.h>
#include <gkg-core-pattern/Singleton.h>
#include <map>
#include <vector>
#include <string>


namespace gkg
{


template < class L, class Compare = std::less< L > >
class ConnectivityMatrixFactory: public Singleton<
                                       ConnectivityMatrixFactory< L, Compare > >
{

  public:

    typedef ConnectivityMatrixFunctor< L, Compare >* ( *Creator )(
                           const std::vector< double >& scalarParameters,
                           const std::vector< std::string >& stringParameters );

    ~ConnectivityMatrixFactory();

    bool registerConnectivityMatrixFunctor(
                                         const std::string& typeROIs1,
                                         const std::string& typeROIs2,
                                         Creator creator,
                                         const std::string& scalarParameterHelp,
                                         const std::string& stringParameterHelp,
                                         const std::string& functorHelp ); 

    std::string getScalarParameterHelp() const;
    std::string getStringParameterHelp() const;
    std::string getFunctorHelp() const;

    ConnectivityMatrixFunctor< L, Compare >*
      create( const std::string& typeROIs1,
              const std::string& typeROIs2,
              const std::vector< double >& scalarParameters,
              const std::vector< std::string >& stringParameters ) const;


  protected:

    friend class Singleton< ConnectivityMatrixFactory< L, Compare > >;

    ConnectivityMatrixFactory();

    std::map< std::string, std::map< std::string, Creator > > _creators;
    std::map< std::string,
              std::map< std::string, std::string > > _scalarParameterHelps;
    std::map< std::string,
              std::map< std::string, std::string > > _stringParameterHelps;
    std::map< std::string,
              std::map< std::string, std::string > > _functorHelps;

};


}


#endif
