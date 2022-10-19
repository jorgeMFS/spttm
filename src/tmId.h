/**
    tmId.h
    Purpose: Int of 128 bits.

    @author Jorge Miguel Ferreira da Silva
    @version 0.1
*/

#pragma once

#include <iostream>

static const __uint128_t UINT128_MAX =__uint128_t(__int128_t(-1L));

#ifdef __SIZEOF_INT128__
    // do some fancy stuff here
    using TmId = __uint128_t;
    std::ostream& operator<<( std::ostream& dest, TmId value );
    TmId string_to_uint128(const std::string& s);
    bool is_number(const std::string& s);

#else
    # error This compiler does not support 128 bit integer types 
#endif

