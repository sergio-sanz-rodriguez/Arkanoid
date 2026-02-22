#include "interactions.h"

// Determine whether two entities overlap
bool is_interacting(const entity& entity1, const entity& entity2) {
    //auto box1 = entity1.get_bounding_box();
    //auto box2 = entity2.get_bounding_box();
    //auto intersection = box1.findIntersection(box2);
    //return intersection.has_value();
    const sf::FloatRect a = entity1.get_bounding_box();
    const sf::FloatRect b = entity2.get_bounding_box();

    constexpr float eps = 0.001f; // small tolerance for seam cases

    const float aL = a.position.x;
    const float aR = a.position.x + a.size.x;
    const float aT = a.position.y;
    const float aB = a.position.y + a.size.y;

    const float bL = b.position.x;
    const float bR = b.position.x + b.size.x;
    const float bT = b.position.y;
    const float bB = b.position.y + b.size.y;

    return (aL <= bR + eps) && (aR >= bL - eps) &&
        (aT <= bB + eps) && (aB >= bT - eps);
}

// Resolve potential collision between the ball and the paddle
sfx_id handle_collision(bouncing_ball& the_ball, const paddle& the_paddle) {

    if (!is_interacting(the_paddle, the_ball))
        return sfx_id::none;

    // Always bounce up first (no random rotation!)
    //the_ball.move_up();

    // How far from paddle center did we hit? [-1, 1]
    const float dist = std::clamp(
        (the_ball.get_position().x - the_paddle.get_position().x) / the_paddle.get_half_width(),
        -1.0f, 1.0f
    );

    // Set the components of the velocity vector
    the_ball.bounce_from_paddle(dist);

    return sfx_id::ball_paddle;
}


// Resolve potential collision between the ball and a brick
sfx_id handle_collision(bouncing_ball& the_ball, brick& the_brick) {

    // Ignore-latch handling
    if (the_ball.ignore_brick() == &the_brick) {
        if (!is_interacting(the_brick, the_ball)) {
            the_ball.clear_ignore_brick();
        }
        return sfx_id::none;
    }

    if (!is_interacting(the_brick, the_ball))
        return sfx_id::none;

    // Get current ball type
    const ball_type type = the_ball.get_ball_type();

    //-------------------------------
    // Damage phase
    //-------------------------------
    if (!the_brick.is_indestructible()) {

        // Base damage
        the_brick.weaken();

        // Extra effects by ball type
        if (type == ball_type::plasma) {
            the_brick.weaken();
        }
        else if (type == ball_type::antimatter) {
            the_brick.set_strength(0);
        }

        // Destroy if depleted
        if (the_brick.is_too_weak()) {
            the_brick.destroy();
        }
    }
    else {

        // Antimatter destroys even indestructible bricks
        if (type == ball_type::antimatter) {
            the_brick.set_strength(0);
            the_brick.set_indestructible(false);
            the_brick.destroy();
        }
    }

    //-------------------------------
    // Bounce / pass-through decision
    //-------------------------------
    // If brick got destroyed this frame, do not bounce.
    const bool brick_still_solid = the_brick.is_indestructible() || (the_brick.get_strength() > 0);

    // Pass-through rule:
    // plasma/antimatter pass through when brick is not solid after damage
    const bool pass_through =
        (type != ball_type::regular) && !brick_still_solid;

    if (pass_through) {
        // Do NOT separate, do NOT bounce.
        // Just ignore this brick until the ball has moved out of it.
        the_ball.set_ignore_brick(&the_brick);
        return sfx_id::ball_brick;
    }

    // -------------------------
    // Bounce resolution (regular behavior)
    // -------------------------
    const float left_overlap = the_ball.right() - the_brick.left();
    const float right_overlap = the_brick.right() - the_ball.left();
    const float top_overlap = the_ball.bottom() - the_brick.top();
    const float bottom_overlap = the_brick.bottom() - the_ball.top();

    //bool from_left = std::abs(left_overlap) < std::abs(right_overlap);
    //bool from_top = std::abs(top_overlap) < std::abs(bottom_overlap);
    const bool from_left = left_overlap < right_overlap;
    const bool from_top = top_overlap < bottom_overlap;

    const float min_x = from_left ? left_overlap : right_overlap;
    const float min_y = from_top ? top_overlap : bottom_overlap;

    auto pos = the_ball.get_position();
    constexpr float eps = 0.5f;
    const float angle = constants::rotation_angle;

    if (min_x < min_y) {
        pos.x += from_left ? -(min_x + eps) : +(min_x + eps);
        if (from_left) the_ball.move_left(angle);
        else           the_ball.move_right(angle);
    }
    else {
        pos.y += from_top ? -(min_y + eps) : +(min_y + eps);
        if (from_top) the_ball.move_up(angle);
        else          the_ball.move_down(angle);
    }

    the_ball.set_position(pos);
    return sfx_id::ball_brick;
}

// Resolve potential collision between any bonus object and the paddle
bool handle_collision(bonus& the_bonus, const paddle& the_paddle) {
    if (!is_interacting(the_bonus, the_paddle)) return false;
    the_bonus.destroy();
    return true;
}

// Resolve potential collision between the burst ball and the brick
sfx_id handle_collision(ballstorm& the_ball, brick& the_brick) {

    if (!is_interacting(the_brick, the_ball))
        return sfx_id::none;

    // Update the brick's strength
    the_brick.weaken();

    // If very damage brick, destroy it
    if (the_brick.is_too_weak()) {
        the_brick.destroy();
    }

    // Destroy projectile ALWAYS
    the_ball.destroy();

    return sfx_id::ball_brick;

}