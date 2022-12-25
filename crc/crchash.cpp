/*
** The crc hash is licensed under the Apache License, Version 2.0, and a copy of the license is included in this file.
**
**Author:Wang Yaofu voipman@qq.com
**Description: The source file of class CrcHash.
*/

#include "crchash.h"
#include "crc16.h"
#include "crc32.h"
#include "crc64.h"
namespace qa_common {
    uint16_t hash16(const std::string& key)
    {
        return qa_calc_crc16(key.c_str(), key.size());
    }
    uint16_t hash16(const char* cpKey, const int iKeyLen)
    {
        return qa_calc_crc16(cpKey, iKeyLen);
    }

    uint32_t hash32(const std::string& key)
    {
        return qa_calc_crc32(key.c_str(), key.size());
    }
    uint32_t hash32(const char* cpKey, const int iKeyLen)
    {
        return qa_calc_crc32(cpKey, iKeyLen);
    }

    uint64_t hash64(const std::string& key)
    {
        return qa_calc_crc64(key.c_str(), key.size());
    }
    uint64_t hash64(const char* cpKey, const int iKeyLen)
    {
        return qa_calc_crc64(cpKey, iKeyLen);
    }
} // namespace common
