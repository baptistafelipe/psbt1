#include <stdio.h>
#include <stdlib.h>
#include "lib_ppm.h"

#define SUB_PIXEL_SIZE 3
#define ZOOM_FACTOR 3
#define SUB_PIXEL_BLACK_THRESHOLD 75
#define SUB_PIXEL_SINGLE_COLOR_THRESHOLD 135
#define MAX_COLOR_INTENSITY 255

// Função para realizar o zoom em uma imagem
void zoom_image(struct image_s *input_image, struct image_s *output_image) {
    int output_width = input_image->width * ZOOM_FACTOR;
    int output_height = input_image->height * ZOOM_FACTOR;

    // Alocando memória para a nova imagem
    if (new_ppm(output_image, output_width, output_height) != 0) {
        printf("Erro ao alocar memória para a imagem de saída.\n");
        exit(EXIT_FAILURE);
    }

    // Iterando sobre os pixels da imagem de entrada
    for (int j = 0; j < input_image->height; j++) {
        for (int i = 0; i < input_image->width; i++) {
            // Obtendo o pixel atual da imagem de entrada
            struct pixel_s current_pixel = input_image->pix[j * input_image->width + i];

            // Iterando sobre os sub-pixels do pixel atual
            for (int y = 0; y < ZOOM_FACTOR; y++) {
                for (int x = 0; x < ZOOM_FACTOR; x++) {
                    // Calculando as coordenadas do sub-pixel na imagem de saída
                    int output_x = i * ZOOM_FACTOR + x;
                    int output_y = j * ZOOM_FACTOR + y;

                    // Obtendo o sub-pixel atual da imagem de saída
                    struct pixel_s *output_pixel = &output_image->pix[output_y * output_width + output_x];

                    // Determinando a cor do sub-pixel de acordo com as regras especificadas
                    if (current_pixel.r <= SUB_PIXEL_BLACK_THRESHOLD) {
                        // Sub-pixel preto
                        output_pixel->r = 0;
                        output_pixel->g = 0;
                        output_pixel->b = 0;
                    } else if (current_pixel.r <= SUB_PIXEL_SINGLE_COLOR_THRESHOLD) {
                        // Sub-pixel com cor única
                        output_pixel->r = (x == 1 && y == 1) ? current_pixel.r : 0;
                        output_pixel->g = (x == 1 && y == 1) ? current_pixel.g : 0;
                        output_pixel->b = (x == 1 && y == 1) ? current_pixel.b : 0;
                    } else {
                        // Sub-pixel com todas as cores
                        output_pixel->r = current_pixel.r;
                        output_pixel->g = current_pixel.g;
                        output_pixel->b = current_pixel.b;
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <imagem_entrada.ppm> <imagem_saida.ppm>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Estrutura para armazenar a imagem de entrada
    struct image_s input_image;

    // Lendo a imagem de entrada
    if (read_ppm(argv[1], &input_image) != 0) {
        printf("Erro ao ler a imagem de entrada.\n");
        return EXIT_FAILURE;
    }

    // Estrutura para armazenar a imagem de saída
    struct image_s output_image;

    // Realizando o zoom na imagem de entrada
    zoom_image(&input_image, &output_image);

    // Escrevendo a imagem de saída
    if (write_ppm(argv[2], &output_image) != 0) {
        printf("Erro ao escrever a imagem de saída.\n");
        return EXIT_FAILURE;
    }

    // Liberando a memória utilizada pelas imagens
    free_ppm(&input_image);
    free_ppm(&output_image);

    return EXIT_SUCCESS;
}
