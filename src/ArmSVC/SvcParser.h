#pragma once

#include "UnistdLoader.h"


class SvcParser
{
public:
  SvcParser( UnistdApisMap&& apisMap ):
    m_apisMap( apisMap )
  {
  };
  bool ProcessFunction( func_t* pFunc );

private:
  UnistdApisMap m_apisMap;
};

