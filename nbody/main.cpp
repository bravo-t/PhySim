/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2024 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include <cctype>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "AppOptions.h"
#include "raylib.h"
#include "Universe.h"
#include "Simulator.h"
#include "Rasterize.h"

/// There seems to have some issues with Intel integrated graphics cards with raylib
/// When multiple graphic cards exists in the system
/// So adding below to force discrete GPU
extern "C"
{
  __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
  __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
  ::NBody::AppOptions opts = ::NBody::processOptions(argc, argv);
  if (opts._worldFile == nullptr) {
    exit(1);
  }
  const int screenWidth = opts._screenWidth;
  const int screenHeight = opts._screenHeight;

  printf("Simulating %s with window size %dx%d\n", opts._worldFile, opts._screenHeight, opts._screenWidth);

  InitWindow(screenWidth, screenHeight, opts._worldFile);

  Camera2D camera = {0};
  camera.zoom = 1.0f;
  camera.offset = {0, 0};
  camera.target = {0, 0};

  RenderTexture2D renderData = LoadRenderTexture(screenWidth, screenHeight);

  // Dynamic memory allocation to store pixels data (Color type)
  Color *pixels = (Color *)malloc(screenWidth*screenHeight*sizeof(Color));
    
  /*Image pixelData = {
    .data = pixels,             // We can assign pixels directly to data
    .width = screenWidth,
    .height = screenHeight,
    .mipmaps = 1,
    .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
  };

  Texture2D renderData = LoadTextureFromImage(pixelData);
  */ 
  SetTargetFPS(opts._fps);

  ::NBody::Universe universe(opts._worldFile);
  ::NBody::Simulator simulator(&universe, opts._simMethod);
  simulator.setSimTick(opts._simTick);
  simulator.setBHTheta(opts._simBHTheta);

  size_t warpTick = opts._warpTick;

  printf("Starting simulation with warpTick = %lu, simTickPerSecond = %.3f\n", warpTick, opts._simTick);
  while (!WindowShouldClose()) {
    for (size_t w=0; w<warpTick; ++w) {
      universe.simulate(simulator);
    }

    ::NBody::drawFrame(screenWidth, screenHeight, universe, renderData, camera, opts._circleSize);
    //::NBody::rasterize(opts._screenWidth, opts._screenHeight, universe, pixels);

    BeginDrawing();
      ClearBackground(BLACK);

      BeginMode2D(camera);
        DrawTexture(renderData.texture, 0, 0, WHITE);
      EndMode2D();

      DrawFPS(GetScreenWidth() - 95, 10);
    EndDrawing();
  }

  CloseWindow(); 

  return 0;
    /*
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [textures] example - texture from raw data");

    // NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)

    // Generate a checked texture by code
    int width = 960;
    int height = 480;

    // Dynamic memory allocation to store pixels data (Color type)
    Color *pixels = (Color *)malloc(width*height*sizeof(Color));

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (((x/32+y/32)/1)%2 == 0) pixels[y*width + x] = ORANGE;
            else pixels[y*width + x] = GOLD;
        }
    }

    // Load pixels data into an image structure and create texture
    Image checkedIm = {
        .data = pixels,             // We can assign pixels directly to data
        .width = width,
        .height = height,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    };

    Texture2D checked = LoadTextureFromImage(checkedIm);
    UnloadImage(checkedIm);         // Unload CPU (RAM) image data (pixels)
    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawTexture(checked, screenWidth/2 - checked.width/2, screenHeight/2 - checked.height/2, Fade(WHITE, 0.5f));

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadTexture(checked);     // Texture unloading

    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
    */
}

/*

struct InvokeOptions {
  int _screenWidth = 800;
  int _screenHeight = 450;
  int _fps = 60;
  const char* _worldFile = nullptr;
};

static InvokeOptions
processOptions(int argc, char** argv) 
{
  InvokeOptions opts;
  int c;
  while ((c = getopt (argc, argv, "w:h:f:")) != -1) {
    switch (c) {
      case 'w':
        opts._screenWidth = atoi(optarg);
        break;
      case 'h':
        opts._screenHeight = atoi(optarg);
        break;
      case 'f':
        opts._fps = atoi(optarg);
        break;
      case '?':
        if (optopt == 'w' || optopt == 'h' || optopt == 'f')
          fprintf (stderr, "Option -w/-h/-f requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
      default:
        abort ();
    }
  }

  if (optind < argc) {
    opts._worldFile = argv[optind];
  } else {
    printf("ERROR: Name of world file is required\n");
  }
  return opts;
}
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int 
main(int argc, char** argv)
{
  InvokeOptions opts = processOptions(argc, argv);
  if (opts._worldFile == nullptr) {
    exit(1);
  }
  const int screenWidth = opts._screenWidth;
  const int screenHeight = opts._screenHeight;

  printf("Simulating %s with window size %dx%d\n", opts._worldFile, opts._screenHeight, opts._screenWidth);

  InitWindow(screenWidth, screenHeight, opts._worldFile);

  // Dynamic memory allocation to store pixels data (Color type)
  Color *pixels = (Color *)malloc(screenWidth*screenHeight*sizeof(Color));
  
  SetTargetFPS(opts._fps);

  ::NBody::Universe universe(opts._worldFile);
  ::NBody::Simulator simulator;

  printf("Start simulation\n");
  while (!WindowShouldClose()) {
    universe.simulate(simulator);
    ::NBody::rasterize(opts._screenWidth, opts._screenHeight, universe, pixels);

    pixels[0] = RED;
    pixels[1] = RED;
    pixels[2] = RED;
    pixels[3] = RED;
    
    
    Image pixelData = {
        .data = pixels,             // We can assign pixels directly to data
        .width = screenWidth,
        .height = screenHeight,
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    };

    Texture2D renderData = LoadTextureFromImage(pixelData);

    BeginDrawing();
    
    DrawTexture(renderData, 0, 0, WHITE);
    //ClearBackground(BLACK);

    DrawFPS(screenWidth - 100, 10);

    EndDrawing();
  }

  CloseWindow(); 

  return 0;
}

*/