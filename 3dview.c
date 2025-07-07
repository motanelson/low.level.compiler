#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
//gcc 3dview.c -o 3dview -lSDL2 -lm
#define SCALE 10
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

const SDL_Color vga_colors[16] = {
    {0x00, 0x00, 0x00, 255}, {0x00, 0x00, 0xAA, 255}, {0x00, 0xAA, 0x00, 255}, {0x00, 0xAA, 0xAA, 255},
    {0xAA, 0x00, 0x00, 255}, {0xAA, 0x00, 0xAA, 255}, {0xAA, 0x55, 0x00, 255}, {0xAA, 0xAA, 0xAA, 255},
    {0x55, 0x55, 0x55, 255}, {0x55, 0x55, 0xFF, 255}, {0x55, 0xFF, 0x55, 255}, {0x55, 0xFF, 0xFF, 255},
    {0xFF, 0x55, 0x55, 255}, {0xFF, 0x55, 0xFF, 255}, {0xFF, 0xFF, 0x55, 255}, {0xFF, 0xFF, 0xFF, 255}
};

uint8_t ***voxel_data;
int sizeX, sizeY, sizeZ,sizexx;
double angle = 0;

bool load_voxels(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return false;

    char header[2];
    fread(header, 1, 2, f);
    if (header[0] != '3' || header[1] != 'D') {
        fclose(f);
        return false;
    }

    sizeX = fgetc(f);
    sizexx = fgetc(f);
    sizeY = fgetc(f);
    sizeZ = fgetc(f);

    voxel_data = malloc(sizeZ * sizeof(uint8_t **));
    for (int z = 0; z < sizeZ; z++) {
        voxel_data[z] = malloc(sizeY * sizeof(uint8_t *));
        for (int y = 0; y < sizeY; y++) {
            voxel_data[z][y] = malloc(sizeX);
            fread(voxel_data[z][y], 1, sizeX, f);
        }
    }

    fclose(f);
    return true;
}

void free_voxels() {
    for (int z = 0; z < sizeZ; z++) {
        for (int y = 0; y < sizeY; y++) {
            free(voxel_data[z][y]);
        }
        free(voxel_data[z]);
    }
    free(voxel_data);
}

void rotate_and_draw(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderClear(renderer);

    double cos_a = cos(angle);
    double sin_a = sin(angle);

    int ox = SCREEN_WIDTH / 2;
    int oy = SCREEN_HEIGHT / 2;

    for (int z = 0; z < sizeZ; z++) {
        double depth = 1.0 + (double)z / sizeZ;
        for (int y = 0; y < sizeY; y++) {
            for (int x = 0; x < sizeX; x++) {
                int color = voxel_data[z][y][x];
                if (color == 0) continue;
                double rx = x - sizeX / 2.0;
                double rz = z - sizeZ / 2.0;
                double x_rot = rx * cos_a - rz * sin_a;
                double z_rot = rx * sin_a + rz * cos_a;
                double scale = SCALE * depth;

                int px = (int)(ox + x_rot * scale);
                int py = (int)(oy + (y - sizeY / 2.0) * scale);

                SDL_Color c = vga_colors[color % 16];
                SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, 255);
                SDL_Rect rect = {px, py, (int)scale, (int)scale};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s ficheiro.3d\n", argv[0]);
        return 1;
    }

    if (!load_voxels(argv[1])) {
        printf("Erro ao carregar ficheiro.\n");
        return 1;
    }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Voxel Viewer 3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Uint32 last_time = SDL_GetTicks();
    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        Uint32 current = SDL_GetTicks();
        if (current - last_time > 900) {
            angle += M_PI / 8; // 45 graus
            last_time = current;
        }

        rotate_and_draw(renderer);
        SDL_Delay(16);
    }

    free_voxels();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

