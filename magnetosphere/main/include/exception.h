#if !defined(MAGNET_EXCEPTION_H_) 
#define MAGNET_EXCEPTION_H_

#include "common.h"

#include <string>

namespace magnet {
namespace main {

#define MAG_EXCEPT(type) throw type(__LINE__, __FILE__, __DATE__ " " __TIME__)
#define MAG_EXCEPT_DESCR(type, description) throw type(__LINE__, __FILE__, __DATE__ " " __TIME__, description)

    class MAG_MAIN_EXPORT Exception
    {
    // Class
    protected:
        Exception(
            const std::string& descr = (const std::string)"Internal Error!",
            unsigned int line = 0, 
            const char* file = NULL, 
            const char* date = NULL
            ) : 
            mDescription(descr),
            mLine(line),
            mFile(file),
            mDate(date)
        {}

    // Methods
    public:
        const char* getDescription() const { return mDescription.c_str(); }

    // Data
    private:
        const std::string mDescription;
        unsigned int mLine;
        const char* mFile;
        const char* mDate;
    };

#define DEFINE_EXCEPTION(name, description)                             \
    class MAG_MAIN_EXPORT name : public Exception {                     \
    public:                                                             \
        name(                                                           \
            unsigned int line,                                          \
            const char* file,                                           \
            const char* date = NULL,                                    \
            const std::string& descr = (const std::string)description   \
            ) : Exception(descr, line, file, date)                      \
        {}                                                              \
        name(                                                           \
            const std::string& descr = (const std::string)description   \
            ) : Exception(descr)                                        \
        {}                                                              \
    };                                                                  \
    
    
    DEFINE_EXCEPTION( EXCEPTION_INTERNAL, "Internal error." )
    DEFINE_EXCEPTION( EXCEPTION_PARAMETERS, "Invalid parameters." )
    DEFINE_EXCEPTION( EXCEPTION_CONFIG, "Configuration error." )
    DEFINE_EXCEPTION( EXCEPTION_UNSUPPORTED, "The requested action is not supported." )

} // namespace main
} // namespace magnet

#endif // !defined(MAGNET_EXCEPTION_H_)
