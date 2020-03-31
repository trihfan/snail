template <typename type>
triangle<type>::triangle(size_t a, size_t b, size_t c, ray<type> ab, ray<type> bc, ray<type> ac)
    : indices({ a, b, c }), sides({ ab, bc, ac }), flags(0)
{
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
