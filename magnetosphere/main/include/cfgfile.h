#if !defined(MAGNET_CFGFILE_H_)
#define MAGNET_CFGFILE_H_

#include "common.h"

#include <map>
#include <string>

namespace magnet {
namespace main {

    class MAG_MAIN_EXPORT ConfigFile {
    // Types
    public:
        typedef std::map<std::string,std::string> SettingMap;
        typedef std::map<std::string,SettingMap> SectionMap;

    // Class
    public:
        ConfigFile();
        ConfigFile(const char* fileName);
        ~ConfigFile();

    // Methods
    public:
        void setValue(const char* section, const char* key, const char* value);
        const char* getValue(const char* section, const char* key, const char* defaultValue);
        const SettingMap* getSection(const char* section);
        void read();
        void write();
        void setFileName(const char* fileName);
            
    // Data
    private:
        std::string mFileName;
        SectionMap* mSettings;
    };

} // namespace main
} // namespace magnet

#endif // !defined(MAGNET_CFGFILE_H_)