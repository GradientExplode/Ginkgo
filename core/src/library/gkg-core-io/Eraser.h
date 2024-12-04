#ifndef _gkg_core_io_Eraser_h_
#define _gkg_core_io_Eraser_h_


#include <gkg-core-cppext/StdInt.h>
#include <iostream>
#include <string>


namespace gkg
{


class Eraser
{

  public:

    Eraser( int32_t count );

    const std::string& getString() const;

  private:

    std::string _string;

};


}


namespace std
{


ostream& operator << ( ostream& os, const gkg::Eraser& thing );


}

#endif
