/*
 * Class to generate mesh from simple shapes (box, sphere, ...)
 */

#pragma once

namespace snail
{
    class generate
	{
    public:
        /**
         * Build a box at the given position and size
         */
        template <typename type>
        static mesh<type>* box(const vector3<type>& position, const vector3<type>& size);

        /**
         * Build a sphere at the given position and radius
         */
        template <typename type>
        static mesh<type>* sphere(const vector3<type>& position, type radius, size_t u = 20, size_t v = 20);

        /**
         * Build a pyramid at the given position and size
         */
        template <typename type>
        static mesh<type>* pyramid(const vector3<type>& position, type height, type length);

        /**
         * Build a tetrahedron at the given position and size
         */
        template <typename type>
        static mesh<type>* tetrahedron(const vector3<type>& position, type height, type length);

        template <typename type>
        static mesh<type>* cylinder(const vector3<type>& position, type radius, size_t u = 20, size_t v = 20);

        template <typename type>
        static mesh<type>* cake(const vector3<type>& position, type radius, size_t u = 20, size_t v = 20);

        template <typename type>
        static mesh<type>* cone(const vector3<type>& position, type radius, size_t u = 20, size_t v = 20);

        template <typename type>
        static mesh<type>* torus(const vector3<type>& position, type radius, size_t u = 20, size_t v = 20);

        template <typename type>
        static mesh<type>* trapezoid(const vector3<type>& position, type radius, size_t u = 20, size_t v = 20);

        /**
         *
         */
        template <typename type>
        static mesh<type>* load(const std::string& filename);
	};

    #include "generate.inl"
}
