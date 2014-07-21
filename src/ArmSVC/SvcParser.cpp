#include "stdafx.h"
#include "SvcParser.h"
#include <string>

size_t funcCounter = 0;

bool SvcParser::ProcessFunction( func_t* pFunc )
{
  ea_t  curEa = pFunc->startEA;
  bool bFuncIdFound = false;

  for ( int i = 0; i < 10; ++i )
  {
      curEa = next_visea( curEa );
      decode_insn( curEa );

      if ( curEa > pFunc->endEA )
        break;

      if ( cmd.itype == ARM_svc )
      {
        insn_t curInst = cmd;
        // filter "svc 0" only
        if ( curInst.Operands[0].type == o_imm && curInst.Operands[0].value == 0 )
        {
          //msg( "svc detected: %X\n, in function: %s", curInst.ea );
          // here we are
          ea_t prevInstEa = prev_visea( curEa );
          decode_insn( prevInstEa );

          insn_t prevInst = cmd;
          // prev inst have to be MOV or LDR
          switch ( cmd.itype )
          {
          case ARM_ldr:
            //msg( "LDR\n");
            bFuncIdFound = true;
            break;

          case ARM_mov:
            if ( prevInst.Operands[0].type == o_reg && 
              prevInst.Operands[0].reg == 7 && // R7
              prevInst.Operands[1].type == o_imm )
            {
              lock_func funcLocker( pFunc );
              
              UnistdApisMapIt apiEntry = m_apisMap.find(prevInst.Operands[1].value);
              if ( apiEntry != m_apisMap.end() )
              {
                std::string funcName( "__call_" );
                char tmpBuff[256] = {0};
              
                //_itoa( funcCounter, tmpBuff, 10 );
                //funcName += tmpBuff;

                funcName += apiEntry->second.c_str();

                set_name( pFunc->startEA, funcName.c_str(), SN_NOWARN );
              }
              //else
              //  msg( "svc %d, at function: %.8X\n WAS NOT RECOGNIZED", prevInst.Operands[1].value, pFunc->startEA );

              ++funcCounter;
              bFuncIdFound = true;
            }
            break;
          }
        }
      }

      if (bFuncIdFound)
        break;
  }

  return bFuncIdFound;
}
