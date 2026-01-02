#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "bonus.h"
#include "audio.h"

// Determine whether two entities overlap
bool is_interacting(const entity& entity1, const entity& entity2);

// Resolve potential collision between the ball and the paddle
sfx_id handle_collision(ball& the_ball, const paddle& the_paddle);

// Resolve potential collision between the ball and the brick
sfx_id handle_collision(ball& the_ball, brick& the_brick);

// Resolve potential collision between the live object and the paddle
bool handle_collision(bonus& the_bonus, const paddle& the_paddle);

#endif //INTERACTIONS_H
#pragma once