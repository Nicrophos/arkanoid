#include "physics.h"
#include <math.h>

void normalizeBallSpeed() {
    float currentSpeed = sqrt(ball.velX * ball.velX + ball.velY * ball.velY);
    if (currentSpeed < MIN_BALL_SPEED) {
        float ratio = MIN_BALL_SPEED / currentSpeed;
        ball.velX *= ratio;
        ball.velY *= ratio;
    }
    else if (currentSpeed > MAX_BALL_SPEED) {
        float ratio = MAX_BALL_SPEED / currentSpeed;
        ball.velX *= ratio;
        ball.velY *= ratio;
    }
}