#include <SDL2/SDL.h>

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT  720

#define NUM_COLS    80
#define NUM_ROWS    45

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t     u8;
typedef uint32_t    u32;
typedef uint64_t    u64;
typedef int32_t     i32;
typedef int64_t     i64;

#define internal static
#define local_persist static
#define global_variable static

#include "pt_console.c"

typedef struct {
    u8 pos_x;
    u8 pos_y;
} Player;

global_variable Player player;

void render_screen(SDL_Renderer *renderer, SDL_Texture *screen, PT_Console *console) {

    PT_ConsoleClear(console);

    PT_ConsolePutCharAt(console, '@', player.pos_x, player.pos_y, 0xFFFFFFFF, 0x000000FF);

    SDL_UpdateTexture(screen, NULL, console->pixels, SCREEN_WIDTH * sizeof(u32));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screen, NULL, NULL);;
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    (void) argc; /* unused */
    (void) argv; /* unused */

    // The window we'll be rendering to
    SDL_Window* window = NULL;

    // Initialize SDL
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        printf( "SDL could not initialize! Error: %s\n", SDL_GetError() );
        return -1;
    }

    window = SDL_CreateWindow(
        "HARK!",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0
    );

    if ( window == NULL ) {
        printf( "Could not create window! Error: %s\n", SDL_GetError() );
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Texture *screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    PT_Console *console = PT_ConsoleInit(SCREEN_WIDTH, SCREEN_HEIGHT, NUM_ROWS, NUM_COLS);

    PT_ConsoleSetBitmapFont(console, "terminal16x16.png", 0, 16, 16);

    player.pos_x = 25;
    player.pos_y = 25;

    bool done = false;
    while (!done) {

        SDL_Event event;
        while(SDL_PollEvent(&event) != 0) {

            if (event.type == SDL_QUIT) {
                done = true;
                break;

            }

            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                switch (key) {
                    case SDLK_ESCAPE:
                        done = true;
                        break;
                    case SDLK_UP:
                    case SDLK_k:
                        if (player.pos_y > 0) {
                            player.pos_y -= 1;
                        }
                        break;
                    case SDLK_DOWN:
                    case SDLK_j:
                        if (player.pos_y < NUM_ROWS - 1) {
                            player.pos_y += 1;
                        }
                        break;
                    case SDLK_LEFT:
                    case SDLK_h:
                        if (player.pos_x > 0) {
                            player.pos_x -= 1;
                        }
                        break;
                    case SDLK_RIGHT:
                    case SDLK_l:
                        if (player.pos_x < NUM_COLS - 1) {
                            player.pos_x += 1;
                        }
                        break;
                    case SDLK_y:
                        if (player.pos_x > 0 && player.pos_y > 0) {
                            player.pos_x -= 1;
                            player.pos_y -= 1;
                        }
                        break;
                    case SDLK_u:
                        if (player.pos_x < NUM_COLS - 1 && player.pos_y > 0) {
                            player.pos_x += 1;
                            player.pos_y -= 1;
                        }
                        break;
                    case SDLK_b:
                        if (player.pos_x > 0 && player.pos_y < NUM_ROWS - 1) {
                            player.pos_x -= 1;
                            player.pos_y += 1;
                        }
                        break;
                    case SDLK_n:
                        if (player.pos_x < NUM_COLS - 1 && player.pos_y < NUM_ROWS - 1) {
                            player.pos_x += 1;
                            player.pos_y += 1;
                        }
                        break;
                    default:
                        break;
                }
            }

      }

      render_screen(renderer, screen, console);

    }

    // Clean up SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Shutdown SDL
    SDL_Quit();

    return 0;
}
