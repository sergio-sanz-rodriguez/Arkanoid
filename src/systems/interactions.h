#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "bonus.h"
#include "audio.h"

// Determine whether two entities overlap
bool is_interacting(const entity& entity1, const entity& entity2);

// Resolve potential collision between the bouncing ball and the paddle
sfx_id handle_collision(bouncing_ball& the_ball, const paddle& the_paddle);

// Resolve potential collision between the bouncing ball and the brick
sfx_id handle_collision(bouncing_ball& the_ball, brick& the_brick);

// Resolve potential collision between a bonus object and the paddle
bool handle_collision(bonus& the_bonus, const paddle& the_paddle);

// Resolve potenttial collision between the burst ball and the brick
sfx_id handle_collision(burst_ball& the_ball, brick& the_brick);

#endif //INTERACTIONS_H
#pragma once