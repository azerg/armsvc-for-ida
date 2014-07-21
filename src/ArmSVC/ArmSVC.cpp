// ArmSVC.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ArmSVC.h"
#include "SvcParser.h"
#include "UnistdLoader.h"
#include <memory>

std::unique_ptr<SvcParser> g_pSvcParser;

int __stdcall IDAP_init()
{
    if ( inf.filetype != f_ELF )
        return PLUGIN_SKIP;

    UnistdApisMap apisMap;
    if ( !LoadUnistdHeader(apisMap) )
    {
      msg( "ArmSVC plugin initialization error: Error loading unistd.h from plugin\\ArmSVC\\unistd.h\n" );
      return PLUGIN_SKIP;
    }

    g_pSvcParser.reset( new SvcParser( std::move(apisMap) ) ); 

    return PLUGIN_KEEP;
}

void __stdcall IDAP_term()
{
}

void __stdcall IDAP_run(int arg)
{	
    size_t ceFuncts = get_func_qty();
    msg( "Total functs: %d\n", ceFuncts );
    msg( "-----------------------------------------------\n" );

    for ( size_t curFunctionId = 0; curFunctionId < ceFuncts; ++curFunctionId )
    {
      func_t* pCurFunction = getn_func(curFunctionId);

      if ( pCurFunction == nullptr )
      {
        msg( "eek" );
        break;
      }

      g_pSvcParser->ProcessFunction( pCurFunction );
    }
}

PBCL_PROCLOC_API plugin_t PLUGIN =
{
    IDP_INTERFACE_VERSION, 
    0,
    IDAP_init,
    IDAP_term,
    IDAP_run,
    "Modifies \"svc 0\" calls to api names",
    "put unistd.h into IDA's plugins directory inside ArmSvc directory",
    "ARM svc parser",
    "Alt-S"
};