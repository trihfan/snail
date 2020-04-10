template <typename type>
std::ostream& operator<<(std::ostream& out, const std::vector<type>& v)
{
    for (const type& value : v)
    {
        out << value << " ";
    }
    return out;
}
