#ifndef _MULTIPROCESSOR_H_
#define _MULTIPROCESSOR_H_

#include <systemc>
#include "ac_tlm_protocol.H"

using tlm::tlm_transport_if;

#ifndef SAFE_FREE
#define SAFE_FREE(ptr) do{if(ptr!=NULL){free(ptr);ptr=NULL;}}while(0)
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) do{if(ptr!=NULL){delete(ptr);ptr=NULL;}}while(0)
#endif
#ifndef SAFE_ARRAY_DELETE
#define SAFE_ARRAY_DELETE(ptr) do{if(ptr!=NULL){delete[](ptr);ptr=NULL;}}while(0)
#endif

template<class T, int NPROCS = 8>
class multiprocessor
    :   public sc_module
    ,   public ac_tlm_transport_if
{
public:
    multiprocessor(sc_module_name module_name)
        :   sc_module(module_name)
        ,   target_export("iport")
        ,   mProcessors()
        ,   mArgC(0)
        ,   mArgV(NULL)
    {
        target_export(*this);

        for ( int p = 0; p < NPROCS; p++ ) {
            char buf[64];
            snprintf(buf,64,"%s_p%d",(const char*)module_name,p);
            mProcessors[p] = new T(buf);
        }
    }

    ~multiprocessor()
    {
        for ( int p = 0; p < NPROCS; p++ ) {
            SAFE_DELETE(mProcessors[p]);
        }
        for ( int p = 0; p < NPROCS; p++ ) {
            for ( int ac = 0; (mArgV[p] != NULL) && (ac < mArgC); ac++ ) {
                SAFE_FREE(mArgV[p][ac]);
            }
            SAFE_ARRAY_DELETE(mArgV[p]);
        }
    }

    ac_tlm_rsp transport(const ac_tlm_req& request)
    {
        ac_tlm_rsp response;
        switch ( request.type ) {
        case WRITE:
            if ( request.data == 0 ) {
                response.status = stop_processor(request.addr);
            } else {
                response.status = start_processor(request.addr);
            }
            break;
        default :
            response.status = ERROR;
            break;
        }
        return response;
    }

    void connect_DM_ports(ac_tlm_transport_if& target)
    {
        for ( int p = 0; p < NPROCS; p++ ) {
            mProcessors[p]->DM_port(target);
         }
    }

    void init(int argc, char* argv[], const bool pause_others = true)
    {
        mArgC = argc;
        for ( int p = 0; p < NPROCS; p++ ) {
            mArgV[p] = new char*[argc];
            for ( int arg = 0; arg < argc; arg++ ) {
                mArgV[p][arg] = strdup(argv[arg]);
            }
            mProcessors[p]->init(mArgC, mArgV[p]);
            if ( p > 0 && pause_others ) {
                mProcessors[p]->ISA.PauseProcessor();
            }
            std::cout << std::endl;
        }
    }

    void PrintStat()
    {
        for ( int p = 0; p < NPROCS; p++ ) {
            mProcessors[p]->PrintStat();
            std::cout << std::endl;
        }
    }

    int get_exit_status()
    {
        int exit_status = 0;
        for ( int p = 0; p < NPROCS; p++ ) {
            exit_status += mProcessors[p]->ac_exit_status;
        }
        return exit_status;
    }

private:
    ac_tlm_rsp_status start_processor(const uint32_t& proc_id)
    {
        ac_tlm_rsp_status status = ERROR;
        if ( proc_id < NPROCS ) {
#ifdef DEBUG
            std::cout << "Starting processor: " << proc_id << std::endl;
#endif
            mProcessors[proc_id]->ISA.ResumeProcessor();
            status = SUCCESS;
        } else {
#ifdef DEBUG
            std::cout << "Invalid processor: " << proc_id << std::endl;
#endif
        }
        return status;
    }
    ac_tlm_rsp_status stop_processor(const uint32_t& proc_id)
    {
        ac_tlm_rsp_status status = ERROR;
        if ( proc_id < NPROCS ) {
#ifdef DEBUG
            std::cout << "Stopping processor: " << proc_id << std::endl;
#endif
            mProcessors[proc_id]->ISA.PauseProcessor();
            status = SUCCESS;
        } else {
#ifdef DEBUG
            std::cout << "Invalid processor: " << proc_id << std::endl;
#endif
        }
        return status;
    }

public:
    sc_export<ac_tlm_transport_if>  target_export;

private:
    T*          mProcessors[NPROCS];
    int         mArgC;
    char***     mArgV;
};

#endif //_MULTIPROCESSOR_H_

