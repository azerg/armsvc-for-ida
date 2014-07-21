#include "stdafx.h"
#include "UnistdLoader.h"
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

inline std::string trim(std::string& str)
{
  str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
  str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
  return str;
}

std::string GetUnistdPath()
{
  std::string strPath;
  static char tmpBuff[MAX_PATH];
  const char unistdFileName[] = "plugins\\ArmSVC\\unistd.h";

  GetModuleFileNameA( GetModuleHandleA("ArmSVC.dll"), tmpBuff, MAX_PATH );

  fs::path unistdPath( tmpBuff );

  unistdPath = unistdPath.parent_path();
  unistdPath /= unistdFileName;

  return unistdPath.string().c_str();
}

bool LoadUnistdHeader( __out UnistdApisMap& outMap )
{
  std::ifstream infile( GetUnistdPath() );
  outMap.clear();

  if ( !infile.good() )
    return false;

  UnistdApiPair outPair;
  for( std::string line; std::getline( infile, line ); )
  {
      if ( ParseLine( line, outPair ) )
      {
        outMap.insert( outPair );
      }
  }

  return true;
}

bool ParseLine( std::string& strLine, __out UnistdApiPair& outPair )
{
  trim( strLine );
  if ( strLine.length() <= 9 )
    return false;

  const char DEFINITION [] = "#define ";

  size_t NameOffset = strLine.find( DEFINITION );
  if ( NameOffset == std::string::npos )
    return false;
  
  int i = NameOffset + sizeof(DEFINITION);

  size_t valueOffset = strLine.find( ' ', NameOffset + sizeof(DEFINITION) );
  if ( valueOffset == std::string::npos )
  {
    valueOffset = strLine.find( '\t', NameOffset + sizeof(DEFINITION) );
    if ( valueOffset == std::string::npos )
      return false;
  }

  outPair.second = strLine.substr( NameOffset + sizeof(DEFINITION), valueOffset - NameOffset - sizeof(DEFINITION) );
  trim( outPair.second );

  size_t tabPos = 0;
  do
  {
    tabPos = outPair.second.rfind( '\t' );
    if ( tabPos != std::string::npos )
      outPair.second.resize( tabPos );

  } while ( tabPos != std::string::npos );

  static char valueBuff [256];
  size_t length = strLine.copy( &valueBuff[0], strLine.length() - valueOffset - 1, valueOffset + 1 );

  int ival = atoi( valueBuff );
  ZeroMemory(valueBuff, length);

  outPair.first = ival;

  return true;
}