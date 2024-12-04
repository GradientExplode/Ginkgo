#include <gkg-dmri-plugin-vtk/VtkBundleMapDiskFormat.h>
#include <gkg-core-io/DiskFormatFactory_i.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-dmri-container/BundleMap_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-exception/Exception.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkPolyDataWriter.h>


template < class L, class Compare >
gkg::VtkBundleMapDiskFormat< L, Compare >::VtkBundleMapDiskFormat()
                                          : gkg::TypedDiskFormat<
                                            gkg::BundleMap< L, Compare > >()
{

  this->_extensions.insert( ".fib" );

}


template < class L, class Compare >
gkg::VtkBundleMapDiskFormat< L, Compare >::~VtkBundleMapDiskFormat()
{
}


template < class L, class Compare >
std::string gkg::VtkBundleMapDiskFormat< L, Compare >::getName() const
{

  return "vtkbundlemap";

}


template < class L, class Compare >
void gkg::VtkBundleMapDiskFormat< L, Compare >::readHeader(
                                             const std::string& /*name*/,
                                             gkg::HeaderedObject& /*object*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::VtkBundleMapDiskFormat< L, Compare >::readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


template < class L, class Compare >
void gkg::VtkBundleMapDiskFormat< L, Compare >::writeHeader(
                                       const std::string& /*name*/,
                                       const gkg::HeaderedObject& /*object*/ ) const
{

  // no header in VTK BundleMap disk format

}


