#if !defined(MAGNET_GRAPHICS_H_)
#define MAGNET_GRAPHICS_H_

#include "common.h"
#include "plugin.h"

namespace magnet {
namespace graphics {

    class Graphics : public main::IPlugin {
    public:
        void initialise();
        Graphics();
    private:
        IPlugin* mRenderer;
    };

} // namespace graphics
} // namespace magnet

#endif // !defined(MAGNET_GRAPHICS_H_)