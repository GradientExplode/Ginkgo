#ifndef _gkg_dmri_io_BundleMeasureSpreadSheetDiskFormat_i_h_
#define _gkg_dmri_io_BundleMeasureSpreadSheetDiskFormat_i_h_


#include <gkg-dmri-io/BundleMeasureSpreadSheetDiskFormat.h>
#include <gkg-communication-sysinfo/ConfigurationInfo.h>
#include <gkg-core-io/TypedDiskFormat_i.h>
#include <gkg-core-io/ByteOrder.h>
#include <gkg-core-io/IOMode.h>
#include <gkg-core-io/DefaultItemIOFactory.h>
#include <gkg-dmri-container/BundleMeasureSpreadSheet_i.h>
#include <gkg-core-io/TypeOf.h>
#include <gkg-core-exception/FileNotFoundException.h>
#include <gkg-core-exception/ObjectTypeException.h>
#include <gkg-core-exception/ItemTypeException.h>
#include <gkg-core-exception/NotOpenException.h>
#include <gkg-core-object/Dictionary.h>
#include <gkg-core-object/GenericObject_i.h>
#include <gkg-core-exception/Exception.h>
#include <vector>
#include <fstream>


template < class L, class Compare >
inline
gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::
                                             BundleMeasureSpreadSheetDiskFormat()
                                                      : gkg::TypedDiskFormat<
                                  gkg::BundleMeasureSpreadSheet< L, Compare > >()
{

  this->_extensions.insert( ".bundlemeasure_spreadsheet" );
  this->_extensions.insert( getHeaderExtension() );

}


template < class L, class Compare >
inline
gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::
                                           ~BundleMeasureSpreadSheetDiskFormat()
{
}


template < class L, class Compare >
inline
std::string 
gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::getName() const
{

  return "bundlemeasure_spreadsheet";

}


template < class L, class Compare >
inline
void gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::readHeader(
                                             const std::string& name,
                                             gkg::HeaderedObject& object ) const
{

  try
  {

    // reading the meta-information header
    try
    {

      this->gkg::DiskFormat::readHeader( name, object );

    }
    catch ( gkg::FileNotFoundException& e )
    {

      // std::cerr << e.what() << ", "
      //           << "meta-information not present"
      //           << std::endl;

    }
    catch ( std::exception& e )
    {

      throw std::runtime_error( e.what() );

    }

    // checking that it is a fibre measure spreadsheet
    std::string objectType;
    object.getHeader().getAttribute( "object_type", objectType );
    if ( objectType != "BundleMeasureSpreadSheet" )
    {

      throw gkg::ObjectTypeException( this->getNameWithoutExtension( name ) +
                                      getHeaderExtension() );

    }

    // checking that it has the good label type
    std::string labelType;
    object.getHeader().getAttribute( "label_type", labelType );
    if ( ! gkg::TypeOf< L >::isSameType( labelType ) )
    {

      throw gkg::ItemTypeException( this->getNameWithoutExtension( name ) +
                                    getHeaderExtension() );

    }

    // updating observer(s)
    object.getHeader().setChanged();
    object.getHeader().notifyObservers();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::"
             "readHeader( "
             "const std::string& name, gkg::HeaderedObject& object ) const" );

}


template < class L, class Compare >
inline
void gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::writeHeader(
                                       const std::string& name,
                                       const gkg::HeaderedObject& object ) const
{

  // writing the meta-information header
  try
  {

    this->gkg::DiskFormat::writeHeader( name, object );

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::"
             "writeHeader( "
             "const std::string& name, const gkg::HeaderedObject& object ) "
             "const" );

}


