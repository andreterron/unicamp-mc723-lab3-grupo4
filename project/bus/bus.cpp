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


/// Constructor
ac_tlm_bus::ac_tlm_bus(sc_module_name module_name):
    sc_module(module_name),
    target_export("iport"),
    MEM_port("MEM_port", 5242880U), // This is the memory port, assigned for 5MB
    LOCK_port("LOCK_port", 5242880U),
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

    const unsigned int LOCK_START = 0x800000;
    const unsigned int PROC_CTRL_START = (LOCK_START << 1);

    if(request.addr < LOCK_START) {
        response = MEM_port->transport(request);
    } else if (request.addr < PROC_CTRL_START) {
        response = LOCK_port->transport(request);
    } else {
        ac_tlm_req newReq(request);
        newReq.addr -= PROC_CTRL_START;

        PROC_port->transport(newReq);
    }

    return response;
}

