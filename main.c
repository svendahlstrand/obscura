#include <stdio.h>
#include <stdlib.h>

#define FIRST_PHOTO_POSITION 0x2000
#define PHOTO_OFFSET 0x1000
#define PHOTO_TILE_WIDTH 16
#define PHOTO_TILE_HEIGHT 14
#define TILE_SIDES 8

uint16_t get_pixel_index_from_tile(uint8_t tile_index, uint8_t x, uint8_t y) {
  uint8_t image_x = x + (tile_index % PHOTO_TILE_WIDTH) * TILE_SIDES;
  uint8_t image_y = y + (tile_index / PHOTO_TILE_WIDTH) * TILE_SIDES;

  return  PHOTO_TILE_WIDTH * TILE_SIDES * image_y + image_x;
}

void write_image_from_save(FILE* save_file, uint8_t number) {
  char filename[13];

  snprintf(filename, sizeof filename, "image-%d.pgm", number + 1);

  FILE* image = fopen(filename, "w+");

  fseek(save_file, FIRST_PHOTO_POSITION + (PHOTO_OFFSET * number), 0);

  uint8_t imageraster[PHOTO_TILE_WIDTH * PHOTO_TILE_HEIGHT * TILE_SIDES * TILE_SIDES];

  fputs("P2\n", image);
  fprintf(image, "%d %d\n", PHOTO_TILE_WIDTH * TILE_SIDES, PHOTO_TILE_HEIGHT * TILE_SIDES);
  fputs("3\n", image);

  char tile[16];

  for (size_t i = 0; i < PHOTO_TILE_WIDTH * PHOTO_TILE_HEIGHT * 2; i += 2) {
    fread(tile, 1, sizeof tile, save_file);

    uint8_t pixel_value;

    for (size_t j = 0, y = 0; j < 16; j += 2, y++) {
      for (size_t k = 0, x = 7; k < 8; k++, x--) {
        pixel_value = ((tile[j] >> k) & 0x01) + (((tile[j + 1] >> k) & 0x01) << 1);

        pixel_value = (pixel_value - 3) * -1;

        imageraster[get_pixel_index_from_tile(i / 2, x, y)] = pixel_value;
      }
    }
  }

  for (size_t i = 0; i < PHOTO_TILE_WIDTH * PHOTO_TILE_HEIGHT * TILE_SIDES * TILE_SIDES; i++) {
    fprintf(image, "%d ", imageraster[i]);

    if (i % (PHOTO_TILE_WIDTH * TILE_SIDES) == PHOTO_TILE_WIDTH * TILE_SIDES - 1) {
      fputs("\n", image);
    }
  }

  fclose(image);
}

int main(int argc, char const *argv[]) {
  FILE* save_file = fopen(argv[1], "r");

  for (size_t i = 0; i < 30; i++) {
    write_image_from_save(save_file, i);
  }

  fclose(save_file);

  return 0;
}
