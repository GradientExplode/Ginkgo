#include <gkg-processing-coordinates/RandomVoxelSampler.h>
#include <gkg-core-exception/Exception.h>


template < class T >
gkg::RandomVoxelSampler< T >::RandomVoxelSampler(
                                      const gkg::Vector3d< double >& resolution,
                                      int32_t pointCount )
                                : gkg::VoxelSampler< T >( resolution,
                                                          pointCount ),
                                  _halfResolutionX( this->_resolution.x / 2.0 ),
                                  _halfResolutionY( this->_resolution.y / 2.0 ),
                                  _halfResolutionZ( this->_resolution.z / 2.0 ),
                                  _randomGenerator( gkg::RandomGenerator::Taus )

{

  try
  {

    // getting a pointer to the numerical analysis factory
    _factory =
      gkg::NumericalAnalysisSelector::getInstance().getImplementationFactory();

  }
  GKG_CATCH( "template < class T > "
             "gkg::RandomVoxelSampler< T >::RandomVoxelSampler( "
             "const gkg::Vector3d< double >& resolution, "
             "int32_t pointCount )" );

}


template < class T >
gkg::RandomVoxelSampler< T >::~RandomVoxelSampler()
{
}


template < class T >
void gkg::RandomVoxelSampler< T >::getPoint(
                                         const gkg::Vector3d< T >& origin,
                                         int32_t pointIndex,
                                         gkg::Vector3d< T >& destination ) const
{

  try
  {

    if ( ( pointIndex >= this->_pointCount ) || ( pointIndex < 0 ) )
    {

      throw std::runtime_error( "not a valid point index" );

    }    
    destination.x = origin.x + _factory->getUniformRandomNumber( 
                                                            _randomGenerator,
                                                            -_halfResolutionX,
                                                            +_halfResolutionX );
    destination.y = origin.y + _factory->getUniformRandomNumber( 
                                                            _randomGenerator,
                                                            -_halfResolutionY,
                                                            +_halfResolutionY );
    destination.z = origin.z + _factory->getUniformRandomNumber( 
                                                            _randomGenerator,
                                                            -_halfResolutionZ,
                                                            +_halfResolutionZ );

  }
  GKG_CATCH( "template < class T > "
             "void gkg::RandomVoxelSampler< T >::getPoint( "
             "const gkg::Vector3d< T >& origin, "
             "int32_t pointIndex, "
             "gkg::Vector3d< T >& destination ) const" );

}


template class gkg::RandomVoxelSampler< float >;
template class gkg::RandomVoxelSampler< double >;

