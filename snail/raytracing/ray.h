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
        ray(const vector3<type>& origin, const vector3<type>& end)
            : rayOrigin(origin), rayEnd(end)
        {
            vector3<type> raySegment = rayEnd - rayOrigin;
            rayLength = raySegment.norm();
            rayDirection = raySegment / rayLength;
        }

        const vector3<type>& getOrigin() const
        {
            return rayOrigin;
        }

        const vector3<type>& getEnd() const
        {
            return rayEnd;
        }

        const vector3<type>& getDirection() const
        {
            return rayDirection;
        }

        type getLength() const
        {
            return rayLength;
        }

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
}

