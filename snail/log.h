/*
 *
 */
#pragma once

#if defined(__linux__) || defined(__APPLE__)
    #define COUT_RED "\033[31m"
    #define COUT_YELLOW "\033[33m"
    #define COUT_DEFAULT "\033[0m"
#else
    #define COUT_RED ""
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

            std::string getColor()
            {
                return COUT_YELLOW;
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

            std::string getColor()
            {
                return COUT_RED;
            }

            void clear()
            {
#ifdef VERBOSITY_LEVEL_0
                std::cout << getColor() << getHeader() << "error count: " << errorCount << COUT_DEFAULT << std::endl;
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
