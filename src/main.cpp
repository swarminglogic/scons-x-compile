#include <iostream>

#include <SDL2/SDL.h>


void logSDLVersion(std::ostream &out,
                   const std::string& what,
                   const SDL_version& compiled,
                   const SDL_version& linked,
                   std::string revision)
{
  out << what << " Version (Compiled): "
     << static_cast<int>(compiled.major) << "."
     << static_cast<int>(compiled.minor) << "."
     << static_cast<int>(compiled.patch);
  if (!revision.empty())
    out << " (" << revision << ")";
  out << std::endl;

  out << what << " Version (Runtime):  "
      << static_cast<int>(linked.major) << "."
      << static_cast<int>(linked.minor) << "."
      << static_cast<int>(linked.patch)
      << std::endl;
}

int main(int argc, char *argv[])
{
  (void) argc;
  (void) argv;

  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    std::cout << "Failed to initialize SDL" << SDL_GetError() << std::endl;

  std::cout << "Hello SDL!" << std::endl;

  // Display SDL version information
  SDL_version compiled;
  SDL_version linked;
  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  logSDLVersion(std::cout, "SDL", compiled, linked, SDL_GetRevision());
  return 0;
}
