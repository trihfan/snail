/*
 *
 */

namespace snail
{
    template <typename type>
    std::ostream& operator<<(std::ostream& out, const std::vector<type>& v);

    #include "serialize.inl"
}
