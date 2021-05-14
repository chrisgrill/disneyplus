#Disney Streaming Services Submission

##Requirements
  - Create a screen that consumes the home page API and renders the pre-populated data onto the screen.
  - The focused tile must be scaled up. 
  -  The app should support navigation similar to a remote control, e.g. up/down/left/right/enter/back/etc. Avoid mouse input.
  -  Minimum layout should be multiple rows of data, but please feel free to add in your own design ideas as well!

##Description
###Language
C++14

###Libraries Used
- libcurl: Used for pulling data over HTTP
- SDL2: OpenGL wrapper
- SDL2_ttf: Text rendering for SDL2
- nlohmann::json: Single include JSON functionality
- stb_image: Single include image loading library

###Solution Description
####Class Descriptions
- DisneyCurl: A wrapper around libcurl functionality. Two static methods for loading JSON data and image data.
- DisneyImage: Uses stb_image library to load image pixel data and then uses SDL functions to create an SDL surface.
- DisneyTile: A data class for managing tile information including image, title, rating, and background image url.
- DisneySet: Manages a set of tiles and includes functionality for navigating a tile set(rotate), loading a background image, and placing text.

####Program Flow
1. Initialize SDL components such as the window and font.
2. Load JSON data and populate tile sets.
3. Start the SDL event loop.
4. Check for keydown events. If a navigation key is pressed, the sets are rotated. This makes the upper-leftmost tile the selected tile.
5. Background image is loaded from selected tile background image url.
6. Background image is copied to the renderer.
7. Text from the tile (title, rating) is copied to the renderer over the background image.
8. Tile sets are rendered.


####Compared to Requirements
- The page data is loaded from the JSON data provided the endpoint.
- I was having issues with the layout when trying to scale the selected tile. Because they were rendered in order and how I handled navigation,
  the selected tile
was rendered first. In SDL there is no way to specify a z-order to move it above other tiles. Any scaling caused it to render below the other tiles. Instead of scaling the tile, I changed the layout to display
  a background image that is loaded dynamically when the tile is selected.
- The app supports up/down/left/right navigation. If this were a normal project, I would try to handle navigation a bit differently and have the selection box move through the grid of tiles.
In this case, I used a shortcut and shuffled the elements to put the selected tile in 0,0 position so it could be easily worked with. This made the wrap around tile shifting easy using std::rotate.
###Improvements
- Unit tests: In a normal project, I would use a unit testing framework like GTest. Based on the additional time needed to get everything building as a standalone project, I chose to leave them out.
- In the interest of time, some of the code is directly from examples from the associated library (e.g libcurl). In a normal project, I would pay a little more attention to that code and make sure there were no issues.
  This also led to some differences in code style in some areas. In a normal work setting I am happy to conform to the organizations style.
- Set text for each row of tiles. This was another area where as a novice with SDL I was having issues with rendering a functional layout.
- Asyncronous image loading. I tried to do this using std::future functionality but was having trouble with things rendering correctly.

###Building/Running
The included archive has a precompiled binary in the build directory.

It also has the directory structure set up for building. 
1. Change directory to `build/`
2. Run `cmake ..`
3. Run `make`
4. Run `./disneyplus`

One thing to note, on my machine I was able to build and run without the symbolic link to the SDL object. When 
I built it on another system, I had trouble with it not finding the object so I symlinked into the build folder.