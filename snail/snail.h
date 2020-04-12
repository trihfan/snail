/*
 * Main header of snail library
 */

// declaration
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

// windows specific
#define NOMINMAX  

// std
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
#include <fstream>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <filesystem>
#include <limits>

// external
#ifdef SNAIL_JSON
    #include <nlohmann/json.hpp>
#endif

#ifdef SNAIL_CHAI
    #include <chaiscript/chaiscript.hpp>
#endif

// utils
#include "utils/serialize.h"
#include "utils/log.h"
#include "utils/hash.h"

// math
#include "math/vector3.h"
#include "math/math.h"

// raytracing
#include "raytracing/ray.h"
#include "raytracing/triangleRayIntersection.h"
#include "raytracing/triangleTriangleIntersection.h"

// mesh
#include "mesh/indexable.h"
#include "mesh/triangle.h"
#include "mesh/mesh.h"
#include "mesh/generate.h"

// chai
#ifdef SNAIL_CHAI
    #include "chai.h"
#endif
#pragma once
