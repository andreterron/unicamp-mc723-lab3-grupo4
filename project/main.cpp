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
#include "multiprocessor.h"
#include "memory.h"
#include "bus.h"
#include "memory_lock.h"
#include <string.h>


int sc_main(int ac, char *av[])
{
    // Bus
    ac_tlm_bus bus("bus");
    // Memory
    ac_tlm_mem mem("mem", 8*1024*1024);
    // Memory lock
    memory_lock lock("memory_lock", 8*1024*1024);
    // ISA simulator
    multiprocessor<mips, 8> multimips("multimips");

#ifdef AC_DEBUG
    //char buf[64];
    //for (int p = 0; p < NPROCS; p++) {
    //    snprintf(buf, 64, "mips_proc%d.trace", p);
    //    ac_trace(buf);
    //}
#endif

    printf("Connecting ports\n");
    multimips.connect_DM_ports(bus.target_export);
    bus.MEM_port(mem.target_export);
    bus.LOCK_port(lock.target_export);
    bus.PROC_port(multimips.target_export);

    printf("Initializing\n");
    multimips.init(ac, av, true);

    printf("Running simulation\n");
    sc_start();

    multimips.PrintStat();
#ifdef AC_STATS
    //mips1_proc1.ac_sim_stats.time = sc_simulation_time();
    //mips2_proc2.ac_sim_stats.time = sc_simulation_time();
    //mips1_proc1.ac_sim_stats.print();
    //mips2_proc2.ac_sim_stats.print();
#endif

#ifdef AC_DEBUG
    ac_close_trace();
#endif

    return multimips.get_exit_status();
}
