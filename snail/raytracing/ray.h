/*
 * A ray used for the triangle intersections
 */

#pragma once

namespace snail
{
    template <typename type>
    class ray
    {
    public:
		ray() = default;

        /**
         * @brief Constructor of a ray from origin to end
         */
        ray(const vector3<type>& origin, const vector3<type>& end);

        const vector3<type>& getOrigin() const;

        const vector3<type>& getEnd() const;

        const vector3<type>& getDirection() const;

        type getLength() const;

    private:
        /**
         * @brief Ray origin
         */
        vector3<type> rayOrigin;

        /**
         * @brief Ray segment end
         */
        vector3<type> rayEnd;

        /**
         * @brief Ray direction unit vector
         */
        vector3<type> rayDirection;

        /**
         * @brief Ray segment length
         */
        type rayLength;
    };

    #include "ray.inl"
    #include "raySerialize.inl"
}

