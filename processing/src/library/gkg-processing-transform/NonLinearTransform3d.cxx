#include <gkg-processing-transform/NonLinearTransform3d.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-io/TypedVolumeReaderProcess.h>
#include <gkg-processing-coordinates/Vector3d.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-io/Writer_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


template < class T >
gkg::NonLinearTransform3d< T >::NonLinearTransform3d(
                                         bool applyInverseLinearTransformFirst )
                               : gkg::Transform3d< T >(),
                                 _applyInverseLinearTransformFirst(
                                              applyInverseLinearTransformFirst )
{
}


template < class T >
gkg::NonLinearTransform3d< T >::NonLinearTransform3d(
       const gkg::RCPointer< gkg::ParameterizedRigidTransform3d< T > >&
                                                  parameterizedRigidTransform3d,
       const gkg::RCPointer< gkg::VectorFieldTransform3d< T > >&
                                                  vectorFieldTransform3d,
       bool applyInverseLinearTransformFirst )
                          : gkg::Transform3d< T >(),
                            _parameterizedRigidTransform3d(
                                                parameterizedRigidTransform3d ),
                            _linearTransform3d(
                                                parameterizedRigidTransform3d ),
                            _vectorFieldTransform3d( vectorFieldTransform3d ),
                            _applyInverseLinearTransformFirst( 
                                              applyInverseLinearTransformFirst )
{
}


template < class T >
gkg::NonLinearTransform3d< T >::NonLinearTransform3d(
     const gkg::RCPointer< gkg::ParameterizedAffineWoShearingTransform3d< T > >&
                                       parameterizedAffineWoShearingTransform3d,
      const gkg::RCPointer< gkg::VectorFieldTransform3d< T > >&
                                       vectorFieldTransform3d,
      bool applyInverseLinearTransformFirst )
                          : gkg::Transform3d< T >(),
                            _parameterizedAffineWoShearingTransform3d(
                                     parameterizedAffineWoShearingTransform3d ),
                            _linearTransform3d(
                                     parameterizedAffineWoShearingTransform3d ),
                            _vectorFieldTransform3d( vectorFieldTransform3d ),
                            _applyInverseLinearTransformFirst( 
                                              applyInverseLinearTransformFirst )
{
}


template < class T >
gkg::NonLinearTransform3d< T >::NonLinearTransform3d(
       const gkg::RCPointer< gkg::ParameterizedAffineTransform3d< T > >& 
                                                 parameterizedAffineTransform3d,
       const gkg::RCPointer< gkg::VectorFieldTransform3d< T > >&
                                                 vectorFieldTransform3d,
       bool applyInverseLinearTransformFirst )
                          : gkg::Transform3d< T >(),
                            _parameterizedAffineTransform3d(
                                               parameterizedAffineTransform3d ),
                            _linearTransform3d( 
                                               parameterizedAffineTransform3d ),
                            _vectorFieldTransform3d( vectorFieldTransform3d ),
                            _applyInverseLinearTransformFirst( 
                                              applyInverseLinearTransformFirst )
{
}


template < class T >
gkg::NonLinearTransform3d< T >::NonLinearTransform3d(
               const gkg::RCPointer< gkg::VectorFieldTransform3d< T > >&
                                                         vectorFieldTransform3d,
               bool applyInverseLinearTransformFirst )
                          : gkg::Transform3d< T >(),
                            _vectorFieldTransform3d( vectorFieldTransform3d ),
                            _applyInverseLinearTransformFirst( 
                                              applyInverseLinearTransformFirst )
{
}


template < class T >
gkg::NonLinearTransform3d< T >::NonLinearTransform3d(
                                   const gkg::NonLinearTransform3d< T >& other )
                          : gkg::Transform3d< T >(),
                            _parameterizedRigidTransform3d(
                                         other._parameterizedRigidTransform3d ),
                            _parameterizedAffineWoShearingTransform3d(
                              other._parameterizedAffineWoShearingTransform3d ),
                            _parameterizedAffineTransform3d(
                                        other._parameterizedAffineTransform3d ),
                            _linearTransform3d( other._linearTransform3d ),
                            _vectorFieldTransform3d(
                                                other._vectorFieldTransform3d ),
                            _applyInverseLinearTransformFirst(
                                       other._applyInverseLinearTransformFirst )
{
}


