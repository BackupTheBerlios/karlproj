#if !defined(MAGNET_GRAPHICS_H_)
#define MAGNET_GRAPHICS_H_

#include "graphicscommon.h"
#include "plugininterfaces.h"
#include "graphicsplugin.h"

namespace magnet {
namespace graphics {

    class MAG_GRAPHICS_EXPORT Graphics : public main::IGraphicsInterface {
        friend class ICmdWindow;
    // Class
    public:
        Graphics();
    
    // Methods
    public:
        void initialise();
        bool update();
        void shutdown();
        void writeString(const char* string);
        void setRenderer(IGraphicsPlugin *renderer);
    private:
        void readString(const char* string, const char* source);

    // Data
    private:
        IGraphicsPlugin* mRenderer;
        ICmdWindow* mCmdWindow;
    };

} // namespace graphics
} // namespace magnet

#endif // !defined(MAGNET_GRAPHICS_H_)