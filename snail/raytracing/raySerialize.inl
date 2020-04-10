template <typename type>
inline std::ostream& operator<<(std::ostream& out, const ray<type>& r)
{
    out << "l=" << r.getLength() << ", from=" << r.getOrigin() << ", to=" << r.getEnd() << ", direction=" << r.getDirection();
    return out;
}
