#ifndef INTERACTIONS_H
#define INTERACTIONS_H

#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "live.h"

// Determine whether two entities overlap
bool is_interacting(const entity& e1, const entity& e2);

// Resolve potential collision between the ball and the paddle
void handle_collision(ball& b, const paddle& p);

// Resolve potential collision between the ball and the brick
void handle_collision(ball& b, brick& p);

// Resolve potential collision between the live object and the paddle
void handle_collision(live& l, const paddle& p, int& lives);

#endif //INTERACTIONS_H
