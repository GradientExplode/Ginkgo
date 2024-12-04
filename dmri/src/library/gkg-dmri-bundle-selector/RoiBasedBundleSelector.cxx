#include <gkg-dmri-bundle-selector/RoiBasedBundleSelector.h>
#include <gkg-processing-container/Volume_i.h>
#include <gkg-processing-algobase/TestFunction_i.h>
#include <gkg-core-exception/Exception.h>


template < class L >
gkg::RoiBasedBundleSelector< L >::RoiBasedBundleSelector(
                          gkg::RCPointer< gkg::Volume< L > > rois,
                          gkg::RCPointer< gkg::Transform3d< float > >
                                transform3dFromRoisVoxelToFiberReal,
                          gkg::RCPointer< gkg::TestFunction< L > > testFunction,
                          gkg::RoiBasedBundleSelector< L >::Type type,
                          float minimumIntersectionPercentagePerFiber,
                          float minimumIntersectingFiberPercentage )
             : gkg::BundleSelector(),
               _roiBasedFiberSelector(
                 rois,
                 transform3dFromRoisVoxelToFiberReal,
                 testFunction,
                 ( ( type == gkg::RoiBasedBundleSelector< L >::Intersection ) ?
                   gkg::RoiBasedFiberSelector< L >::Intersection :
                   gkg::RoiBasedFiberSelector< L >::NoIntersection ),
                 minimumIntersectionPercentagePerFiber ),
               _type( type ),
               _minimumIntersectingFiberPercentage(
                 minimumIntersectingFiberPercentage )
{
}


template < class L >
gkg::RoiBasedBundleSelector< L >::~RoiBasedBundleSelector()
{
}



template < class L >
const typename gkg::RoiBasedFiberSelector< L > 
gkg::RoiBasedBundleSelector< L >::getRoiBasedFiberSelector() const
{

  try
  {

    return _roiBasedFiberSelector;

  }
  GKG_CATCH( "template < class L > "
             "const typename gkg::RoiBasedFiberSelector< L > "
             "gkg::RoiBasedBundleSelector< L >::getRoiBasedFiberSelector() "
             "const" );

}


template < class L >
bool gkg::RoiBasedBundleSelector< L >::select(
        const typename gkg::RoiBasedBundleSelector< L >::Bundle& bundle ) const
{

  try
  {

    int32_t intersectingFiberCount = 0;
    typename gkg::RoiBasedBundleSelector< L >::Bundle::const_iterator
       f = bundle.begin(),
       fe = bundle.end();
    while ( f != fe )
    {

      if ( _roiBasedFiberSelector.select( *f ) )
      {

        ++ intersectingFiberCount;

      }
      ++ f;

    }

    bool result = false;
    if ( _type == gkg::RoiBasedBundleSelector< L >::Intersection )
    {

      if ( !intersectingFiberCount )
      {

        result = false;

      }
      else
      {

        float percent = ( float )intersectingFiberCount * 100.0 /
                        ( float )bundle.getCurve3dCount();
        if ( percent > _minimumIntersectingFiberPercentage )
        {

         result = true;

        }
        else
        {

          result = false;

        }

      }

    }
    else
    {

      if ( !intersectingFiberCount )
      {

        result = true;

      }
      else
      {

        result = false;

      }

    }

    return result;

  }
  GKG_CATCH( "template < class L > "
             "bool gkg::RoiBasedBundleSelector< L >::select( "
             "const typename gkg::RoiBasedBundleSelector< L >::Bundle& "
             "bundle ) const" );

}


template class gkg::RoiBasedBundleSelector< int8_t >;
template class gkg::RoiBasedBundleSelector< uint8_t >;
template class gkg::RoiBasedBundleSelector< int16_t >;
template class gkg::RoiBasedBundleSelector< uint16_t >;
template class gkg::RoiBasedBundleSelector< int32_t >;
template class gkg::RoiBasedBundleSelector< uint32_t >;
template class gkg::RoiBasedBundleSelector< int64_t >;
template class gkg::RoiBasedBundleSelector< uint64_t >;
template class gkg::RoiBasedBundleSelector< float >;
template class gkg::RoiBasedBundleSelector< double >;

