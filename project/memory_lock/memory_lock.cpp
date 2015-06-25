#include "memory_lock.h"

memory_lock::memory_lock(sc_module_name module_name, int k)
    :   sc_module(module_name)
    ,   target_export("iport")
    ,   mLock(NULL)
    ,   mSize(k)
{
    target_export(*this);

    mLock = new char[k];
    memset(mLock, 0, k * sizeof(char));
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
        mLock[a] = d;
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
        d = mLock[a];
        mLock[a] = 1;
        status = SUCCESS;
    } else {
        std::cerr << "Lock memory out of bounds: " << a << std::endl;
    }
    return status;
}

