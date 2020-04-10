template <typename type>
inline std::ostream& operator<<(std::ostream& out, typename triangleRayIntersection<type>::intersectionHint h)
{
	static constexpr std::array<std::string_view, 3> hintToStr { "parallels", "outside", "inside" };
    out << hintToStr[h];
    return out;
}
