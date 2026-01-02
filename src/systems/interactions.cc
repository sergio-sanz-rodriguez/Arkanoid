#include "interactions.h"

// Determine whether two entities overlap
bool is_interacting(const entity& entity1, const entity& entity2) {
    auto box1 = entity1.get_bounding_box();
    auto box2 = entity2.get_bounding_box();
    auto intersection = box1.findIntersection(box2);
    return intersection.has_value();
}

// Resolve potential collision between the ball and the paddle
sfx_id handle_collision(ball &the_ball, const paddle& the_paddle) {

    if (is_interacting(the_paddle, the_ball)) {

        // Make the ball bounce upwards
        the_ball.move_up();

        // Make the new direction depend on where the collision occurs on the paddle
        // If the collision is on the left of the paddle, make the ball bounce to the left
        if (the_ball.get_position().x < the_paddle.get_position().x)
            the_ball.move_left();
        else
            the_ball.move_right();
        return sfx_id::ball_paddle;
    }
    else {
        return sfx_id::none;
    }
}

// Resolve potential collision between the ball and a brick
sfx_id handle_collision(ball& the_ball, brick& the_brick) {

    if (is_interacting(the_brick, the_ball)) {

        // Update the brick's strength
        the_brick.weaken();

        if (the_brick.is_too_weak()) {
            // The brick is destroyed
            the_brick.destroy();
        }
        if (!the_ball.get_fireball()) {
            // Make the new direction depend on where the collision occurs on the brick
            // If the ball collides on the side of the brick, make the ball bounce to the left/right
            // If the ball collides on the top/bottom of the brick, make the ball bounce upwards/downwards

            // First we find the amount of overlap in each direction
            // The smaller the left overlap, the closer the ball is to the left side of the brick
            // And similarly for the other sides of the brick
            float left_overlap = the_ball.right() - the_brick.left();
            float right_overlap = the_brick.right() - the_ball.left();
            float top_overlap = the_ball.bottom() - the_brick.top();
            float bottom_overlap = the_brick.bottom() - the_ball.top();

            // If the left overlap is smaller than the right overlap, the ball hit the left side
            bool from_left = std::abs(left_overlap) < std::abs(right_overlap);
            bool from_top = std::abs(top_overlap) < std::abs(bottom_overlap);

            // Use the right or left overlap as appropriate
            float min_x_overlap = from_left ? left_overlap : right_overlap;
            float min_y_overlap = from_top ? top_overlap : bottom_overlap;

            // If the ball hit the left or right side of the brick, change its horizontal direction
            // If the ball hit the top or bottom of the brick, change its vertical direction
            if (std::abs(min_x_overlap) < std::abs(min_y_overlap)) {
                if (from_left) {
                    the_ball.move_left();
                }
                else {
                    the_ball.move_right();
                }
            }
            else {
                if (from_top) {
                    the_ball.move_up();
                }
                else {
                    the_ball.move_down();
                }
            }
        }
        return sfx_id::ball_brick;
    }
    else {
        return sfx_id::none;
    }
}

// Resolve potential collision between the bonus ball and the paddle
bool handle_collision(bonus& the_bonus, const paddle& the_paddle) {
    if (!is_interacting(the_bonus, the_paddle)) return false;
    the_bonus.destroy();
    return true;
}
