#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>


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


void logSDLMixerMediaInfo(std::ostream& out)
{
  // Write music decoder information
  const int nMusicDecoders = Mix_GetNumMusicDecoders();
  out << "Music decoders (" << nMusicDecoders << "): ";
  if (nMusicDecoders > 0)
    out << Mix_GetMusicDecoder(0);
  for (int i = 1 ; i < nMusicDecoders ; ++i)
    out << ", " << Mix_GetMusicDecoder(i);
  out << std::endl;

  // Write audio decoder information
  const int nChunkDecoders =  Mix_GetNumChunkDecoders();
  out << "Audio decoders (" << nChunkDecoders << "): ";
  if (nChunkDecoders > 0)
    out << Mix_GetChunkDecoder(0);
  for (int i = 1 ; i < nChunkDecoders ; ++i) {
    out << ", " << Mix_GetChunkDecoder(i);
  }
  out << std::endl;
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


  // Initialize SDL_image and display version information
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  int imgFlagsInit = IMG_Init(imgFlags);
  if ((imgFlagsInit & imgFlags) != imgFlags)
    std::cout << "Failed to initialize SDL_image:"
              << IMG_GetError() << std::endl;
  std::cout << "Hello SDL_image!" << std::endl;
  SDL_IMAGE_VERSION(&compiled);
  logSDLVersion(std::cout, "SDL_image", compiled, *IMG_Linked_Version(), "");


  // Initialize SDL_mixer and display version information
  int mixFlags = MIX_INIT_OGG;
  int mixFlagsInit = Mix_Init(mixFlags);
  if ((mixFlagsInit & mixFlags) != mixFlags) {
    std::cout << "Failed to initialize SDL_mixer"
              << Mix_GetError() << std::endl;
  }
  if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
    std::cout << "Failed to acquire sound device"
              << Mix_GetError() << std::endl;
  }
  std::cout << "Hello SDL_mixer!" << std::endl;
  // Write version information to log
  SDL_MIXER_VERSION(&compiled);
  logSDLVersion(std::cout, "SDL_mixer", compiled, *Mix_Linked_Version(), "");
  logSDLMixerMediaInfo(std::cout);

  // Create a window and renderer using SDL
  SDL_Window* window = SDL_CreateWindow("SDL Window",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        240, 120,
                                        SDL_WINDOW_SHOWN);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawColor(renderer, 20, 40, 60, 255);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  // Wait for user event before closing
  bool isRunning = true;
  SDL_Event event;
  while (isRunning) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_KEYDOWN &&
          event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        isRunning = false;
      else if (event.type == SDL_QUIT)
        isRunning = false;
    }
    SDL_Delay(30);
  }

  // Cleanup
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  IMG_Quit();
  const int nOpenAudio = Mix_QuerySpec(nullptr, nullptr, nullptr);
  for (int i = 0 ; i < nOpenAudio ; ++i)
    Mix_CloseAudio();
  while (Mix_Init(0))
    Mix_Quit();
  SDL_Quit();
  return 0;
}
