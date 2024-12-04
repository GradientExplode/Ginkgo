#include <gkg-dmri-connectivity-matrix/MeshToMeshConnectivityMatrixFunctor_i.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory.h>


template class gkg::MeshToMeshConnectivityMatrixFunctor< int16_t,
                                                      std::less< int16_t > >;
template class gkg::MeshToMeshConnectivityMatrixFunctor<
                                                     std::string,
                                                     std::less< std::string > >;


static bool register_gkg_MeshToMeshConnectivityMatrixFunctor_int16_t 
                                                       __attribute__((unused)) =
  gkg::ConnectivityMatrixFactory< int16_t,
                                std::less< int16_t > >::getInstance().
     registerConnectivityMatrixFunctor(
              "mesh",
              "mesh",
              gkg::MeshToMeshConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::createInstance,
              gkg::MeshToMeshConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getScalarParameterHelp(),
              gkg::MeshToMeshConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getStringParameterHelp(),
              gkg::MeshToMeshConnectivityMatrixFunctor<
                           int16_t,
                           std::less< int16_t > >::getFunctorHelp() );

static bool register_gkg_MeshToMeshConnectivityMatrixFunctor_std_string 
                                                       __attribute__((unused)) =
  gkg::ConnectivityMatrixFactory< std::string,
                                std::less< std::string > >::getInstance().
     registerConnectivityMatrixFunctor(
              "mesh",
              "mesh",
              gkg::MeshToMeshConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::createInstance,
              gkg::MeshToMeshConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getScalarParameterHelp(),
              gkg::MeshToMeshConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getStringParameterHelp(),
              gkg::MeshToMeshConnectivityMatrixFunctor<
                           std::string,
                           std::less< std::string > >::getFunctorHelp() );
