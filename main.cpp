#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "color.h"
#include "framebuffer.h"
#include "render.h"
#include "vertexGML.h"


using namespace std;

// Dimensiones de la ventana
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}


void render(SDL_Renderer* renderer) {
        // Definir los vértices del cubo
    std::vector<glm::vec3> vertices = {
        glm::vec3(1.125986, 0.781798, -1.058748),
        glm::vec3(0.074502, -0.860990, -1.501048),
        glm::vec3(1.568286, 0.015753, 0.735004),
        glm::vec3(0.516802, -1.627034, 0.292704),
        glm::vec3(-0.516802, 1.627034, -0.292704),
        glm::vec3(-1.568286, -0.015753, -0.735004),
        glm::vec3(-0.074502, 0.860990, 1.501048),
        glm::vec3(-1.125986, -0.781798, 1.058748)
    };

    // Definir las caras del cubo
    std::vector<Face> faces = {
        Face{ {{5, 1, 1}, {3, 2, 1}, {1, 3, 1}} },
        Face{ {{3, 2, 2}, {8, 4, 2}, {4, 5, 2}} },
        Face{ {{7, 6, 3}, {6, 7, 3}, {8, 8, 3}} },
        Face{ {{2, 9, 4}, {8, 10, 4}, {6, 11, 4}} },
        Face{ {{1, 3, 5}, {4, 5, 5}, {2, 9, 5}} },
        Face{ {{5, 12, 6}, {2, 9, 6}, {6, 7, 6}} },
        Face{ {{5, 1, 1}, {7, 13, 1}, {3, 2, 1}} },
        Face{ {{3, 2, 2}, {7, 14, 2}, {8, 4, 2}} },
        Face{ {{7, 6, 3}, {5, 12, 3}, {6, 7, 3}} },
        Face{ {{2, 9, 4}, {4, 5, 4}, {8, 10, 4}} },
        Face{ {{1, 3, 5}, {3, 2, 5}, {4, 5, 5}} },
        Face{ {{5, 12, 6}, {1, 3, 6}, {2, 9, 6}} }
    };

    // Dibujar el cubo triangulando las caras
    triangulateAndDrawCube(vertices, faces);
    // Render the framebuffer to the screen
    renderBuffer(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("Cube", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    bool running = true;
    SDL_Event event;
        // Clear the framebuffer
    clear();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Call our render function
        render(renderer);

        // Present the frame buffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
