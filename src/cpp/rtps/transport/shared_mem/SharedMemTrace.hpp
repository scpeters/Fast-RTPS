// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _FASTDDS_SHAREDMEM_TRACE_H_
#define _FASTDDS_SHAREDMEM_TRACE_H_

#include <fastdds/rtps/common/Locator.h>

namespace eprosima{
namespace fastdds{
namespace rtps{

static uint16_t __dump_id = 0;

static void __dump_packet(
        FILE* f,
        const fastrtps::rtps::Locator_t& from,
        const fastrtps::rtps::Locator_t& to,
        const fastrtps::rtps::octet* buf,
        const uint32_t len)
{

    if (f != NULL && from.kind == 16 && to.kind == 16)
    {
        uint32_t ipSize = len + 28;
        uint32_t udpSize = len + 8;

        __dump_id++;
        if (__dump_id == 0)
        {
            __dump_id++;
        }

        // IP header
        fprintf(f, "000000 45 00 %02x %02x %02x %02x 00 00 11 11 00 00\n", (ipSize >> 8) & 0xFF, ipSize & 0xFF,
                (__dump_id >> 8) & 0xFF, __dump_id & 0xFF);
                
        if (IsAddressDefined(from))
        {
            fprintf(f, "00000c %02x %02x %02x %02x\n", from.address[12], from.address[13], from.address[14],
                    from.address[15]);
        }
        else
        {
            fprintf(f, "00000c %02x %02x %02x %02x\n", 192, 168, 1, 254);
        }
        if (IsAddressDefined(to))
        {
            fprintf(f, "000010 %02x %02x %02x %02x\n", to.address[12], to.address[13], to.address[14], to.address[15]);
        }
        else
        {
            fprintf(f, "000010 %02x %02x %02x %02x\n", 192, 168, 1, 254);
        }

        // UDP header
        fprintf(f, "000014 %02x %02x %02x %02x\n", (from.port >> 8) & 0xFF, from.port & 0xFF, (to.port >> 8) & 0xFF,
                to.port & 0xFF);
        fprintf(f, "000018 %02x %02x 00 00", (udpSize >> 8) & 0xFF, udpSize & 0xFF);

        // Data
        for (uint32_t i = 0; i < len; i++)
        {
            if ((i & 15) == 0)
            {
                fprintf(f, "\n%06x", i + 28);
            }
            fprintf(f, " %02x", buf[i]);
        }

        fprintf(f, "\n\n");
    }
}

} // namespace rtps
} // namespace fastdds
} // namespace eprosima

#endif // _FASTDDS_SHAREDMEM_TRACE_H_