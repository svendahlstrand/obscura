#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define FIRST_PHOTO_POSITION 0x2000
#define PHOTO_OFFSET 0x1000
#define PHOTO_TILE_WIDTH 16
#define PHOTO_TILE_HEIGHT 14
#define TILE_SIDES 8
#define MAX_FILE_NAME_LENGTH 262

// Returns imageraster index given a tile index and x and y coordinates.
uint16_t imageraster_pixel_index_from_tile(uint8_t tile_index, uint8_t x, uint8_t y) {
  uint8_t image_x = x + (tile_index % PHOTO_TILE_WIDTH) * TILE_SIDES;
  uint8_t image_y = y + (tile_index / PHOTO_TILE_WIDTH) * TILE_SIDES;

  return  PHOTO_TILE_WIDTH * TILE_SIDES * image_y + image_x;
}

// Takes a Game Boy Camera save RAM file and photo index and populates the
// provided image raster with pixels. Valid index is between 0 and 29.
void imageraster_from_game_boy_save_ram(FILE* save_file, uint8_t *imageraster, uint8_t photo_index) {
  char tile[16];

  fseek(save_file, FIRST_PHOTO_POSITION + (PHOTO_OFFSET * photo_index), 0);

  for (size_t i = 0; i < PHOTO_TILE_WIDTH * PHOTO_TILE_HEIGHT * 2; i += 2) {
    fread(tile, 1, sizeof tile, save_file);

    uint8_t pixel_value;

    for (size_t j = 0, y = 0; j < 16; j += 2, y++) {
      for (size_t k = 0, x = 7; k < 8; k++, x--) {
        pixel_value = ((tile[j] >> k) & 0x01) + (((tile[j + 1] >> k) & 0x01) << 1);

        pixel_value = (pixel_value - 3) * -1;

        imageraster[imageraster_pixel_index_from_tile(i / 2, x, y)] = pixel_value;
      }
    }
  }
}

// Creates and initializes a PGM file for writing, indicated by filename and
// returns a pointer to the file stream.
// filename and postfix can be at most 256 characters long together.
FILE *pgm_open_and_initialize(char filename[], uint8_t postfix) {
  char full_name[MAX_FILE_NAME_LENGTH];
  sprintf(full_name, "%s-%d.pgm", filename, postfix);

  FILE* image = fopen(full_name, "w+");

  fputs("P2\n", image);
  fprintf(image, "%d %d\n", PHOTO_TILE_WIDTH * TILE_SIDES, PHOTO_TILE_HEIGHT * TILE_SIDES);
  fputs("3\n", image);

  return image;
}

// Writes an image ("image-<photo_index>.pgm") to disk base on the provided
// image raster.
void pgm_from_imageraster(uint8_t *imageraster, uint8_t photo_index) {
  FILE* image = pgm_open_and_initialize("image", photo_index + 1);

  for (size_t i = 0; i < PHOTO_TILE_WIDTH * PHOTO_TILE_HEIGHT * TILE_SIDES * TILE_SIDES; i++) {
    fprintf(image, "%d ", imageraster[i]);

    if (i % (PHOTO_TILE_WIDTH * TILE_SIDES) == PHOTO_TILE_WIDTH * TILE_SIDES - 1) {
      fputs("\n", image);
    }
  }

  fclose(image);
}

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <file>\n", argv[0]);
    exit(0);
  }

  FILE* save_file = fopen(argv[1], "r");

  if (save_file == NULL) {
    fprintf(stderr, "Could not open file '%s'.\n", argv[1]);
    exit(1);
  }

  uint8_t imageraster[PHOTO_TILE_WIDTH * PHOTO_TILE_HEIGHT * TILE_SIDES * TILE_SIDES];

  for (size_t i = 0; i < 30; i++) {
    imageraster_from_game_boy_save_ram(save_file, imageraster, i);
    pgm_from_imageraster(imageraster, i);
  }

  fclose(save_file);

  return 0;
}
