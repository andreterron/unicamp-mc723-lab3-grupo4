#include "memory_lock.h"

//#define DEBUG

memory_lock::memory_lock(sc_module_name module_name, int k)
    :   sc_module(module_name)
    ,   target_export("iport")
    ,   mLock(NULL)
    ,   mSize(k)
{
    target_export(*this);

    mLock = new uint8_t[k];
    memset(mLock, 0, k * sizeof(uint8_t));
}

memory_lock::~memory_lock()
{
    SAFE_ARRAY_DELETE(mLock);
}

ac_tlm_rsp memory_lock::transport(const ac_tlm_req& request)
{
    ac_tlm_rsp response;

    switch ( request.type ) {
    case READ:
        response.status = readm(request.addr, response.data);
        break;
    case WRITE:
        response.status = writem(request.addr, request.data);
        break;
    default :
        response.status = ERROR;
        break;
    }

    return response;
}

ac_tlm_rsp_status memory_lock::writem(const uint32_t& a, const uint32_t& d)
{
    ac_tlm_rsp_status status = ERROR;
    if ( a < mSize ) {
        *((uint32_t *) &mLock[a]) = *((uint32_t *) &d);
#ifdef DEBUG
        std::cerr << "Writing to memory lock. mLock[" << std::hex << a << "] = " << d << std::endl;
#endif
        status = SUCCESS;
    } else {
        std::cerr << "Lock memory out of bounds: " << a << std::endl;
    }
    return status;
}

ac_tlm_rsp_status memory_lock::readm(const uint32_t& a, uint32_t& d)
{
    ac_tlm_rsp_status status = ERROR;
    if ( a < mSize ) {
        *((uint32_t *) &d) = *((uint32_t *) &mLock[a]);
        *((uint32_t *) &mLock[a]) = 1;
#ifdef DEBUG
        std::cerr << "Reading memory lock. mLock[" << std::hex << a << "] = " << d << std::endl;
#endif
        status = SUCCESS;
    } else {
        std::cerr << "Lock memory out of bounds: " << a << std::endl;
    }
    return status;
}

