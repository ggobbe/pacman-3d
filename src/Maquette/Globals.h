/* Global variables/constants */
#pragma once

#define SCALE 5 
#define FPS 10	/* Zoom when in FPS mode */

#define PILL_RADIUS 2  /*The radius of a normal pill */
#define PACMAN_RADIUS 2.25	// The pacman radius
#define GHOST_RADIUS 2.25	// A ghost radius

/*Cell related constants*/
#define WALL_DEPTH_RATIO 0.03

/*Types of cells*/
#define EMPTY 1			/* 0001 */
#define HARD 2			/* 0010 */
#define PILL 5			/* 0101 */
#define SUPER_PILL 9 	/* 1001 */
#define GHOST 6			/* 0110 */
#define PACMAN_POS 16	/* 10000 */
#define NO_PACMAN_POS 15	/* 01111 */

#define SCORE_PILL 5
#define SCORE_SUPER_PILL 25
#define SCORE_GHOST 100
#define LIFE_BEARING 500	/* Bearing before to win a new life */

/* Proportions des viewports */
#define TAILLE_X_VIEWPORT_WORLD 0.5
#define TAILLE_Y_VIEWPORT_WORLD 0.5
#define TAILLE_X_VIEWPORT_INFOS 0.25
#define TAILLE_Y_VIEWPORT_INFOS 0.0666 // 1/15
#define TAILLE_X_VIEWPORT_CONTROLS 0.2
#define TAILLE_Y_VIEWPORT_CONTROLS 0.2

#define NORMAL 0  /*States of Pacman*/
#define SUPER 1 
#define SUPER_TIME 10
#define GHOSTS_PRISON_TIME 3
#define FALSE 0
#define TRUE 1

// Direction du pacman
#define PAC_NORTH 0
#define PAC_EAST 1
#define PAC_SOUTH 2
#define PAC_WEST 3

#define ZOOM_DEFAULT 2

// projection modes
#define PARALLEL_PROJECTION 0
#define PERSPECTIVE_PROJECTION 1
#define PI 3.141592653589793238462643

#define BUFSIZE 255

// buttons
// View button
#define BTN_VIEW_X_LEFT 5
#define BTN_VIEW_X_RIGHT 150
#define BTN_VIEW_Y_DOWN 200
#define BTN_VIEW_Y_UP 240

// Reset button
#define BTN_RESET_X_LEFT 5
#define BTN_RESET_X_RIGHT 150
#define BTN_RESET_Y_DOWN 100
#define BTN_RESET_Y_UP 140

// Close button
#define BTN_CLOSE_X_LEFT 5
#define BTN_CLOSE_X_RIGHT 150
#define BTN_CLOSE_Y_DOWN 150
#define BTN_CLOSE_Y_UP 190

// Up button
#define BTN_NORTH_X_LEFT 50
#define BTN_NORTH_X_RIGHT 85
#define BTN_NORTH_Y_DOWN 30
#define BTN_NORTH_Y_UP 70

// Left button
#define BTN_WEST_X_LEFT 10
#define BTN_WEST_X_RIGHT 50
#define BTN_WEST_Y_DOWN 0
#define BTN_WEST_Y_UP 30

// Down button
#define BTN_SOUTH_X_LEFT 50
#define BTN_SOUTH_X_RIGHT 85
#define BTN_SOUTH_Y_DOWN -40
#define BTN_SOUTH_Y_UP 0

// Right button
#define BTN_EAST_X_LEFT 85
#define BTN_EAST_X_RIGHT 125
#define BTN_EAST_Y_DOWN 0
#define BTN_EAST_Y_UP 30
