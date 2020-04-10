/*
 *
 */
#pragma once

#define EIGEN_DEFAULT_IO_FORMAT IOFormat(6, 0, ", ", ", ", "", "", "[", "]")
#define EIGEN_MATRIXBASE_PLUGIN "snail/math/vector3Extension.h"
#include <Eigen/Dense>

namespace snail
{
    template<typename type>
    using vector3 = Eigen::Matrix<type, 3, 1>;
}

#include "vector3Serialize.inl"
#include "vector3.inl"
