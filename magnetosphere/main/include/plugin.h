#if !defined(MAGNET_PLUGIN_H_)
#define MAGNET_PLUGIN_H_

#include "common.h"

#include <map>
#include <string>
#include <vector>

namespace magnet {
namespace main {

    class MAG_MAIN_EXPORT IPlugin
    {
    // Types
    public:
        struct Option {
            std::string value;
            enum OptionType
            {
                OT_STRING  = 0,
                OT_INTEGER = 1,
                OT_FLOAT   = 2,
                OT_BOOL    = 3,
                OT_CHOICE  = 4,
                OT_PLUGIN  = 5,
            } type;
            std::vector <std::string> choices;
        };
        typedef std::map<std::string,Option> OptionsMap;

    // Class
    protected:
        IPlugin(const char* pluginName, const float pluginVersion);
        ~IPlugin();

    // Methods
    public:
        float getVersion();
        const char* getName();
        const OptionsMap* getOptions();
        void setOption(const char* option, const char* value);
        // note, applyOptions returns whether or not changes were applied, to notify the user of a needed restart
        virtual bool applyOptions();
        // initialise is called after plugin options are loaded from the config file
        virtual void initialise() = 0;
    protected:
        void setOptionType(const char* option, Option::OptionType type);
        unsigned int addOptionChoice(const char* option, const char* choice);
        unsigned int getChoiceOption(const char* option);
        const char* getStringOption(const char* option);
        float getFloatOption(const char* option);
        bool getBoolOption(const char* option);
        int getIntOption(const char* option);

    // Data
    private:
        OptionsMap* mOptions;
        std::string mPluginName;
        float mPluginVersion;
    };

    // All plugins should use this, or export their own extern C getModuleObject
    #define INITIALISE_MAGNET_PLUGIN(classname)                \
        static classname s_##classname##_ModuleObject;         \
        extern "C" MAGNET_EXPORT                               \
            magnet::main::IPlugin* getModuleObject()           \
        {                                                      \
            return &s_##classname##_ModuleObject;              \
        }                                                      \

} // namespace main
} // namespace magnet

#endif // !defined(MAGNET_PLUGIN_H_)
