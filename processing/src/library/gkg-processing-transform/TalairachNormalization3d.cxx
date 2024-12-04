#include <gkg-processing-transform/TalairachNormalization3d_i.h>


template < class T >
gkg::TalairachNormalization3d< T >::TalairachNormalization3d()
                                   : gkg::Scaling3d< T >( 1.0, 1.0, 1.0, true )
{
}


template < class T >
gkg::TalairachNormalization3d< T >::TalairachNormalization3d(
                                     const gkg::HeaderedObject& headeredObject )
                                   : gkg::Scaling3d< T >()
{

  try
  {

    if ( headeredObject.getHeader().hasAttribute( 
                                           "talairach_normalization_scaling" ) )
    {

      gkg::GenericObjectList genericObjectList;
      try
      {

        headeredObject.getHeader().getAttribute( 
                                              "talairach_normalization_scaling",
					      genericObjectList );
        this->setDirectScaling( ( T )genericObjectList[ 0 ]->getScalar(),
                                ( T )genericObjectList[ 1 ]->getScalar(),
                                ( T )genericObjectList[ 2 ]->getScalar(),
                                true,
                                false );

      }
      catch ( std::exception& )
      {

        std::vector< T > tmp;
        headeredObject.getHeader().getAttribute( 
                                              "talairach_normalization_scaling",
					      tmp );
        this->setDirectScaling( tmp[ 0 ], tmp[ 1 ], tmp[ 2 ], true, false );
 
      }

    }
    else
    {
    
      throw std::runtime_error( 
                      "'talairach_normalization_scaling' attribute not found" );
    
    }

  }
  GKG_CATCH( "template < class T > "
             "gkg::TalairachNormalization3d< T >::TalairachNormalization3d( "
             "const gkg::HeaderedObject& headeredObject )" );

}


template < class T >
gkg::TalairachNormalization3d< T >::TalairachNormalization3d(
                                             const gkg::Vector3d< T >& scaling )
                                   : gkg::Scaling3d< T >( scaling, true )
{
}


template < class T >
gkg::TalairachNormalization3d< T >::TalairachNormalization3d(
                               const gkg::TalairachNormalization3d< T >& other )
                                   : gkg::Scaling3d< T >( other )
{
}


template < class T >
gkg::TalairachNormalization3d< T >::~TalairachNormalization3d()
{
}


template < class T >
gkg::TalairachNormalization3d< T >&
gkg::TalairachNormalization3d< T >::operator=(
                               const gkg::TalairachNormalization3d< T >& other )
{

  try
  {

    this->gkg::Scaling3d< T >::operator=( other );

    return *this;

  }
  GKG_CATCH( "template < class T > "
             "gkg::TalairachNormalization3d< T >& "
             "gkg::TalairachNormalization3d< T >::operator=( "
             "const gkg::TalairachNormalization3d< T >& other )" );

}


template < class T >
void gkg::TalairachNormalization3d< T >::addToHeaderedObject(
                                              gkg::HeaderedObject& other ) const
{

  try
  {
  
    gkg::TalairachNormalization3d< float >::addSemantic( other );
    
    // adding AC coordinate to header
    std::vector< T > scaling( 3 );
    this->getDirectScaling( scaling[ 0 ], scaling[ 1 ], scaling[ 2 ] );
    other.getHeader().addAttribute( "talairach_normalization_scaling",
                                    scaling );
    
  }
  GKG_CATCH( "template < class T > "
             "void gkg::TalairachNormalization3d< T >::addToHeaderedObject("
             "gkg::HeaderedObject& other ) const" );

}


template < class T >
void gkg::TalairachNormalization3d< T >::addSemantic( HeaderedObject& other )
{

  try
  {

    other.addSemantic( "__talairach_normalization__",
                       "talairach_normalization_scaling",
		       gkg::Semantic(
		                 gkg::TypeOf< std::vector< T > >::getName() ) ); 
			
  }
  GKG_CATCH( "template < class T > "
             "void gkg::TalairachNormalization3d< T >::addSemantic(" 
             "gkg::HeaderedObject& other )" );

}


template class gkg::TalairachNormalization3d< float >;
template class gkg::TalairachNormalization3d< double >;
