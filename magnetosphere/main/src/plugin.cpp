#include "plugin.h"
#include "core.h"

#include <sstream>

namespace magnet {
namespace main {

    IPlugin::IPlugin(const char* pluginName, const float pluginVersion)
        : mPluginName(pluginName), mPluginVersion(pluginVersion)
    {
        mOptions = new OptionsMap();
        Core::getSingleton().setLibraryObject(pluginName, this);
    }

    IPlugin::~IPlugin()
    {
        delete mOptions;
    }

    const char* IPlugin::getName()
    {
        return mPluginName.c_str();
    }

    const IPlugin::OptionsMap* IPlugin::getOptions() 
    {
        return mOptions;
    }

    float IPlugin::getVersion()
    {
        return mPluginVersion;
    }

    void IPlugin::setOption(const char* option, const char* value)
    {
        (*mOptions)[option].value = value;
    }

    unsigned int IPlugin::addOptionChoice(const char* option, const char* choice)
    {
        (*mOptions)[option].choices.push_back(choice);
        return (unsigned int)(*mOptions)[option].choices.size() - 1;
    }
    
    bool IPlugin::getBoolOption(const char* option)
    {
        std::string val = (*mOptions)[option].value;
        if (val == "0" || val == "no" || val == "")
            return false;
        return true;
    }

    unsigned int IPlugin::getChoiceOption(const char* option)
    {
        Option* opt = &(*mOptions)[option];
        for (unsigned int i = 0; i < opt->choices.size(); i ++)
        {
            if (opt->choices[i] == opt->value)
                return i;
        }
        opt->value = opt->choices[0];
        return 0;
    }

    float IPlugin::getFloatOption(const char* option)
    {
        return (float)atof((*mOptions)[option].value.c_str());
    }

    int IPlugin::getIntOption(const char* option)
    {
        return atoi((*mOptions)[option].value.c_str());
    }

    const char* IPlugin::getStringOption(const char* option)
    {
        return (*mOptions)[option].value.c_str();
    }

    void IPlugin::setOptionType(const char* option, Option::OptionType type)
    {
        (*mOptions)[option].type = type;
    }

} // namespace main
} // namespace magnet