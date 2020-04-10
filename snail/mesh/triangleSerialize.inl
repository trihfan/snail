template <typename type>
std::ostream& operator<<(std::ostream& out, const triangle<type>& t)
{
    out << "[" << t.getSide(0).getOrigin() << ", " << t.getSide(1).getOrigin() << ", " << t.getSide(2).getOrigin() << "]";
    return out;
}
