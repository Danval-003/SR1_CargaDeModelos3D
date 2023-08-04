#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "color.h"
#include "framebuffer.h"
#include "render.h"
#include "vertexGML.h"
#include "objSettings.h"


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
    std::vector<glm::vec3> vertices_lo;

    // Definir las caras del cubo

    std::vector<glm::vec3> vertices_out;

    // Definir las caras del cubo
    std::vector<Face> faces_out;
    std::vector<Face> out_faces;
    std::vector<glm::vec3> out_vertices;


    if(!loadOBJ("../navecita.obj", out_vertices, out_faces)){

        cout<<"No funciono"<<endl;
    }

    triangulateFaces(out_faces, out_vertices, vertices_out, faces_out);

    // Dibujar el cubo triangulando las caras
    triangulateAndDrawCube(vertices_out, faces_out);
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
