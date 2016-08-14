
#include "input_utils.hpp"
#include <glm/glm.hpp>
#include <cmath>

/* view matrix:
 *
 *     S 0 0 X
 *     0 S 0 Y
 *     0 0 1 0
 *     0 0 0 1
 *
 * S:   Clip Space Units per World Units
 * X,Y: Translation in Clip Space Units
 */

using namespace input_utils;

void input_utils::mouse_move(glm::mat4 &view, glm::vec2 movement, ViewConfig const &config)
{
    view[3][0]+= movement[0] * config.trans_per_mouse_move;
    view[3][1]+= movement[1] * config.trans_per_mouse_move;
}

void input_utils::scroll(glm::mat4 &view, float scroll, glm::vec2 focus, ViewConfig const &config)
{
    float zoom = pow(config.zoom_per_scroll, scroll);

    glm::vec2 focus_world(
	(focus.x - view[3][0]) / view[0][0],
	(focus.y - view[3][1]) / view[1][1]
    );

    // where the focus would be shifted at
    // in clip space units
    glm::vec2 new_focus(
	focus_world.x * view[0][0] * zoom + view[3][0],
	focus_world.y * view[1][1] * zoom + view[3][1]
    );
    
    // correction translation in world units
    glm::vec2 correction = focus - new_focus;
    
    view[0][0]*= zoom;
    view[1][1]*= zoom;
    view[3][0]+= correction.x;
    view[3][1]+= correction.y;
}
