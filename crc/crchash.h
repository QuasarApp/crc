/*
** The crc hash is licensed under the Apache License, Version 2.0, and a copy of the license is included in this file.
**
**Author:Wang Yaofu voipman@qq.com
**Description: The header file of class CrcHash.
*/

#ifndef _COMMON_CRC_CRCHASH_H_
#define _COMMON_CRC_CRCHASH_H_
#include <string>
#include <stdint.h>
namespace qa_common {
    // string => 0x0000-0xffff
    uint16_t hash16(const std::string& key);
    uint16_t hash16(const char* cpKey, const int iKeyLen);

    // string => 0x00000000-0xffffffff
    uint32_t hash32(const std::string& key);
    uint32_t hash32(const char* cpKey, const int iKeyLen);

    // string => 0x0000000000000000-0xffffffffffffffff
    uint64_t hash64(const std::string& key);
    uint64_t hash64(const char* cpKey, const int iKeyLen);
} // namespace common
#endif  // _COMMON_CRC_CRCHASH_H_
