#include <iomanip>
#include <iostream>

#define GL_GLEXT_PROTOTYPES 1
#ifdef __WIN32__
#  define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <SDL2/SDL_opengl_glext.h>


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>


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


void logAcquiredGlVersion(std::ostream& out,
                          const std::string& glName)
{
  int minorVersion = 0;
  int majorVersion = 0;
  glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
  glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
  out << "Created " << glName << " context: "
      << majorVersion << "." << minorVersion << std::endl;
}

void logOpenGLContextInfo(std::ostream& out)
{
  // assert(context_ && "Missing OpenGL Context");
  out << std::setw(20) << std::left << "OpenGL Version: "
      << glGetString(GL_VERSION) << std::endl;
  out << std::setw(20) << std::left << "OpenGL GLSL: "
      << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  out << std::setw(20) << std::left << "OpenGL Renderer: "
      << glGetString(GL_RENDERER) << std::endl;
  out << std::setw(20) << std::left << "OpenGL Vendor: "
      << glGetString(GL_VENDOR) << std::endl;

  // supported extensions:
  GLint nExtensions;
  glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
  out << std::setw(20) << std::left << "OpenGL #EXT: "
      << nExtensions << std::endl;

  #ifdef DEBUG_OPENGL_EXT
  for (GLint i = 0 ; i < nExtensions ; ++i) {
    ss << glGetStringi(GL_EXTENSIONS, i);
    out << ss.str() << std::endl;
  }
  #endif
}


void logGraphicsDriverInfo(std::ostream& out)
{
  const int nVideoDrivers = SDL_GetNumVideoDrivers();
  out << "Found " << nVideoDrivers << " video drivers" << std::endl;

  const std::string currentVideoDriver(SDL_GetCurrentVideoDriver());
  for (int i = 0; i < nVideoDrivers; i++) {
    const std::string videoDriver(SDL_GetVideoDriver(i));
    out << "Video Driver #" << i << ": " << videoDriver;
    if (currentVideoDriver == videoDriver)
      out << " (Current)";
    out << std::endl;
  }

  const int nRenderDrivers = SDL_GetNumRenderDrivers();
  out << "Found " << nRenderDrivers << " render drivers" << std::endl;

  SDL_RendererInfo info;
  for (int i = 0 ; i < nRenderDrivers ; ++i) {
    SDL_GetRenderDriverInfo(i, &info);
    out << "Render Driver #" << i << ": " << info.name;

    bool isSoftware      = info.flags & SDL_RENDERER_SOFTWARE;
    bool isHardware      = info.flags & SDL_RENDERER_ACCELERATED;
    bool isVSyncEnabled  = info.flags & SDL_RENDERER_PRESENTVSYNC;
    bool isTargetTexture = info.flags & SDL_RENDERER_TARGETTEXTURE;

    out << "\t [";
    if (isSoftware) out << "SW";
    if (isHardware) out << "HW";
    if (isVSyncEnabled) out << ", VSync";
    if (isTargetTexture) out << ", TT";
    out << "]" << std::endl;
  }
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
  SDL_MIXER_VERSION(&compiled);
  logSDLVersion(std::cout, "SDL_mixer", compiled, *Mix_Linked_Version(), "");
  logSDLMixerMediaInfo(std::cout);


  // Initialize SDL_mixer and display version information
  if (TTF_Init() != 0)
    std::cout << "Failed to initialize SDL_ttf:"
              << SDL_GetError() << std::endl;
  std::cout << "Hello SDL_ttf!" << std::endl;
  SDL_TTF_VERSION(&compiled);
  logSDLVersion(std::cout, "SDL_ttf", compiled, *TTF_Linked_Version(), "");


  // Create a window and OpenGL glContext using SDL and GLEW
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                      SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_Window* window = SDL_CreateWindow("SDL Window",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        640, 480,
                                        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  SDL_GLContext glContext = nullptr;
  const std::pair<int, int> glVersions[11]
     {{4, 4}, {4, 3}, {4, 2}, {4, 1}, {4, 0},
      {3, 3}, {3, 2}, {3, 1}, {3, 0},
      {2, 1}, {2, 0}
  };
  const std::string glName = "OpenGL";
  for (auto& glVersion : glVersions) {
    std::cout << "Trying to create " << glName << " " << glVersion.first << "."
              << glVersion.second << " glContext" << std::endl;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glVersion.first);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glVersion.second);
    glContext = SDL_GL_CreateContext(window);
    if (glContext != nullptr)
      break;
  }
  if (glContext == nullptr)
    std::cout << "Failed to create OpenGL Context " << std::endl;

  bool isOk = SDL_GL_MakeCurrent(window, glContext) <= 0;
  if (!isOk)
    std::cout << "Failed to set OpenGL context" << SDL_GetError() << std::endl;

  glewExperimental = true;
  if (glewInit() != GLEW_OK)
    std::cout << "Failed to initialize GLEW" << std::endl;

  logAcquiredGlVersion(std::cout, glName);
  logOpenGLContextInfo(std::cout);
  logGraphicsDriverInfo(std::cout);

  glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  SDL_GL_SwapWindow(window);

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
  SDL_GL_DeleteContext(glContext);
  IMG_Quit();
  const int nOpenAudio = Mix_QuerySpec(nullptr, nullptr, nullptr);
  for (int i = 0 ; i < nOpenAudio ; ++i)
    Mix_CloseAudio();
  while (Mix_Init(0))
    Mix_Quit();
  TTF_Quit();
  SDL_Quit();
  return 0;
}
