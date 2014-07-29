Pacman 3D
=========

PacMan clone using glut for the 3d rendering

### Keys
- **Arrows**: Move Pacman
- **F1/F2**: Increase/decrease camera X position
- **F3/F4**: Zoom/unzoom
- **F5**: Toggle Pacman view mode
- **F6**: Switch between 3D and 2D rendering modes
- **HOME/END**: Increase/decrease camera Y position
- **PAGE-UP/PAGE-DOWN**: Increase/decrease camera Z position

The controls on the right part of the screen can be clicked to move Pacman or to Reset the game.

### Visual Studio 2013
In order to build the project with Visual Studio 2013, you might need to do the following
- right click on the project, select Properties
- navigate to **Configuration Properties > Linker > Advanced**
- Change the value of **Image Has Safe Exception Handlers** to **NO (/SAFESEH:NO)**
