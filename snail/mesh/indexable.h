/*
 * Indexable item
 */

#pragma once

namespace snail
{
    class indexable
    {
    public:
        indexable() = default;
        virtual ~indexable() = default;

        /**
         * @brief Set the item index
         */
        inline void setIndex(size_t index);

        /**
         * @brief Return the item index
         */
        inline size_t getIndex() const;

    private:
        /**
         * @brief The item index
         */
        size_t index;
    };
}

#include "indexable.inl"
