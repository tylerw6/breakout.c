//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//
 
// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
 
// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"
 
// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400
 
// number of rows of bricks
#define ROWS 5
 
// number of columns of bricks
#define COLS 10
 
// radius of ball in pixels
#define RADIUS 10
 
// lives
#define LIVES 5
 
// paddle and ball height and width
int paddlex = 25;
int paddley = 10;
int ballx = 20;
 
// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GRect initPaddlel(GWindow window);
GRect initPaddler(GWindow window);
GLabel initScoreboard(GWindow window);
GLabel initLife(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
void updateLife(GWindow window, GLabel life, int lives);
GObject detectCollision(GWindow window, GOval ball);
 
int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));
 
    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);
 
    // instantiate bricks
    initBricks(window);
 
    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);
 
    // instantiate paddles, centered at bottom of window
    GRect paddle = initPaddle(window);
    GRect paddler = initPaddler(window);
    GRect paddlel = initPaddlel(window);
 
    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    GLabel life = initLife(window);
 
    // number of bricks initially
    int bricks = COLS * ROWS;
 
    // number of lives initially
    int lives = LIVES;
 
    // number of points initially
    int points = 0;
     
    // speed of ball initially
    double speedx = (drand48() - .5) * 1.5;
    double speedy = 1;
     
    // start upon Click
    waitForClick();
     
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        move(ball, speedx, speedy);
        GObject object = detectCollision(window, ball);
         
        // ramp up speed as score increases
        if (speedy > 0)
        {
            speedy = ((points) / 10) + 2;
        }
        else if (speedy < 0)
        {
            speedy = -((points) / 10) - 2;
        }
        // bounce ball off left, right or center of paddle 
        // and change speed accordingly
        if (object != NULL)
        {
            if (object == paddle)
            {
                speedy = -speedy;
            }
            else if (object == paddler)
            {
                speedy = -speedy;
                speedx = speedx + 1;
            }
            else if (object == paddlel)
            {
                speedy = -speedy;
                speedx = speedx - 1;
            }
            // if a brick is hit, give points, minus bricks
            else if (strcmp(getType(object), "GRect") == 0)
            {
                speedy = -speedy;
                removeGWindow(window, object);
                points++;
                bricks--;
                updateScoreboard(window, label, points);
            }
        }
         
        // bounce off right edge of window
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            speedx = -speedx;
        }
 
        // bounce off left edge of window
        else if (getX(ball) <= 0)
        {
            speedx = -speedx;
        }
         
        // bounce off top edge of window
        else if (getY(ball) <= 0)
        {
            speedy = -speedy;
        }
         
        // reset game for new life
        else if (getY(ball) >= getHeight(window))
        {
            lives--;
            removeGWindow(window, paddle);
            removeGWindow(window, paddler);
            removeGWindow(window, paddlel);
            paddle = initPaddle(window);
            paddler = initPaddler(window);
            paddlel = initPaddlel(window);
            ball = initBall(window);
            updateLife(window, life, lives);
            speedx = (drand48() - .5) * 1.5;
            waitForClick();
        }
         
        // move paddle concurrent with mouse
        GEvent event = getNextEvent(MOUSE_EVENT);
        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - getWidth(paddle) / 2;
                setLocation(paddle, x, 550);
                setLocation(paddler, x + 25, 550);
                setLocation(paddlel, x - 25, 550); 
            }
        }
         
        // linger before moving again
        pause(10);
    }
 
    // wait for click before exiting
    waitForClick();
 
    // game over
    closeGWindow(window);
    return 0;
}
 
/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    // for any added rows or future levels add bricks in this sequence of colors
     
    char* color[5] = {"RED", "ORANGE", "YELLOW", "GREEN", "BLUE"};
     
    for (int i = 0; i < ROWS; i++)
    {
        int y = 30 + 13 * i;
         
        for (int j = 0; j < COLS; j++)
        {
            int x = 7 + 39 * j;
            GRect brick = newGRect(x, y, 36, 10);
            setFilled(brick, true);
            setColor(brick, color[i % 5]);
            add(window, brick);
        }
    }
}
 
/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // TODO
    GOval ball = newGOval(190, 300, ballx, ballx);
    setFilled(ball, true);
    setColor(ball, "DARK_GRAY");
    add(window, ball);
    return ball;
}
 
/**
 * Instantiates 3 paddles in bottom-middle of window.
 */
 GRect initPaddler(GWindow window)
{
    // changes ball speed in +x direction
    GRect paddler = newGRect(200, 550, paddlex, paddley);
    setFilled(paddler, true);
    setColor(paddler, "BLACK");
    add(window, paddler);
   return paddler;
}
GRect initPaddlel(GWindow window)
{
    // changes ball speed in -x direction
    GRect paddlel = newGRect(150, 550, paddlex, paddley);
    setFilled(paddlel, true);
    setColor(paddlel, "BLACK");
    add(window, paddlel);
     
   return paddlel;
}
GRect initPaddle(GWindow window)
{
    // no change in ball speed
     
    GRect paddle = newGRect(175, 550, paddlex, paddley);
    setFilled(paddle, true);
    setColor(paddle, "BLACK");
    add(window, paddle);
     
   return paddle;
}
 
// Keeps track of life left
GLabel initLife(GWindow window)
{
    char st[12];
    sprintf(st, "%i", LIVES);
    GLabel life = newGLabel(st);
    double x = getWidth(window) - 15;
    setLocation(life, x, 15);
    add(window, life);
    return life;
}
 
void updateLife(GWindow window, GLabel life, int lives)
{
    char st[12];
    sprintf(st, "%i", lives);
    setLabel(life, st);
}
 
/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    // TODO
    GLabel label = newGLabel("0");
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
    add(window, label);
    return label;
}
 
/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);
 
    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}
 
/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);
 
    // for checking for collisions
    GObject object;
 
    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }
 
    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }
 
    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }
 
    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }
 
    // no collision
    return NULL;
}
