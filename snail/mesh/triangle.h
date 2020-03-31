/*
 * A triangle
 */

#pragma once

namespace snail
{
    template <typename type>
    class triangle : public indexable
    {
    public:
        /**
         * @brief Enumeration of triangle flags
         */
        enum flag : int
        {
            invalid = 1,
        };

        /**
         * @brief Constructor with triangle indices and sides
         */
        triangle(size_t a, size_t b, size_t c, ray<type> ab, ray<type> bc, ray<type> ac);

        /**
         * @brief Return the vertex index
         */
        size_t operator[](size_t index) const;

        /**
         * @brief Return the vertex index
         */
        size_t& operator[](size_t index);


        /**
         * @brief Return the triangle side
         */
        const ray<type>& getSide(size_t index) const;

        /**
         * @brief Add or remove a flag to the triangle
         */
        void setFlag(flag flag, bool enabled);

        /**
         * @brief Return true if a flag is set
         */
        bool hasFlag(flag flag) const;

        void setId(size_t id);
        size_t getId() const;

    private:
        /**
         * @brief The triangle indices
         */
        std::array<size_t, 3> indices;

        /**
         * @brief The triangle sides
         */
        std::array<ray<type>, 3> sides;

        /**
         * @brief The triangle flags
         */
        int flags;

        size_t id;
    };

    #include "triangle.inl"
}
