/*
 *
 */

namespace snail
{
    // mesh
    template <typename type>
    std::ostream& operator<<(std::ostream& out, const triangle<type>& t);

    // raytracing
    template <typename type>
    std::ostream& operator<<(std::ostream& out, const ray<type>& r);

    template <typename type>
    std::ostream& operator<<(std::ostream& out, const intersection<type>& inter);

    // all
    template <typename type>
    std::ostream& operator<<(std::ostream& out, const std::vector<type>& v);

    #include "output.inl"
}
