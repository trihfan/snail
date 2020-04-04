class logEnd
{
public:
    ~logEnd()
    {
#ifdef VERBOSITY_LEVEL_0
        std::cout << std::endl;
#endif
    }
};

typedef std::basic_ostream<char, std::char_traits<char>> coutType;
typedef coutType& (*coutManipulator)(coutType&);

inline std::unique_ptr<logEnd> operator<<(std::unique_ptr<logEnd> l, coutManipulator manip)
{
#ifdef VERBOSITY_LEVEL_0
    manip(std::cout);
#endif
    return std::move(l);
}

template<typename type>
inline std::unique_ptr<logEnd> operator<<(infoStart& info, const type& value)
{
#ifdef VERBOSITY_LEVEL_0
    std::cout << info.getHeader() << value;
#endif
    return std::make_unique<logEnd>();
}

template<typename type>
inline std::unique_ptr<logEnd> operator<<(warnStart& warning, const type& value)
{
#ifdef VERBOSITY_LEVEL_0
    std::cout << warning.getColor() << warning.getHeader() << value << COUT_DEFAULT;
#endif
    return std::make_unique<logEnd>();
}

template<typename type>
inline std::unique_ptr<logEnd> operator<<(errStart& error, const type& value)
{
#ifdef VERBOSITY_LEVEL_0
    std::cout << error.getColor() << error.getHeader() << value << COUT_DEFAULT;
#endif
    return std::make_unique<logEnd>();
}

template<typename type>
inline std::unique_ptr<logEnd> operator<<(std::unique_ptr<logEnd> l, const type& value)
{
#ifdef VERBOSITY_LEVEL_0
    std::cout << value;
#endif
    return std::move(l);
}
