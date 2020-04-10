namespace std
{
    template<typename type>
    struct hash<snail::vector3<type>>
    {
        size_t operator()(const snail::vector3<type>& value) const
        {
            size_t seed = 0;
            hashCombine(seed, value.x());
            hashCombine(seed, value.y());
            hashCombine(seed, value.z());
            return seed;
        }
    };
}
