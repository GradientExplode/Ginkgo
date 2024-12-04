#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorClusteringSelectLabels_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorClusteringSelectLabels_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorClusteringSelectLabels.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>




template < class L, class Compare >
inline
gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::
                                        BundleMapOperatorClusteringSelectLabels(
                             const std::vector< std::string >& /*stringParameters*/,
                             const std::vector< double >& /*scalarParameters*/,
                             bool verbose )
                               : gkg::BundleMapOperator< L, Compare >( verbose )
{

  try
  {

    throw std::runtime_error( "should not enter this code" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::"
             "BundleMapOperatorClusteringSelectLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}



namespace gkg
{


template <>
inline
BundleMapOperatorClusteringSelectLabels< int16_t, std::less< int16_t > >::
                                              BundleMapOperatorClusteringSelectLabels(
                             const std::vector< std::string >& /*stringParameters*/,
                             const std::vector< double >& /*scalarParameters*/,
                             bool verbose )
                               : BundleMapOperator< int16_t,
                                                    std::less< int16_t > >(
                                                                       verbose )
{

  try
  {

    throw std::runtime_error( "not available for bundle maps of label type int16_t" );


  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorClusteringSelectLabels< int16_t, "
             "std::less< int16_t > >::"
             "BundleMapOperatorClusteringSelectLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template <>
inline
BundleMapOperatorClusteringSelectLabels< std::string, std::less< std::string > >::
                                              BundleMapOperatorClusteringSelectLabels(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& scalarParameters,
                             bool verbose )
                           : gkg::BundleMapOperator< std::string,
                                                     std::less< std::string > >(
                                                                       verbose )
{

  try
  {

    // looking whether a ROI label is provided or not
    if ( !stringParameters.empty() )
    {

     _roiLabel = stringParameters[ 0 ];

    }

    // looking whether a length range is provided or not
    if ( !scalarParameters.empty() )
    {

      if ( scalarParameters.size() == 1U )
      {

        _lowerLength = ( float )scalarParameters[ 0 ];
        _upperLength = -1.0f;

      }
      else if ( scalarParameters.size() == 2U )
      {

        _lowerLength = ( float )scalarParameters[ 0 ];
        _upperLength = ( float )scalarParameters[ 1 ];

      }
      else
      {

        throw std::runtime_error( "maximum two scalar parameters allowed" );

      }

    }
    else
    {

      _lowerLength = -1.0f;
      _upperLength = -1.0f;

    }


  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorClusteringSelectLabels< std::string, "
             "std::less< std::string > >::"
             "BundleMapOperatorClusteringSelectLabels( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


}


template < class L, class Compare >
inline
gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::
                                            ~BundleMapOperatorClusteringSelectLabels()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::startContribution(
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


    typename gkg::BundleMap< L, Compare >::const_iterator
      ib = inputBundleMap->begin(),
      ibe = inputBundleMap->end();
    while ( ib != ibe )
    {

      if ( this->check( ib->first ) )
      {

        outputBundleMap->addBundle( ib->first, ib->second );

      }

      ++ ib;

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

     this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::getName() const
{

  try
  {

    return gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::getStaticName()
{

  try
  {

    return "clustering-select-labels";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::getStringParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: ROI label to be selected (right-hemisphere, left-hemisphere, "
     " .        corpus-callosum, or cerebellum) if string type labelling\n"
      ".    .\n"
      ".    .\n"
      ".  <Pn>\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: lower length in mm\n"
      ".  <P2>: upper length in mm\n"
      ".    .\n"
      ".    .\n"
      ".  <Pn>\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
bool
gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::check( const L& /*label*/ ) const
{

  try
  {

    throw std::runtime_error( "should not enter this code" );
    return false;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "bool "
             "gkg::BundleMapOperatorClusteringSelectLabels< L, Compare >::"
             "check( const L& label )" );

}


namespace gkg
{


template <>
inline
bool
gkg::BundleMapOperatorClusteringSelectLabels< std::string, 
                                             std::less< std::string > >::check(
                                               const std::string& label ) const
{

  try
  {

    bool result = false;


    // extracting ROI label and range from input label
    size_t positionOfFirstPipe = label.find( "|" );
    std::string roiLabel = "";

    if ( positionOfFirstPipe < label.length() )
    {

      size_t 
        positionOfSecondPipe = label.find( "|",
                                           positionOfFirstPipe + 1 );

      if ( positionOfSecondPipe < label.length() )
      {

        roiLabel = label.substr( 0, positionOfFirstPipe );

        if ( _lowerLength < 0.0f )
        {

          if ( roiLabel == _roiLabel )
          {

            result = true;

          }

        }
        else
        {

          std::string rangeLengthString = label.substr(
                                    positionOfFirstPipe + 1,
                                    positionOfSecondPipe -
                                    positionOfFirstPipe - 1 );

          std::string lowerLengthString = rangeLengthString.substr(
                                               0,
                                               rangeLengthString.find( "m" ) );

          float lowerLength = 0.0;
          if ( !gkg::StringConverter::toScalar( lowerLengthString,
                                                lowerLength ) )
          {

            throw std::runtime_error(
                       "unable to convert lower length string to scalar value" );

          }

          if ( _upperLength < 0.0f )
          {

            if ( ( roiLabel == _roiLabel ) &&
                 ( lowerLength >= _lowerLength ) )
            {

              result = true;

            }

          }
          else
          {

            std::string upperLengthString = rangeLengthString.substr(
                                               rangeLengthString.find( "-" ) + 1,
                                               rangeLengthString.length() - 
                                               rangeLengthString.find( "-" ) - 3 );
            float upperLength = 0.0;
            if ( !gkg::StringConverter::toScalar( upperLengthString,
                                                  upperLength ) )
            {
  
              throw std::runtime_error(
                       "unable to convert upper length string to scalar value" );

            }
            if ( ( roiLabel == _roiLabel ) &&
                 ( lowerLength >= _lowerLength ) &&
                 ( upperLength <= _upperLength ) )
            {

              result = true;

            }

          }

        }

      }
      else
      {

        throw std::runtime_error(
                             "unable to find a second pipe character in label" );

      }

    }
    else
    {

      throw std::runtime_error( "unable to find pipe character in label" );

    }

    return result;

  }
  GKG_CATCH( "template <> "
             "inline "
             "bool "
             "gkg::BundleMapOperatorClusteringSelectLabels< std::string, "
             "std::less< std::string > >::check( "
             "const std::string& label ) const" );

}


}


#endif
