template <typename type>
ray<type>::ray(const vector3<type>& origin, const vector3<type>& end)
    : rayOrigin(origin), rayEnd(end)
{
    vector3<type> raySegment = rayEnd - rayOrigin;
    rayLength = raySegment.norm();
    rayDirection = raySegment / rayLength;
    assert(equals(rayDirection.norm(), type(1)));
}

template <typename type>
const vector3<type>& ray<type>::getOrigin() const
{
    return rayOrigin;
}

template <typename type>
const vector3<type>& ray<type>::getEnd() const
{
    return rayEnd;
}

template <typename type>
const vector3<type>& ray<type>::getDirection() const
{
    return rayDirection;
}

template <typename type>
type ray<type>::getLength() const
{
    return rayLength;
}

