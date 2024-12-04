#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorRemoveDuplicates_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorRemoveDuplicates_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorRemoveDuplicates.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-dmri-bundle-distance/BundleDistanceFunctorFactory.h>
#include <gkg-core-io/Reader_i.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>


template < class L, class Compare >
inline
gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::
                                   BundleMapOperatorRemoveDuplicates(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                             : gkg::BundleMapOperator< L, Compare >( verbose ),
                               _bundleDistanceThreshold( 0.0f )
{

  try
  {

    // sanity checks for string parameters
    if ( stringParameters.size() != 1U )
    {

      throw std::runtime_error( "string parameters must have 1 argument" );

    }

    std::string bundleDistanceFunctorName = stringParameters[ 0 ];

    // sanity checks for scalar parameters
    if ( scalarParameters.size() != 1U )
         
    {

      throw std::runtime_error( 
                                "scalar parameters  must have 1 argument(s)" );

    }

    _bundleDistanceThreshold = ( float )scalarParameters[ 0 ];


    int32_t bundleDistanceParameterCount = 
                                   ( int32_t )( scalarParameters.size() - 1U );
    std::vector< double > 
               bundleDistanceFunctorParameters( bundleDistanceParameterCount );
    int32_t p = 0;
    for ( p = 0; p < bundleDistanceParameterCount; p++ )
    {

      bundleDistanceFunctorParameters[ p ] = scalarParameters[ p + 1 ];

    }

    _bundleDistanceFunctor.reset(
      gkg::BundleDistanceFunctorFactory::getInstance().create( 
                                           bundleDistanceFunctorName,
                                           bundleDistanceFunctorParameters ) );


  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::"
             "BundleMapOperatorRemoveDuplicates( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template < class L, class Compare >
inline
gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::
                                 ~BundleMapOperatorRemoveDuplicates()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::startContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    // sanity checks
    if ( inputBundleMap.isNull() )
    {

      throw std::runtime_error( "input pointer is empty" );

    }
    if ( outputBundleMap.isNull() )
    {

      throw std::runtime_error( "output pointer is empty" );

    }

    // allocating the set of bundle labels to be removed
    std::set< L > toBeRemoved;

    // looping over input bundles
    float bundleDistance = 0.0f;
    typename gkg::BundleMap< L, Compare >::const_iterator
      ib1 = inputBundleMap->begin(),
      ib1e = inputBundleMap->end();
    while ( ib1 != ib1e )
    {

      const L& label1 = ib1->first;
      const typename gkg::BundleMap< L, Compare >::Bundle&
        bundle1 = ib1->second;

      typename gkg::BundleMap< L, Compare >::const_iterator
      ib2 = ib1,
      ib2e = ib1e;

      std::advance( ib2, 1 );

      while ( ib2 != ib2e )
      {

        const L& label2 = ib2->first;
        const typename gkg::BundleMap< L, Compare >::Bundle&
        bundle2 = ib2->second;

        if ( ( toBeRemoved.find( label1 ) == toBeRemoved.end() ) &&
             ( toBeRemoved.find( label2 ) == toBeRemoved.end() ) )
        {

          std::cout << "( " << label1 << ", " << label2 << " ) "
                    << std::flush;
          bundleDistance = _bundleDistanceFunctor->getDistance( bundle1,
                                                                bundle2 );
          if ( bundleDistance <= _bundleDistanceThreshold )
          {

            toBeRemoved.insert( label2 );

          }

        }

        ++ ib2;

      }
      ++ ib1;

    }

    // now filling the output bundle map without duplicates
    typename gkg::BundleMap< L, Compare >::const_iterator
      ib = inputBundleMap->begin(),
      ibe = inputBundleMap->end();
    while ( ib != ibe )
    {

      const L& label = ib->first;
      if ( toBeRemoved.find( label ) == toBeRemoved.end() )
      {

        const typename gkg::BundleMap< L, Compare >::Bundle&
          bundle = ib->second;

        // adding the kept bundle to the output bundle map
        outputBundleMap->addBundle( label, bundle );

      }

      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorRemoveDuplicates< L, "
             "Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorRemoveDuplicates< L, "
             "Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorRemoveDuplicates< L, "
             "Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::getName() const
{

  try
  {

    return 
        gkg::BundleMapOperatorRemoveDuplicates< L, Compare >:: getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::getStaticName()
{

  try
  {

    return "remove-duplicates";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::getStringParameterHelp()
{

  try
  {

    std::string stringHelp = ".  <P1>: bundle distance functor name, one of\n";

    std::set< std::string > 
      names = gkg::BundleDistanceFunctorFactory::getInstance().getNames();
    std::set< std::string >::const_iterator
      fn = names.begin(),
      fne = names.end();
    while ( fn != fne )
    {

      stringHelp += ".        - " + *fn + "\n";
      ++ fn;

    }
    return stringHelp;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::getScalarParameterHelp()
{

  try
  {

    std::string scalarParameterHelp = ".  <P1>: bundle distance threshold\n";

    std::set< std::string > 
      names = gkg::BundleDistanceFunctorFactory::getInstance().getNames();
    std::set< std::string >::const_iterator
      fn = names.begin(),
      fne = names.end();
    while ( fn != fne )
    {

      scalarParameterHelp += ".  for " + *fn + " : \n" +
                             gkg::BundleDistanceFunctorFactory::getInstance().
                               getScalarParameterHelp( *fn ) + "\n";
      ++ fn;

    }

    return scalarParameterHelp;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorRemoveDuplicates< L, Compare >::"
             "getStringParameterHelp()" );

}


#endif
