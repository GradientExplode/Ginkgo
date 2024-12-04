#ifndef _gkg_dmri_connectivity_matrix_ConnectivityMatrixFactory_i_h_
#define _gkg_dmri_connectivity_matrix_ConnectivityMatrixFactory_i_h_


#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFactory.h>
#include <gkg-dmri-connectivity-matrix/ConnectivityMatrixFunctor_i.h>
#include <gkg-core-exception/Exception.h>


template < class L, class Compare >
inline
gkg::ConnectivityMatrixFactory< L, Compare >::ConnectivityMatrixFactory()
{
}


template < class L, class Compare >
inline
gkg::ConnectivityMatrixFactory< L, Compare >::~ConnectivityMatrixFactory()
{
}


template < class L, class Compare >
inline
bool 
gkg::ConnectivityMatrixFactory< L, Compare >::registerConnectivityMatrixFunctor(
        const std::string& typeROIs1,
        const std::string& typeROIs2,
        typename gkg::ConnectivityMatrixFactory< L, Compare >::Creator creator,
        const std::string& scalarParameterHelp,
        const std::string& stringParameterHelp,
        const std::string& functorHelp )
{

  try
  {

    // sanity check
    typename 
    std::map< std::string,
              std::map< std::string,
                        typename gkg::ConnectivityMatrixFactory<
                                          L,
                                          Compare >::Creator > >::const_iterator
      c1 = _creators.find( typeROIs1 );

    if ( c1 != _creators.end() )
    {

      typename 
      std::map< std::string,
                typename gkg::ConnectivityMatrixFactory<
                                          L,
                                          Compare >::Creator >::const_iterator
        c2 = c1->second.find( typeROIs2 );

      if ( c2 != c1->second.end() )
      {

        throw std::runtime_error(
                            std::string( "connectivity matrix functor "
                                         "creator already registered for ( " ) +
                            typeROIs1 + ", " + typeROIs2 + " )" );

      }

    }

    if ( !creator )
    {

      throw std::runtime_error( "nul connectivity matrix functor creator" );

    }

    // registering creator
    _creators[ typeROIs1 ][ typeROIs2 ] = creator;
    _scalarParameterHelps[ typeROIs1 ][ typeROIs2 ] = scalarParameterHelp;
    _stringParameterHelps[ typeROIs1 ][ typeROIs2 ] = stringParameterHelp;
    _functorHelps[ typeROIs1 ][ typeROIs2 ] = functorHelp;

    return true;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "bool  "
             "gkg::ConnectivityMatrixFactory< L, Compare >::"
             "registerConnectivityMatrixFunctor( "
             "const std::string& typeROIs1, "
             "const std::string& typeROIs2, "
             "typename gkg::ConnectivityMatrixFactory< L, Compare >::Creator"
             " creator )" );

}



template < class L, class Compare >
inline
std::string 
gkg::ConnectivityMatrixFactory< L, Compare >::getScalarParameterHelp() const
{

  try
  {

    std::string help = "Scalar parameters according to ROIs1/ROIs2:\n";
    std::map< std::string,
              std::map< std::string, std::string > >::const_iterator
      c1 = _scalarParameterHelps.begin(),
      c1e = _scalarParameterHelps.end();

    while ( c1 != c1e )
    {

      std::map< std::string, std::string >::const_iterator
        c2 = c1->second.begin(),
        c2e = c1->second.end();

      while ( c2 != c2e )
      {

        help += "- for " + c1->first + " to " + c2->first +
                " connectivity type\n";
        help += c2->second;
        ++ c2;

      }

      ++ c1;

    }
    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::ConnectivityMatrixFactory< L, Compare >::"
             "getScalarParameterHelp() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::ConnectivityMatrixFactory< L, Compare >::getStringParameterHelp() const
{

  try
  {

    std::string help = "String parameters according to ROIs1/ROIs2:\n";
    std::map< std::string,
              std::map< std::string, std::string > >::const_iterator
      c1 = _stringParameterHelps.begin(),
      c1e = _stringParameterHelps.end();

    while ( c1 != c1e )
    {

      std::map< std::string, std::string >::const_iterator
        c2 = c1->second.begin(),
        c2e = c1->second.end();

      while ( c2 != c2e )
      {

        help += "- for " + c1->first + " to " + c2->first +
                " connectivity type\n";
        help += c2->second;
        ++ c2;

      }

      ++ c1;

    }
    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::ConnectivityMatrixFactory< L, Compare >::"
             "getStringParameterHelp() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::ConnectivityMatrixFactory< L, Compare >::getFunctorHelp() const
{

  try
  {

    std::string help = "List of functors according to ROIs1/ROIs2:\n";
    std::map< std::string,
              std::map< std::string, std::string > >::const_iterator
      c1 = _functorHelps.begin(),
      c1e = _functorHelps.end();

    while ( c1 != c1e )
    {

      std::map< std::string, std::string >::const_iterator
        c2 = c1->second.begin(),
        c2e = c1->second.end();

      while ( c2 != c2e )
      {

        help += "- for " + c1->first + " to " + c2->first +
                " connectivity type\n";
        help += c2->second;
        ++ c2;

      }

      ++ c1;

    }
    return help;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::ConnectivityMatrixFactory< L, Compare >::"
             "getFunctorHelp() const" );

}


template < class L, class Compare >
inline
gkg::ConnectivityMatrixFunctor< L, Compare >*
gkg::ConnectivityMatrixFactory< L, Compare >::create(
                      const std::string& typeROIs1,
                      const std::string& typeROIs2,
                      const std::vector< double >& scalarParameters,
                      const std::vector< std::string >& stringParameters ) const
{

  try
  {

    typename 
    std::map< std::string, std::map< std::string,
                                     typename gkg::ConnectivityMatrixFactory<
                                          L,
                                          Compare >::Creator > >::const_iterator
      c1 = _creators.find( typeROIs1 );

    if ( c1 == _creators.end() )
    {

       throw std::runtime_error( "invalid type of ROIs for first ROI set" );

    }

    typename 
    std::map< std::string,
              typename gkg::ConnectivityMatrixFactory<
                                          L,
                                          Compare >::Creator >::const_iterator
      c2 = c1->second.find( typeROIs2 );

    if ( c2 == c1->second.end() )
    {

       throw std::runtime_error( "invalid type of ROIs for second ROI set" );

    }

    typename 
    gkg::ConnectivityMatrixFactory< L, Compare >::Creator
      creator = c2->second;

    return ( *creator )( scalarParameters, stringParameters );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::ConnectivityMatrixFunctor< L, Compare >* "
             "gkg::ConnectivityMatrixFactory< L, Compare >::create( "
             "const std::string& typeROIs1, "
             "const std::string& typeROIs2, "
             "const std::vector< double >& scalarParameters, "
             "const std::vector< std::string >& stringParameters ) const" );

}


#endif
