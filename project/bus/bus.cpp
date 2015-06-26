/**
 * @file      memory.cpp
 * @author    Bruno de Carvalho Albertini
 *
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br/
 *
 * @version   0.1
 * @date      Sun, 02 Apr 2006 08:07:46 -0200
 *
 * @brief     Implements a ac_tlm memory.
 *
 * @attention Copyright (C) 2002-2005 --- The ArchC Team
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) any later version.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *
 */

//////////////////////////////////////////////////////////////////////////////
// Standard includes
// SystemC includes
// ArchC includes

#include "bus.h"

//////////////////////////////////////////////////////////////////////////////

//#define BUS_DEBUG

/// Constructor
ac_tlm_bus::ac_tlm_bus(sc_module_name module_name):
    sc_module(module_name),
    target_export("iport"),
    MEM_port("MEM_port", 8*1024*1024), // This is the memory port, assigned for 5MB
    LOCK_port("LOCK_port", 8*1024*1024),
    PROC_port("PROC_port", 8)
{
    /// Binds target_export to the memory
    target_export(*this);
}

/// Destructor
ac_tlm_bus::~ac_tlm_bus()
{
}

/// This is the transport method. Everything should go through this file.
/// To connect more components, you will need to have an if/then/else or a switch
/// statement inside this method. Notice that ac_tlm_req has an address field.
ac_tlm_rsp ac_tlm_bus::transport(const ac_tlm_req &request)
{
    ac_tlm_rsp response;

    const unsigned int LOCK_BASE = 0x800000;
    const unsigned int PROC_BASE = (LOCK_BASE << 1);

    ac_tlm_req tmp(request);
    if(request.addr < LOCK_BASE) {
#ifdef BUS_DEBUG
//        std::cerr << "bus) Transporting MEM. req={.addr=" << request.addr << ", .data=" << request.data << "}" << std::endl;
#endif
        response = MEM_port->transport(request);
    } else if (request.addr < PROC_BASE) {
#ifdef BUS_DEBUG
        std::cerr << "bus) Transporting LOCK. req={.addr=" << request.addr << ", .data=" << request.data << "}" << std::endl;
        std::cerr << "bus) Transporting LOCK. tmp={.addr=" << tmp.addr << ", .data=" << tmp.data << "}" << std::endl;
#endif
        tmp.addr -= LOCK_BASE;
        response = LOCK_port->transport(tmp);
    } else {
#ifdef BUS_DEBUG
        std::cerr << "bus) Transporting PROC. req={.addr=" << request.addr << ", .data=" << request.data << "}" << std::endl;
        std::cerr << "bus) Transporting PROC. tmp={.addr=" << tmp.addr << ", .data=" << tmp.data << "}" << std::endl;
#endif
        tmp.addr -= PROC_BASE;
        response = PROC_port->transport(tmp);
    }

    return response;
}

