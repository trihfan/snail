/*
 *
 */
#pragma once

#define EIGEN_DEFAULT_IO_FORMAT IOFormat(6, 0, ", ", "", "[", "]", "[", "]")
#define EIGEN_MATRIXBASE_PLUGIN "snail/mesh/vertex.h"
#include <Eigen/Dense>

namespace snail
{
    template<typename type>
    using vector3 = Eigen::Matrix<type, 3, 1>;
}