template < class L, class Compare >
inline
void gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::read(
                     const std::string& name,
                     gkg::BundleMeasureSpreadSheet< L, Compare >& object ) const
{

  try
  {

    // reading meta-information
    readHeader( name, object ); 

    // reading core data
    std::ifstream
      is( ( this->getNameWithoutExtension( name ) +
            ".bundlemeasure_spreadsheet" ).c_str() );
    if ( !is )
    {

      throw gkg::FileNotFoundException(
        this->getNameWithoutExtension( name ) + ".bundlemeasure_spreadsheet" );

    }

    L label;
    std::string measureName;
    int32_t size1 = 0;
    int32_t size2 = 0;
    int32_t s1 = 0;
    int32_t s2 = 0;
    L labelForCheck;
    std::string measureNameForCheck;
    int32_t size1ForCheck = 0;
    int32_t size2ForCheck = 0;
    while ( !is.eof() )
    {

      is >> label;
      is >> measureName;
      is >> size1;
      is >> size2;

      gkg::Matrix measureMatrix( size1, size2 );

      for ( s1 = 0; s1 < size1; s1++ )
      {

        // skip/check first columns if not the first line
        if ( s1 )
        {

          is >> labelForCheck;
          is >> measureNameForCheck;
          is >> size1ForCheck;
          is >> size2ForCheck;
          if ( ( labelForCheck != label ) ||
               ( measureNameForCheck != measureName ) ||
               ( size1ForCheck != size1 ) ||
               ( size2ForCheck != size2 ) )
          {

             throw std::runtime_error(
                       "bad label / fiber index / measure name / matrix size" );

          }

        }
        for ( s2 = 0; s2 < size2; s2++ )
        {

          is >> measureMatrix( s1, s2 );

        }

      }
      object.addMeasure( label, measureName, measureMatrix );

    }

    is.close();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::"
             "read( "
             "const std::string& name, "
             "gkg::BundleMeasureSpreadSheet< L, Compare >& object ) const" );

}


template < class L, class Compare >
inline
void gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::write(
                            const std::string& name,
                            gkg::BundleMeasureSpreadSheet< L, Compare >& object,
                            bool /*ascii*/ ) const
{

  try
  {

    // writing header(s) meta-information
    writeHeader( name, object );


    // writing core data <*.fibermeasure_spreadsheet>
    std::ofstream os( ( this->getNameWithoutExtension( name ) +
                        ".bundlemeasure_spreadsheet" ).c_str() );
    if ( !os )
    {

      throw gkg::NotOpenException( this->getNameWithoutExtension( name ) +
                                   ".bundlemeasure_spreadsheet" );

    }
    std::string 
      separator = gkg::ConfigurationInfo::getInstance().getCSVSeparator();

    // looping over bundles
    typename 
      std::map< L,
                std::map< std::string, Matrix >,
                Compare >::const_iterator
      l = object.begin(),
      le = object.end();
    while ( l != le )
    {

      // getting label and measure map
      L label = l->first;
      const std::map< std::string, gkg::Matrix >&
        measureMap = l->second;

      // looping over measures
       std::map< std::string, gkg::Matrix >::const_iterator
        m = measureMap.begin(),
        me = measureMap.end();
      while ( m != me )
      {

        // getting measure name and matrix
        const std::string& measureName = m->first;
        const gkg::Matrix& measureMatrix = m->second;

        // looping over matrix items
        int32_t size1 = measureMatrix.getSize1();
        int32_t size2 = measureMatrix.getSize2();
        int32_t s1 = 0;
        int32_t s2 = 0;
        for ( s1 = 0; s1 < size1; s1++ )
        {

          os << label << separator
             << measureName << separator
             << size1 << separator
             << size2 << separator;

          for ( s2 = 0; s2 < size2 - 1; s2++ )
          {

            os << measureMatrix( s1, s2 ) << separator;

          }
          os << measureMatrix( s1, s2 ) << std::endl;

        }

        ++ m;

      }

      ++ l;

    }

    os.close();

  }
  GKG_CATCH( "template < class L, class Compare > "
             "inline "
             "void gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::"
             "write( const std::string& "
             "name, gkg::BundleMeasureSpreadSheet< L, Compare >& object, "
             "bool ascii ) const" );

}


template < class L, class Compare >
inline
bool gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::hasReader() const
{

  return true;

}


template < class L, class Compare >
inline
bool gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::hasWriter() const
{

  return true;

}


template < class L, class Compare >
inline
std::string 
gkg::BundleMeasureSpreadSheetDiskFormat< L, Compare >::getHeaderExtension() const
{

  return std::string( ".bundlemeasure_spreadsheet" ) + 
         gkg::ConfigurationInfo::getInstance().getHeaderExtensionName();

}


#endif
