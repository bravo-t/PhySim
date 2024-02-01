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
#include "raylib.h"

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

  SetTargetFPS(opts._fps);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

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

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}