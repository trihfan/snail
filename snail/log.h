/*
 *
 */
#pragma once

#if defined(__linux__) || defined(__APPLE__)
    #define COUT_RED "\033[31m"
    #define COUT_GREEN "\033[32m"
    #define COUT_YELLOW "\033[33m"
    #define COUT_DEFAULT "\033[0m"
#else
    #define COUT_RED ""
    #define COUT_GREEN ""
    #define COUT_YELLOW ""
    #define COUT_DEFAULT ""
#endif

namespace snail
{
    namespace log
    {
        class infoStart
        {
        public:
            std::string getHeader()
            {
                return "[info] ";
            }
        };

        class warnStart
        {
        public:
            std::string getHeader()
            {
                return "[warn] ";
            }
        };

        class errStart
        {
        public:
            std::string getHeader()
            {
                errorCount++;
                return "[err] ";
            }

            void clear()
            {
#ifdef VERBOSITY_LEVEL_0
                std::cout << (errorCount > 0 ? COUT_RED : COUT_GREEN) << getHeader() << (errorCount - 1) << " error(s)"<< COUT_DEFAULT << std::endl;
#endif
                errorCount = 0;
            }

        private:
            uint64_t errorCount = 0;
        };

        static infoStart info;
        static warnStart warn;
        static errStart err;

        #include "log.inl"
    }
}
