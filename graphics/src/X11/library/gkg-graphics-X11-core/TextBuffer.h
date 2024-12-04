#ifndef _gkg_graphics_X11_core_TextBuffer_h_
#define _gkg_graphics_X11_core_TextBuffer_h_


#include <gkg-core-cppext/StdInt.h>
#include <string>


namespace gkg
{


class RegularExpression;


// editable text buffer
class TextBuffer
{

  public:

    enum IOResult
    {

      OpenError,
      MemoryError,
      ReadError,
      ReadOk,
      WriteError,
      WriteOk

    };

    TextBuffer( const std::string& text = "" );
    virtual ~TextBuffer();

    IOResult load( const std::string& path );
    IOResult save();
    IOResult save(const std::string& path );

    int32_t search( RegularExpression* regularExpression,
                    int32_t index, int32_t range, int32_t stop ) const;
    int32_t forwardSearch( RegularExpression* regularExpression, 
                           int32_t index ) const;
    int32_t backwardSearch( RegularExpression* regularExpression, 
                            int32_t index ) const;
    int32_t forwardSearch( const std::string& pattern, int32_t index );
    int32_t backwardSearch( const std::string& pattern, int32_t index );
    int32_t getSearchBeginning();
    int32_t getSearchEnd();

    int32_t match( RegularExpression* regularExpression,
                   int32_t index, int32_t stop ) const;
    bool forwardMatch( RegularExpression* regularExpression, 
                       int32_t index ) const;
    bool backwardMatch( RegularExpression* regularExpression, 
                        int32_t index ) const;

    virtual int32_t insert( int32_t index, const std::string& text );
    virtual int32_t erase( int32_t index, int32_t length );
    int32_t copy( int32_t index, std::string& buffer, int32_t length ) const;

    int32_t getHeight() const;
    int32_t getWidth() const;
    int32_t getLength() const;

    const std::string& getText() const;
    std::string getText( int32_t index ) const;
    std::string getText( int32_t index1, int32_t index2 ) const;
    char getChar( int32_t index ) const;

    int32_t getLineIndex( int32_t line );
    int32_t getLinesBetween( int32_t index1, int32_t index2 ) const;
    int32_t getLineNumber( int32_t index );
    int32_t getLineOffset( int32_t index ) const;

    int32_t getPreviousCharacter( int32_t index) const;
    int32_t getNextCharacter( int32_t index ) const;

    bool isBeginningOfText( int32_t index ) const;
    int32_t getBeginningOfText() const;

    bool isEndOfText( int32_t index ) const;
    int32_t getEndOfText() const;

    bool isBeginningOfLine( int32_t index ) const;
    int32_t getBeginningOfLine( int32_t index ) const;
    int32_t getBeginningOfNextLine( int32_t index ) const;

    bool isEndOfLine( int32_t index ) const;
    int32_t getEndOfLine( int32_t index ) const;
    int32_t getEndOfPreviousLine( int32_t index ) const;

    bool isBeginningOfWord( int32_t index ) const;
    int32_t getBeginningOfWord( int32_t index ) const;
    int32_t getBeginningOfNextWord( int32_t index ) const;

    bool isEndOfWord( int32_t index ) const;
    int32_t getEndOfWord( int32_t index ) const;
    int32_t getEndOfPreviousWord( int32_t index ) const;

    bool isBufferModified() const;
    void rightTrim();				     

  protected:

    std::string _text;

  private:

    std::string _path;
    bool _modified;
    int32_t _lineCount;
    int32_t _lastLine;
    int32_t _lastIndex;
    RegularExpression* _currentRegularExpression;

};


}


#endif
