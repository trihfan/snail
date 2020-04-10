template <typename type>
triangle<type>::triangle(size_t a, size_t b, size_t c, const ray<type>& ab, const ray<type>& bc, const ray<type>& ca)
    : indices({ a, b, c }), sides({ ab, bc, ca }), flags(0)
{
    const type& abl = ab.getLength();
    const type& bcl = bc.getLength();
    const type& cal = ca.getLength();

    const type s = (abl + bcl + cal) / type(2);
    area = std::sqrt(s * (s - abl) * (s - bcl) * (s - cal));

    log(debug) << "New triangle: " << *this;
    if (area < epsilon<type>())
    {
        log(err) << "triangle area is null";
    }
}

template <typename type>
size_t triangle<type>::operator[](size_t index) const
{
    return indices[index];
}

template <typename type>
size_t& triangle<type>::operator[](size_t index)
{
    return indices[index];
}

template <typename type>
const ray<type>& triangle<type>::getSide(size_t index) const
{
    return sides[index];
}

template <typename type>
void triangle<type>::setFlag(flag flag, bool enabled)
{
    flags = enabled ? flags | flag : flags & ~flag;
}

template <typename type>
bool triangle<type>::hasFlag(flag flag) const
{
    return flags & flag;
}

template <typename type>
void triangle<type>::setId(size_t id)
{
    this->id = id;
}

template <typename type>
size_t triangle<type>::getId() const
{
    return id;
}

template <typename type>
const type& triangle<type>::getArea() const
{
    return area;
}