template < class T >
gkg::NonLinearTransform3d< T >::~NonLinearTransform3d()
{
}



template < class T >
gkg::RCPointer< gkg::Transform3d< T > > 
gkg::NonLinearTransform3d< T >::clone() const
{

  try
  {

    gkg::RCPointer< Transform3d< T > > transform;
    transform.reset( new gkg::NonLinearTransform3d< T >( *this ) );
    return transform;

  }
  GKG_CATCH( "template < class T > "
             "gkg::RCPointer< gkg::Transform3d< T > > "
             "gkg::NonLinearTransform3d< T >::clone() const" );

}


template < class T >
void gkg::NonLinearTransform3d< T >::getDirect( const gkg::Vector3d< T >& from, 
                                                gkg::Vector3d< T >& to ) const
{

  try
  {

    if ( _applyInverseLinearTransformFirst )
    {

      gkg::Vector3d< T > intermediate( from );
      if ( !_linearTransform3d.isNull() )
      {

        _linearTransform3d->getDirect( from, intermediate );

      }

      if ( !_vectorFieldTransform3d.isNull() )
      {

        _vectorFieldTransform3d->getDirect( intermediate, to );

      }
      else
      {

        to = intermediate;

      }

    }
    else
    {

      gkg::Vector3d< T > intermediate( from );
      if ( !_vectorFieldTransform3d.isNull() )
      {

        _vectorFieldTransform3d->getDirect( from, intermediate );

      }

      if ( !_linearTransform3d.isNull() )
      {

        _linearTransform3d->getDirect( intermediate, to );

      }
      else
      {

        to = intermediate;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NonLinearTransform3d< T >::getDirect( "
             "const gkg::Vector3d< T >& from, "
             "gkg::Vector3d< T >& to ) const" );

}


template < class T >
void gkg::NonLinearTransform3d< T >::getInverse(
                                                const gkg::Vector3d< T >& to, 
                                                gkg::Vector3d< T >& from ) const
{

  try
  {

    if ( _applyInverseLinearTransformFirst )
    {

      gkg::Vector3d< T > intermediate( to );

      if ( !_vectorFieldTransform3d.isNull() )
      {

        _vectorFieldTransform3d->getInverse( to, intermediate );

      }

      if ( !_linearTransform3d.isNull() )
      {

        _linearTransform3d->getInverse( intermediate, from );

      }
      else
      {

        from = intermediate;

      }

    }
    else
    {

      gkg::Vector3d< T > intermediate( to );
      if ( !_linearTransform3d.isNull() )
      {

        _linearTransform3d->getInverse( to, intermediate );

      }

      if ( !_vectorFieldTransform3d.isNull() )
      {

        _vectorFieldTransform3d->getInverse( intermediate, from );

      }
      else
      {

        from = intermediate;

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NonLinearTransform3d< T >::getInverse( "
             "const gkg::Vector3d< T >& to, "
             "gkg::Vector3d< T >& from ) const" );

}


template < class T >
void gkg::NonLinearTransform3d< T >::readTrm(
                          const std::string& fileNameLinearTransform,
                          const std::string& fileNameNonLinearDirectTransform,
                          const std::string& fileNameNonLinearInverseTransform )
{

  try
  {

    // sanity check
    if ( fileNameNonLinearDirectTransform.empty() !=
         fileNameNonLinearInverseTransform.empty() )
    {

      throw std::runtime_error(
                          "direct or inverse non linear transform is missing" );

    }

    // saving the linear part of the non linear 3D transformation
    if ( !fileNameLinearTransform.empty() )
    {

      try
      {

        gkg::ParameterizedRigidTransform3d< T >*
          parameterizedRigidTransform3d =
                                    new gkg::ParameterizedRigidTransform3d< T >;

        std::ifstream is( fileNameLinearTransform.c_str() );
        parameterizedRigidTransform3d->readTrm( is );

        is.close();

        _parameterizedRigidTransform3d.reset( parameterizedRigidTransform3d );
        _linearTransform3d = _parameterizedRigidTransform3d;

      }
      catch( std::exception& )
      {

        try
        {
          gkg::ParameterizedAffineTransform3d< T >*
            parameterizedAffineTransform3d = 
                                   new gkg::ParameterizedAffineTransform3d< T >;
          std::ifstream is( fileNameLinearTransform.c_str() );
          parameterizedAffineTransform3d->readTrm( is );
          is.close();

          _parameterizedAffineTransform3d.reset(
                                               parameterizedAffineTransform3d );
          _linearTransform3d = _parameterizedAffineTransform3d;

        }
        catch( std::exception& )
        {

          throw std::runtime_error( "unable to read the linear transform as "
                                    " a rigid/affine wo shearing/affine one" );

        }

      }

    }

    if ( !fileNameNonLinearDirectTransform.empty() )
    {

      gkg::Volume< T > directVectorField;
      try
      {

        gkg::TypedVolumeReaderProcess< T >
          typedDirectVolumeReaderProcess( directVectorField );
        typedDirectVolumeReaderProcess.execute(
                                             fileNameNonLinearDirectTransform );

      }
      catch ( std::exception& )
      {

        gkg::Volume< gkg::Vector3d< float > > directVectorFieldOfVector3d;

        gkg::Reader::getInstance().read( fileNameNonLinearDirectTransform,
                                         directVectorFieldOfVector3d );

        gkg::Vector3d< int32_t > size;
        gkg::Vector3d< double > resolution;

        directVectorFieldOfVector3d.getSize( size );
        directVectorFieldOfVector3d.getResolution( resolution );

        directVectorField.reallocate( size, 3 );
        directVectorField.setResolution( resolution );

        int32_t x = 0;
        int32_t y = 0;
        int32_t z = 0;
        for ( z = 0; z < size.z; z++ )
        {

          for ( y = 0; y < size.y; y++ )
          {

            for ( x = 0; x < size.x; x++ )
            {

              directVectorField( x, y, z, 0 ) =
                                  ( T )directVectorFieldOfVector3d( x, y, z ).x;
              directVectorField( x, y, z, 1 ) =
                                  ( T )directVectorFieldOfVector3d( x, y, z ).y;
              directVectorField( x, y, z, 2 ) =
                                  ( T )directVectorFieldOfVector3d( x, y, z ).z;

            }

          }

        }

      }

      gkg::Volume< T > inverseVectorField;
      try
      {

        gkg::TypedVolumeReaderProcess< T >
          typedInverseVolumeReaderProcess( inverseVectorField );
        typedInverseVolumeReaderProcess.execute(
                                            fileNameNonLinearInverseTransform );

      }
      catch ( std::exception& )
      {

        gkg::Volume< gkg::Vector3d< float > > inverseVectorFieldOfVector3d;
        gkg::Reader::getInstance().read( fileNameNonLinearInverseTransform,
                                         inverseVectorFieldOfVector3d );

        gkg::Vector3d< int32_t > size;
        gkg::Vector3d< double > resolution;

        inverseVectorFieldOfVector3d.getSize( size );
        inverseVectorFieldOfVector3d.getResolution( resolution );

        inverseVectorField.reallocate( size, 3 );
        inverseVectorField.setResolution( resolution );

        int32_t x = 0;
        int32_t y = 0;
        int32_t z = 0;
        for ( z = 0; z < size.z; z++ )
        {

          for ( y = 0; y < size.y; y++ )
          {

            for ( x = 0; x < size.x; x++ )
            {

              inverseVectorField( x, y, z, 0 ) =
                                 ( T )inverseVectorFieldOfVector3d( x, y, z ).x;
              inverseVectorField( x, y, z, 1 ) =
                                 ( T )inverseVectorFieldOfVector3d( x, y, z ).y;
              inverseVectorField( x, y, z, 2 ) =
                                 ( T )inverseVectorFieldOfVector3d( x, y, z ).z;

            }

          }

        }

      }

      if ( _vectorFieldTransform3d.isNull() )
      {

        _vectorFieldTransform3d.reset( new gkg::VectorFieldTransform3d< T >(
                                                         directVectorField,
                                                         inverseVectorField ) );

      }
      else
      {

        _vectorFieldTransform3d->setDirectAndInverseVectorFields( 
                                                           directVectorField,
                                                           inverseVectorField );

      }

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NonLinearTransform3d< T >::readTrm( "
             "const std::string& fileNameLinearTransform, "
             "const std::string& fileNameNonLinearDirectTransform, "
             "const std::string& fileNameNonLinearInverseTransform )" );

}


template < class T >
void gkg::NonLinearTransform3d< T >::writeTrm(
                    const std::string& fileNameLinearTransform,
                    const std::string& fileNameNonLinearDirectTransform,
                    const std::string& fileNameNonLinearInverseTransform ) const
{

  try
  {

    // sanity check
    if ( fileNameNonLinearDirectTransform.empty() !=
         fileNameNonLinearInverseTransform.empty() )
    {

      throw std::runtime_error(
                          "direct or inverse non linear transform is missing" );

    }

    // saving the linear part of the non linear 3D transformation
    if ( !_linearTransform3d.isNull() && !fileNameLinearTransform.empty() )
    {

      std::ofstream os( fileNameLinearTransform.c_str() );
      _linearTransform3d->writeTrm( os );
      os.close();

    }

    // saving the vector field
    if ( !_vectorFieldTransform3d.isNull() && 
         !fileNameNonLinearDirectTransform.empty() && 
         !fileNameNonLinearInverseTransform.empty() )
    {

      gkg::Volume< T > directVectorField;
      gkg::Volume< T > inverseVectorField;
      _vectorFieldTransform3d->getDirectAndInverseVectorFields( 
                                                           directVectorField,
                                                           inverseVectorField );
      gkg::Writer::getInstance().write( fileNameNonLinearDirectTransform,
                                        directVectorField );
      gkg::Writer::getInstance().write( fileNameNonLinearInverseTransform,
                                        inverseVectorField );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NonLinearTransform3d< T >::writeTrm( "
             "const std::string& fileNameLinearTransform, "
             "const std::string& fileNameNonLinearDirectTransform, "
             "const std::string& fileNameNonLinearInverseTransform ) const" );

}


template < class T >
void gkg::NonLinearTransform3d< T >::writeInverseTrm(
                    const std::string& fileNameLinearTransform,
                    const std::string& fileNameNonLinearDirectTransform,
                    const std::string& fileNameNonLinearInverseTransform ) const
{

  try
  {

    // sanity check
    if ( fileNameNonLinearDirectTransform.empty() !=
         fileNameNonLinearInverseTransform.empty() )
    {

      throw std::runtime_error(
                          "direct or inverse non linear transform is missing" );

    }

    // saving the linear part of the non linear 3D transformation
    if ( !_linearTransform3d.isNull() && !fileNameLinearTransform.empty() )
    {

      if ( !_parameterizedRigidTransform3d.isNull() )
      {

        const gkg::RigidTransform3d< T >&
          rigidTransform3d =
            _parameterizedRigidTransform3d->getRigidTransform3d();
        std::vector< T > parameters( 6 );
        rigidTransform3d.getDirectParameters( parameters );
        gkg::RigidTransform3d< T >
          inverseRigidTransform3d( parameters,
                                   false,
                                   false );
        std::ofstream os( fileNameLinearTransform.c_str() );
        inverseRigidTransform3d.writeTrm( os );
        os.close();

      }
      else if ( !_parameterizedAffineWoShearingTransform3d.isNull() )
      {

        const gkg::AffineWoShearingTransform3d< T >& 
          affineWoShearingTransform3d =
            _parameterizedAffineWoShearingTransform3d->
                                              getAffineWoShearingTransform3d();
        std::vector< T > parameters( 9 );
        affineWoShearingTransform3d.getDirectParameters( parameters );
        gkg::AffineWoShearingTransform3d< T > 
          inverseAffineWoShearingTransform3d( parameters,
                                              false,
                                              false );
        std::ofstream os( fileNameLinearTransform.c_str() );
        inverseAffineWoShearingTransform3d.writeTrm( os );
        os.close();

      }
      else if ( !_parameterizedAffineTransform3d.isNull() )
      {

        const gkg::AffineTransform3d< T >& 
          affineTransform3d =
            _parameterizedAffineTransform3d->getAffineTransform3d();
        std::vector< T > parameters( 12 );
        affineTransform3d.getDirectParameters( parameters );
        gkg::AffineTransform3d< T > 
          inverseAffineTransform3d( parameters,
                                    false,
                                    false );
        std::ofstream os( fileNameLinearTransform.c_str() );
        inverseAffineTransform3d.writeTrm( os );
        os.close();

      }


    }

    // saving the vector field
    if ( !_vectorFieldTransform3d.isNull() && 
         !fileNameNonLinearDirectTransform.empty() && 
         !fileNameNonLinearInverseTransform.empty() )
    {

      gkg::Volume< T > directVectorField;
      gkg::Volume< T > inverseVectorField;
      _vectorFieldTransform3d->getDirectAndInverseVectorFields( 
                                                           directVectorField,
                                                           inverseVectorField );

      gkg::Writer::getInstance().write( fileNameNonLinearDirectTransform,
                                        inverseVectorField );
      gkg::Writer::getInstance().write( fileNameNonLinearInverseTransform,
                                        directVectorField );

    }

  }
  GKG_CATCH( "template < class T > "
             "void gkg::NonLinearTransform3d< T >::writeTrm( "
             "const std::string& fileNameLinearTransform, "
             "const std::string& fileNameNonLinearDirectTransform, "
             "const std::string& fileNameNonLinearInverseTransform ) const" );

}


template < class T >
const gkg::RCPointer< gkg::ParameterizedRigidTransform3d< T > >&
gkg::NonLinearTransform3d< T >::getParameterizedRigidTransform3d() const
{

  try
  {

    return _parameterizedRigidTransform3d;

  }
  GKG_CATCH( "template < class T > "
             "const gkg::RCPointer< gkg::ParameterizedRigidTransform3d< T > >& "
             "gkg::NonLinearTransform3d< T >::"
             "getParameterizedRigidTransform3d() "
             "const" );

}


template < class T >
const gkg::RCPointer< gkg::ParameterizedAffineWoShearingTransform3d< T > >&
gkg::NonLinearTransform3d< T >::
                             getParameterizedAffineWoShearingTransform3d() const
{

  try
  {

    return _parameterizedAffineWoShearingTransform3d;

  }
  GKG_CATCH( "template < class T > "
             "const gkg::RCPointer< "
             "gkg::ParameterizedAffineWoShearingTransform3d< T > >& "
             "gkg::NonLinearTransform3d< T >::"
             "getParameterizedAffineWoShearingTransform3d() const" );

}


template < class T >
const gkg::RCPointer< gkg::ParameterizedAffineTransform3d< T > >&
gkg::NonLinearTransform3d< T >::getParameterizedAffineTransform3d() const
{

  try
  {

    return _parameterizedAffineTransform3d;

  }
  GKG_CATCH( "template < class T > "
             "const gkg::RCPointer< "
             "gkg::ParameterizedAffineTransform3d< T > >& "
             "gkg::NonLinearTransform3d< T >::"
             "getParameterizedAffineTransform3d() "
             "const" );

}


template < class T >
const gkg::RCPointer< gkg::ParameterizedTransform3d< T > >&
gkg::NonLinearTransform3d< T >::getParameterizedLinearTransform3d() const
{

  try
  {

    return _linearTransform3d;

  }
  GKG_CATCH( "template < class T > "
             "const gkg::RCPointer< "
             "gkg::ParameterizedTransform3d< T > >& "
             "gkg::NonLinearTransform3d< T >::"
             "getParameterizedLinearTransform3d() "
             "const" );

}


template < class T >
const gkg::RCPointer< gkg::VectorFieldTransform3d< T > >&
gkg::NonLinearTransform3d< T >::getVectorFieldTransform3d() const
{

  try
  {

    return _vectorFieldTransform3d;

  }
  GKG_CATCH( "template < class T > "
             "const gkg::RCPointer< "
             "gkg::VectorFieldTransform3d< T > >& "
             "gkg::NonLinearTransform3d< T >::"
             "getVectorFieldTransform3d() "
             "const" );

}


template class gkg::NonLinearTransform3d< float >;
template class gkg::NonLinearTransform3d< double >;
