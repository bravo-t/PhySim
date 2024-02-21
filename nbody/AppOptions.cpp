#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include "AppOptions.h"

namespace NBody {

AppOptions
processOptions(int argc, char** argv) 
{
  AppOptions opts;
  int c;
  while ((c = getopt (argc, argv, "w:h:f:r:t:s")) != -1) {
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
      case 'r':
        opts._warpTick = atoi(optarg);
        break;
      case 't':
        opts._simTick = atoi(optarg);
        break;
      case 's':
        opts._circleSize = atoi(optarg);
        break;
      case '?':
        if (optopt == 'w' || optopt == 'h' || optopt == 'f' || optopt == 'r' || optopt == 's')
          fprintf (stderr, "Option -w/-h/-f/-r/-s/-t requires an argument.\n", optopt);
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
}