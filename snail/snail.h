/*
 *
 */

namespace snail
{
    template <typename type>
    class triangle;

    template <typename type>
    class mesh;

    template <typename type>
    class ray;

    template <typename type>
    struct intersection;
}

#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
#include <list>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <memory>

#include "vector3.h"
#include "log.h"
#include "hash.h"
#include "output.h"
#include "math/math.h"

#include "raytracing/ray.h"
#include "raytracing/triangleRayIntersection.h"
#include "raytracing/triangleTriangleIntersection.h"

#include "mesh/cut1.h"
#include "mesh/cut2.h"
#include "mesh/indexable.h"
#include "mesh/triangle.h"
#include "mesh/mesh.h"
#include "mesh/generate.h"

#pragma once
