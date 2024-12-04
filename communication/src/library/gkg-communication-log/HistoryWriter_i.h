#ifndef _gkg_communication_log_HistoryWriter_i_h_
#define _gkg_communication_log_HistoryWriter_i_h_


#include <gkg-communication-log/HistoryWriter.h>
#include <gkg-core-exception/Exception.h>


template < class T >
void gkg::HistoryWriter::add( const std::string& text,
                              T value,
                              bool newLine )
{

  try
  {

    if ( _active && !_fileName.empty() )
    {

      checkAndCreateDirectory();

      std::ofstream fs( _fileName.c_str(), std::ios_base::app );

      if ( fs.good() )
      {

        addDate( fs );

        if ( !text.empty() )
        {

          fs << text << " ";

        }
        
        fs << value << " ";

        if ( newLine )
        {

          fs << std::endl;

        }

        fs.close();

      }

    }

  }
  GKG_CATCH( "void gkg::HistoryWriter::add( const std::string& text, "
             "T value, bool newLine )" );

}


#endif
