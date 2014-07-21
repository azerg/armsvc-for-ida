#pragma once

#include <string>
#include <unordered_map>

typedef std::unordered_map<int, std::string> UnistdApisMap;
typedef UnistdApisMap::iterator UnistdApisMapIt;
typedef std::pair<int, std::string> UnistdApiPair;

bool LoadUnistdHeader( __out UnistdApisMap& outMap );
bool ParseLine( std::string& strLine, __out UnistdApiPair& outPair );
std::string GetUnistdPath();