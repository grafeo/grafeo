#ifndef GRAFEO_IMAGE_H
#define GRAFEO_IMAGE_H

Array* image_read(const char* filename);
Array* image_read_png(const char* filename);
Array* image_read_jpg(const char* filename);

void image_write(Array* array, const char* filename);
void image_write_png(Array* array, const char* filename);
void image_write_jpg(Array* array, const char* filename);

#endif

