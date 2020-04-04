/*
 *
 */
#pragma once

namespace std
{
    template <class type>
    inline void hashCombine(std::size_t& seed, const type& value);

    #include "hash.inl"
}
