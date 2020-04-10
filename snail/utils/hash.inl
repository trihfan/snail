template <class type>
inline void hashCombine(std::size_t& seed, const type& value)
{
    std::hash<type> hasher;
    seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

