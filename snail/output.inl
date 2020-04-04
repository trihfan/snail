template <typename type>
std::ostream& operator<<(std::ostream& out, const triangle<type>& t)
{
    out << "[" << t.getSide(0) << ", " << t.getSide(1) << ", " << t.getSide(2) << "]";
    return out;
}

template <typename type>
std::ostream& operator<<(std::ostream& out, const ray<type>& r)
{
    out << r.getOrigin();
    return out;
}

template <typename type>
std::ostream& operator<<(std::ostream& out, const intersection<type>& inter)
{
    static const std::array<std::string, 7> hintToStr { "ab", "bc", "ca", "inside", "a", "b", "c" };
    out << "[ " << hintToStr[inter.hint] << ", " << inter.position << ", " << inter.u << ", " << inter.v << ", " << inter.t << " ]";
    return out;
}

template <typename type>
std::ostream& operator<<(std::ostream& out, const std::vector<type>& v)
{
    for (const type& value : v)
    {
        out << value << " ";
    }
    return out;
}
