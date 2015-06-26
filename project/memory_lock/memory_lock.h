#ifndef _MEMORY_LOCK_H_
#define _MEMORY_LOCK_H_

#include <systemc>
#include "ac_tlm_protocol.H"

using tlm::tlm_transport_if;

#ifndef SAFE_FREE
#define SAFE_FREE(ptr) do{if(ptr!=NULL){free(ptr);ptr = NULL;}}while(0)
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(ptr) do{if(ptr!=NULL){delete(ptr);ptr = NULL;}}while(0)
#endif
#ifndef SAFE_ARRAY_DELETE
#define SAFE_ARRAY_DELETE(ptr) do{if(ptr!=NULL){delete[](ptr);ptr = NULL;}}while(0)
#endif
//#define DEBUG

class memory_lock
    :   public sc_module
    ,   public ac_tlm_transport_if
{
public:
    memory_lock(sc_module_name module_name, int k = 5242880);
    ~memory_lock();

    ac_tlm_rsp transport(const ac_tlm_req&);

private:
    ac_tlm_rsp_status writem(const uint32_t&, const uint32_t&);
    ac_tlm_rsp_status readm(const uint32_t&, uint32_t&);

public:
    sc_export<ac_tlm_transport_if>  target_export;

private:
    uint8_t*    mLock;
    int         mSize;
};

#endif //_MEMORY_LOCK_H_

