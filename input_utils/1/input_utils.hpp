#ifndef INPUT_UTILS_HPP_INCLUDED
#define INPUT_UTILS_HPP_INCLUDED

#include <glm/glm.hpp>

namespace input_utils
{

struct ViewConfig
{
    // how many clip space units to move for each mouse movement
    // (mouse movement typically in pixels)
    float trans_per_mouse_move;
    float zoom_per_scroll;
};

void scroll(glm::mat4 &view, float scroll, glm::vec2 focus, ViewConfig const &config);
void mouse_move(glm::mat4 &view, glm::vec2 movement, ViewConfig const &config);
}

#endif
