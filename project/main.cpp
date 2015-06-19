/******************************************************
 * This is the main file for the mips1 ArchC model    *
 * This file is automatically generated by ArchC      *
 * WITHOUT WARRANTY OF ANY KIND, either express       *
 * or implied.                                        *
 * For more information on ArchC, please visit:       *
 * http://www.archc.org                               *
 *                                                    *
 * The ArchC Team                                     *
 * Computer Systems Laboratory (LSC)                  *
 * IC-UNICAMP                                         *
 * http://www.lsc.ic.unicamp.br                       *
 ******************************************************/
 
// Rodolfo editou aqui
//
const char *project_name="mips";
const char *project_file="mips1.ac";
const char *archc_version="2.0beta1";
const char *archc_options="-abi -dy ";

#include <systemc.h>
#include "mips.H"
#include "memory.h"
#include "bus.h"
#include "lock.h"
#include <string.h>

int sc_main(int ac, char *av[])
{

  //!  ISA simulator
  int nprocs = 8;
  mips** procs = new mips*[nprocs];
  for (int p = 0; p < nprocs; p++) {
    char procId[10];
    snprintf(procId, 10, "mips%d", p);
    procs[p] = new mips(procId);
  }
  //! Bus
  ac_tlm_bus bus("bus");
// Memory
  ac_tlm_mem mem("mem");
  ac_tlm_lock lock("lock");

#ifdef AC_DEBUG
  char buf[64];
  for (int p = 0; p < nprocs; p++) {
    snprintf(buf, 64, "mips_proc%d.trace", p);
    ac_trace(buf);
  }
#endif 

  for (int p = 0; p < nprocs; p++) {
    procs[p]->DM_port(bus.target_export);
  }
  bus.MEM_port(mem.target_export);
  bus.LOCK_port(lock.target_export);

  char*** args = new char**[nprocs];
  for (int p = 0; p < nprocs; p++) {
    args[p] = new char*[ac];
    for (int i = 0; i < ac; i++) {
      args[p][i] = strdup(av[i]);
    }
    procs[p]->init(ac,args[p]);
    if ( p > 0 ) {
      //procs[p]->ISA.PauseProcessor();
    }
  }
  cerr << endl;

  sc_start();

  for (int p = 0; p < nprocs; p++) {
    procs[p]->PrintStat();
  }
  cerr << endl;

#ifdef AC_STATS
  //mips1_proc1.ac_sim_stats.time = sc_simulation_time();
  //mips2_proc2.ac_sim_stats.time = sc_simulation_time();
  //mips1_proc1.ac_sim_stats.print();
  //mips2_proc2.ac_sim_stats.print();
#endif 

#ifdef AC_DEBUG
  ac_close_trace();
#endif 

  int retCode = 0;
  for (int p = 0; p < nprocs; p++) {
    retCode += procs[p]->ac_exit_status;
  }
  return retCode;
}
