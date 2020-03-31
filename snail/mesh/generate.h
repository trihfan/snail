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
        static mesh_t<type> box(const vector3<type>& position, const vector3<type>& size);

        /**
         * Build a sphere at the given position and radius
         */
        template <typename type>
        static mesh_t<type> sphere(const vector3<type>& position, type radius, size_t u = 20, size_t v = 20);

        /**
         * Build a pyramid at the given position and size
         */
        template <typename type>
        static mesh_t<type> pyramid(const vector3<type>& position, type height, type length);

        /**
         * Build a tetrahedron at the given position and size
         */
        template <typename type>
        static mesh_t<type> tetrahedron(const vector3<type>& position, type height, type length);

        template <typename type>
        static mesh_t<type> cylinder(const vector3<type>& position, type radius, size_t u = 20, size_t v = 20);

        template <typename type>
        static mesh_t<type> cake(const vector3<type>& position, type radius, size_t u = 20, size_t v = 20);

        template <typename type>
        static mesh_t<type> cone(const vector3<type>& position, type radius, size_t u = 20, size_t v = 20);

        template <typename type>
        static mesh_t<type> torus(const vector3<type>& position, type radius, size_t u = 20, size_t v = 20);

        template <typename type>
        static mesh_t<type> trapezoid(const vector3<type>& position, type radius, size_t u = 20, size_t v = 20);
	};

    #include "generate.inl"
}
