/*
 *
 */
#pragma once

namespace snail
{
    enum logType : int
    {
        err,
        warn,
        info,
        success,
        fail,
        debug
    };


    inline uint64_t errorCount = 0;

    class log
    {
    public:
        log(logType type) : type(type)
        {
            if (type == err)
            {
                errorCount++;
            }
            setColor(type);
            static const std::array<std::string_view, 6> headers { "err", "warn", "info", "success", "fail", "debug" };
            write(std::string("[") + std::string(headers[type]) + "] ");
        }

        ~log()
        {
            setColor(info);
            *this << std::endl;
        }

        // stream operator
        template<typename type>
        inline log& operator<<(const type& value)
        {
            write(value);
            return *this;
        }

        // std manip
        typedef std::basic_ostream<char, std::char_traits<char>> coutType;
        typedef coutType& (*coutManipulator)(coutType&);

        inline log& operator<<(coutManipulator manip)
        {
            #ifdef VERBOSITY_LEVEL
                if (this->type <= verbosity[VERBOSITY_LEVEL])
                {
                    manip(std::cout);
                }
            #endif
            return *this;
        }

    private:
        const std::array<int, 5> verbosity { -1, 0, 4, 4, 5 };
        logType type;

        void setColor(logType type)
        {
            #if defined(__linux__) || defined(__APPLE__)
                static const std::array<std::string, 6> colors { "\033[31m", "\033[33m", "\033[0m", "\033[32;1m", "\033[31;1m", "\033[35m" };
                write(colors[type]);
            #else
                // todo
            #endif
        }

        template<typename type>
        void write(const type& value)
        {
            #ifdef VERBOSITY_LEVEL
                if (this->type <= verbosity[VERBOSITY_LEVEL])
                {
                    std::cout << value;
                }
            #endif
        }
    };

    class errorCounter
    {
    public:
        static void finish()
        {
            log(errorCount > 0 ? fail : success) << errorCount << " error(s) detected";
            errorCount = 0;
        }
    };
}
