#if !defined(MAGNET_COMMANDWINDOW_H_)
#define MAGNET_COMMANDWINDOW_H_

#include "graphicscommon.h"

#include <string>

namespace magnet {
namespace graphics {

    class MAG_GRAPHICS_EXPORT ICmdWindow //: public main::stream
    {
    // Class
    public:
        ICmdWindow(const char* name);
        virtual ~ICmdWindow();
        
    // Methods
    public:
        virtual void writeString(const char* string) = 0;
    protected:
        void readString(const char* string);

    // Data
    private:
        std::string mName;
    };

} // namespace graphics
} // namespace magnet

#endif // !defined(MAGNET_COMMANDWINDOW_H_)