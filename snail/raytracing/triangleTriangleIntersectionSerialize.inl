inline std::ostream& operator<<(std::ostream& out, intersectionHint h)
{
    static constexpr std::array<std::string_view, 7> hintToStr { "ab", "bc", "ac", "inside", "a", "b", "c" };
    out << hintToStr[h];
    return out;
}

template <typename type>
inline std::ostream& operator<<(std::ostream& out, const intersection<type>& i)
{
    out << "[ r=" << i.hint << ", p=" << i.position << ", u=" << i.u << ", v=" << i.v << ", t=" << i.t << " ]";
    return out;
}
