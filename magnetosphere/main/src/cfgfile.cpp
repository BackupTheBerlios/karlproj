#include "cfgfile.h"

#include <fstream>

namespace magnet {
namespace main {

    ConfigFile::ConfigFile()
    {
        mSettings = new SectionMap();
    }

    ConfigFile::ConfigFile(const char* fileName)
    {
        mSettings = new SectionMap();
        setFileName(fileName);
        read();
    }

    ConfigFile::~ConfigFile()
    {
        //write();
        delete mSettings;
    }

    void ConfigFile::read()
    {
        std::ifstream fil;
        fil.open(mFileName.c_str());
        if (fil.is_open())
        {
            std::string cursection("Default Section");
            std::string key, value;
            size_t blackspace;
            while (fil >> key)
            {
                if (key == "::")
                {
                    getline(fil, value);
                    cursection = std::string(value,value.find_first_not_of(" \t"));
                } else {
                    getline(fil, value);
                    blackspace = value.find("=");
                    if (blackspace != -1)
                    {
                        key += std::string(value, 0, blackspace);
                        (*mSettings)[cursection][key] = std::string(value, blackspace + 1);
                    } else {
                        blackspace = key.find("=");
                        if (blackspace != -1)
                        {
                            (*mSettings)[cursection][std::string(key, 0, blackspace)] = std::string(key, blackspace + 1) + value;
                        }
                    }
                }
            }
            fil.close();
        }
    }

    void ConfigFile::write()
    {
        std::ofstream fil;
        fil.open(mFileName.c_str());
        if (fil.is_open())
        {
            SectionMap::iterator sectionIter;
            SettingMap::iterator settingIter;
            for (sectionIter = mSettings->begin(); 
                sectionIter != mSettings->end(); 
                sectionIter ++)
            {
                fil << std::endl << ":: " << sectionIter->first << std::endl;
                for (settingIter = sectionIter->second.begin();
                    settingIter != sectionIter->second.end();
                    settingIter ++)
                {
                    fil << settingIter->first << "=" 
                        << settingIter->second << std::endl;
                }
            }
            fil.close();
        } else {
            throw( "Failed to open config file for writing!" );
        }
    }

    const ConfigFile::SettingMap* ConfigFile::getSection(const char* section)
    {
        return &(*mSettings)[section];
    }

    const char* ConfigFile::getValue(const char* section, const char* key, const char* defaultValue)
    {
        if ((*mSettings)[section].find(key) == (*mSettings)[section].end())
            (*mSettings)[section][key] = defaultValue;
        return (*mSettings)[section][key].c_str();
    }

    void ConfigFile::setFileName(const char* fileName)
    {
        mFileName = fileName;
    }

    void ConfigFile::setValue(const char* section, const char* key, const char* value)
    {
        (*mSettings)[section][key] = value;
    }

} // namespace main
} // namespace magnet