#include "FontAtlasGenerator.hpp"
#include "SDFGenerator.hpp"

#include <Crimild.hpp>

using namespace crimild;
using namespace crimild::tools::fontgen;

std::string extractFontName( std::string input )
{
	std::string filename = input.substr( input.find_last_of( "/" ) + 1 );
	std::string fontName = filename.substr( 0, filename.find_last_of( "." ) );
	return fontName;
}

int main( int argc, char **argv )
{
	Settings settings;
	settings.parseCommandLine( argc, argv );

	std::string fontFile = settings.get( "font_file", "" );
	if ( fontFile == "" ) {
		std::cout << "usage: " << argv[ 0 ] << " font_file=path/to/font.ttf [texture_size=number]" << std::endl;
		return -1;
	}

	std::cout << fontFile << std::endl;

	std::string fontName = extractFontName( fontFile );
	int size = settings.get( "texture_size", 1024 );

	FontAtlasGenerator fontAtlasGenerator( fontFile, size, size );
	if ( fontAtlasGenerator.execute( fontName ) ) {
#ifdef __APPLE__		
		system( ( std::string( "open \"" ) + fontName + ".tga\"" ).c_str() );
		system( ( std::string( "open \"" ) + fontName + ".txt\"" ).c_str() );
#endif
	}

	SDFGenerator sdfGenerator;
	sdfGenerator.execute( fontName + ".tga" );

	return 0;
}

