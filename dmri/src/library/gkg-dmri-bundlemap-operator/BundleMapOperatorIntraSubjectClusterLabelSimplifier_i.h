#ifndef _gkg_dmri_bundlemap_operator_BundleMapOperatorIntraSubjectClusterLabelSimplifier_i_h_
#define _gkg_dmri_bundlemap_operator_BundleMapOperatorIntraSubjectClusterLabelSimplifier_i_h_


#include <gkg-dmri-bundlemap-operator/BundleMapOperatorIntraSubjectClusterLabelSimplifier.h>
#include <gkg-dmri-bundlemap-operator/BundleMapOperator_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/Exception.h>
#include <fstream>



template < class L, class Compare >
inline
gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, Compare >::
                            BundleMapOperatorIntraSubjectClusterLabelSimplifier(
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
             "gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, Compare >::"
             "BundleMapOperatorIntraSubjectClusterLabelSimplifier( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}



namespace gkg
{


template <>
inline
BundleMapOperatorIntraSubjectClusterLabelSimplifier< int16_t, 
                                                     std::less< int16_t > >::
                            BundleMapOperatorIntraSubjectClusterLabelSimplifier(
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
             "BundleMapOperatorIntraSubjectClusterLabelSimplifier< int16_t, "
             "std::less< int16_t > >::"
             "BundleMapOperatorIntraSubjectClusterLabelSimplifier( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


template <>
inline
BundleMapOperatorIntraSubjectClusterLabelSimplifier<
                                                    std::string,
                                                    std::less< std::string > >::
                            BundleMapOperatorIntraSubjectClusterLabelSimplifier(
                             const std::vector< std::string >& stringParameters,
                             const std::vector< double >& /*scalarParameters*/,
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

     _fileNameLabelLut = stringParameters[ 0 ];

    }

    _currentRoiIndex = 1;

  }
  GKG_CATCH( "template <> "
             "inline "
             "BundleMapOperatorIntraSubjectClusterLabelSimplifier< std::string, "
             "std::less< std::string > >::"
             "BundleMapOperatorIntraSubjectClusterLabelSimplifier( "
             "const std::vector< std::string >& stringParameters, "
             "const std::vector< double >& scalarParameters, "
             "bool verbose )" );

}


}


template < class L, class Compare >
inline
gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, Compare >::
                          ~BundleMapOperatorIntraSubjectClusterLabelSimplifier()
{
}



template < class L, class Compare >
inline
void gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, Compare >::
                                                              startContribution(
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

      outputBundleMap->addBundle( this->getNewLabel( ib->first ), ib->second );

      ++ ib;

    }

    if ( !_fileNameLabelLut.empty() )
    {

      std::ofstream os( _fileNameLabelLut.c_str() );

      std::map< std::string, int32_t >::const_iterator
        r = _roiToIndexLut.begin(),
        re = _roiToIndexLut.end();
      while ( r != re )
      {

        os << r->first << " " << r->second << std::endl;
        ++ r;

      }


      std::map< std::string, std::string >::const_iterator
        c = _clusterLabelToNewLabelLut.begin(),
        ce = _clusterLabelToNewLabelLut.end();
      while ( c != ce )
      {

        os << c->first << " " << c->second << std::endl;
        ++ c;

      }

      os.close();

    }

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L,"
             " Compare >::"
             "startContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, Compare >::
                                                                addContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L,"
             " Compare >::"
             "addContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
void gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, Compare >::
                                                                endContribution(
                gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap,
                gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap )
{

  try
  {

    this->startContribution( inputBundleMap, outputBundleMap );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L,"
             " Compare >::"
             "endContribution( "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > inputBundleMap, "
             "gkg::RCPointer< gkg::BundleMap< L, Compare > > outputBundleMap "
             ") const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, Compare >::
                                                                 getName() const
{

  try
  {

    return gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L,
                                                     Compare >::getStaticName();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, "
             "Compare >::"
             "getName() const" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, Compare >::
                                                                 getStaticName()
{

  try
  {

    return "intra-subject-cluster-label-simplifier";

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, "
             "Compare >::"
             "getStaticName()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, Compare >::
                                                        getStringParameterHelp()
{

  try
  {

    return std::string(
      ".  <P1>: output ROI and label simplification LUT filename\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, "
             "Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, Compare >::
                                                        getScalarParameterHelp()
{

  try
  {

    return std::string(
      ".  N/A\n" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, "
             "Compare >::"
             "getStringParameterHelp()" );

}


template < class L, class Compare >
inline
L
gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, Compare >::
                                               getNewLabel( const L& /*label*/ )
{

  try
  {

    throw std::runtime_error( "should not enter this code" );
    L l;
    return l;

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< L, "
             "Compare >::"
             "getNewLabel( const L& /*label*/ )" );

}


namespace gkg
{


template <>
inline
std::string
gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< std::string, 
                                             std::less< std::string > >::
                                         getNewLabel( const std::string& label )
{

  try
  {

    // extracting ROI label and range from input label
    size_t positionOfFirstPipe = label.find( "|" );

    std::string newLabel;

    if ( positionOfFirstPipe < label.length() )
    {

      std::string roiLabel = label.substr( 0, positionOfFirstPipe );
      if ( _roiToIndexLut.find( roiLabel ) == _roiToIndexLut.end() )
      {

        _roiToIndexLut[ roiLabel ] = _currentRoiIndex;
        _currentClusterIndices[ _currentRoiIndex ] = 1;
        ++ _currentRoiIndex;

      }
      else
      {

        ++ _currentClusterIndices[  _roiToIndexLut[ roiLabel ] ];

      }
      

      std::string newRoiLabel = gkg::StringConverter::toString(
                                                   _roiToIndexLut[ roiLabel ] );
      std::string newClusterIndex = gkg::StringConverter::toString(
                         _currentClusterIndices[ _roiToIndexLut[ roiLabel ] ] );

      uint32_t newClusterIndexSize = newClusterIndex.length();
      std::string prefix = "000000";
      newLabel = newRoiLabel + '|' +
                 prefix.substr( 0, 6U - newClusterIndexSize ) +
                 newClusterIndex;
      _clusterLabelToNewLabelLut[ label ] = newLabel;

    }
    else
    {

      throw std::runtime_error( "bad input label format" );

    }

    return newLabel;

  }
  GKG_CATCH( "template <> "
             "inline "
             "std::string "
             "gkg::BundleMapOperatorIntraSubjectClusterLabelSimplifier< "
             "std::string, "
             "std::less< std::string > >::getNewLabel( "
             "const std::string& label )" );

}


}


#endif