template < class L, class Compare >
void gkg::VtkBundleMapDiskFormat< L, Compare >::read(
                                   const std::string& /*name*/,
                                   gkg::BundleMap< L, Compare >& /*object*/ ) const
{

  try
  {

    throw std::runtime_error( "not implemented" );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::VtkBundleMapDiskFormat< L, Compare >::read( "
             "const std::string& /*name*/, "
             "gkg::BundleMap< L, Compare >& /*object*/ ) const" );

}


template < class L, class Compare >
void gkg::VtkBundleMapDiskFormat< L, Compare >::write(
                                         const std::string& name,
                                         gkg::BundleMap< L, Compare >& object,
                                         bool /*ascii*/ ) const
{

  try
  {

    // allocating VTK polydata and points
    vtkPolyData* vtkPolyData = vtkPolyData::New();
    vtkPoints* vtkPoints = vtkPoints::New();

    vtkPolyData->Initialize();
    vtkPolyData->Allocate();

    // looping over bundle(s)
    typename gkg::BundleMap< L, Compare >::const_iterator
      b = object.begin(),
      be = object.end();
    int32_t pointIndex = 0;
    while ( b != be )
    {

      // looping over 3D curve(s)
      typename gkg::BundleMap< L, Compare >::Bundle::const_iterator
        c = b->second.begin(),
        ce = b->second.end();
      while ( c != ce )
      {

        // allocating id type(s) for current 3D curve
        vtkIdType* vtkIdTypes = new vtkIdType[ c->getPointCount() ];

        // looping over 3D point(s)
        gkg::Curve3d< float >::const_iterator
          p = c->begin(),
          pe = c->end();
        pointIndex = 0;
        while ( p != pe )
        {

          // writing current 3D point
          vtkIdTypes[ pointIndex ] = vtkPoints->InsertNextPoint( p->x,
                                                                 p->y,
                                                                 p->z );
          ++ p;
          ++ pointIndex;

        }

        // adding polyline to VTK polydata
        vtkPolyData->InsertNextCell( VTK_POLY_LINE,
                                     c->getPointCount(),
                                     vtkIdTypes );
        delete [] vtkIdTypes;

        ++ c;

      }
      ++ b;

    }

    // adding points
    vtkPolyData->SetPoints( vtkPoints );

    // writing VTK file
    vtkPolyDataWriter* vtkPolyDataWriter = vtkPolyDataWriter::New();
    
    
#if VTK_MAJOR_VERSION >= 6
    
    vtkPolyDataWriter->SetInputData( vtkPolyData );
    
#else

    vtkPolyDataWriter->SetInput( vtkPolyData );

#endif

    vtkPolyDataWriter->SetFileName( ( this->getNameWithoutExtension( name ) +
                                      ".fib" ).c_str() );
    vtkPolyDataWriter->Update();

    // deleting VTK structure(s)
    vtkPolyDataWriter->Delete();
    vtkPoints->Delete();
    vtkPolyData->Delete();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "void gkg::VtkBundleMapDiskFormat<L, Compare >::write( "
             "const std::string& "
             "name, gkg::BundleMap< L, Compare >& object, "
             "bool /*ascii*/ ) const" );

}


template < class L, class Compare >
bool gkg::VtkBundleMapDiskFormat< L, Compare >::hasReader() const
{

  return false;

}


template < class L, class Compare >
bool gkg::VtkBundleMapDiskFormat< L, Compare >::hasWriter() const
{

  return true;

}


//
// VTK BundleMap disk format singleton instanciation
//

template class gkg::VtkBundleMapDiskFormat< int8_t >;
template class gkg::VtkBundleMapDiskFormat< uint8_t >;
template class gkg::VtkBundleMapDiskFormat< int16_t >;
template class gkg::VtkBundleMapDiskFormat< uint16_t >;
template class gkg::VtkBundleMapDiskFormat< int32_t >;
template class gkg::VtkBundleMapDiskFormat< uint32_t >;
template class gkg::VtkBundleMapDiskFormat< int64_t >;
template class gkg::VtkBundleMapDiskFormat< uint64_t >;
template class gkg::VtkBundleMapDiskFormat< std::string >;


//
// registrating VTK BundleMap disk format for "BundleMap" object factory
//

static bool initialize()
{

  try
  {

    gkg::DiskFormatFactory< gkg::BundleMap< int8_t > 
                         >::getInstance().registerDiskFormat(
         gkg::VtkBundleMapDiskFormat< int8_t >::getInstance().getName(),
         &gkg::VtkBundleMapDiskFormat< int8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< uint8_t > 
                         >::getInstance().registerDiskFormat(
         gkg::VtkBundleMapDiskFormat< uint8_t >::getInstance().getName(),
         &gkg::VtkBundleMapDiskFormat< uint8_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< int16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::VtkBundleMapDiskFormat< int16_t >::getInstance().getName(),
         &gkg::VtkBundleMapDiskFormat< int16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< uint16_t > 
                         >::getInstance().registerDiskFormat(
         gkg::VtkBundleMapDiskFormat< uint16_t >::getInstance().getName(),
         &gkg::VtkBundleMapDiskFormat< uint16_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< int32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::VtkBundleMapDiskFormat< int32_t >::getInstance().getName(),
         &gkg::VtkBundleMapDiskFormat< int32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< uint32_t > 
                         >::getInstance().registerDiskFormat(
         gkg::VtkBundleMapDiskFormat< uint32_t >::getInstance().getName(),
         &gkg::VtkBundleMapDiskFormat< uint32_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< int64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::VtkBundleMapDiskFormat< int64_t >::getInstance().getName(),
         &gkg::VtkBundleMapDiskFormat< int64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< uint64_t > 
                         >::getInstance().registerDiskFormat(
         gkg::VtkBundleMapDiskFormat< uint64_t >::getInstance().getName(),
         &gkg::VtkBundleMapDiskFormat< uint64_t >::getInstance() );
    gkg::DiskFormatFactory< gkg::BundleMap< std::string > 
                         >::getInstance().registerDiskFormat(
         gkg::VtkBundleMapDiskFormat< std::string >::getInstance().getName(),
         &gkg::VtkBundleMapDiskFormat< std::string >::getInstance() );

    return true;

  }
  GKG_CATCH( "VTK BundleMap disk format registration: " )

}

static bool initialized __attribute__((unused)) = initialize();
