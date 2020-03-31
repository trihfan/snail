/*
 *
 */
#pragma once

//#define EIGEN_NO_STATIC_ASSERT
//#define EIGEN_NO_DEBUG
#define EIGEN_MATRIXBASE_PLUGIN "snail/mesh/vertex.h"
#include <Eigen/Dense>

namespace snail
{
    template<typename type>
    using vector3 = Eigen::Matrix<type, 3, 1>;
}

namespace std
{
    template <class type>
    inline void hashCombine(std::size_t& seed, const type& value)
    {
        std::hash<type> hasher;
        seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template<typename type>
    struct hash<snail::vector3<type>>
    {
        size_t operator()(const snail::vector3<type>& value) const
        {
            size_t seed = 0;
            hashCombine(seed, value.x());
            hashCombine(seed, value.y());
            hashCombine(seed, value.z());
            return seed;
        }
    };
}
