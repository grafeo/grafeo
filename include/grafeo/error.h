#ifndef GRAFEO_ERROR_H
#define GRAFEO_ERROR_H

typedef struct _Error
{
  uint8_t  domain;
  uint16_t code;
  uint8_t *message;
}Error;

Error* error_new_with_msg(const char* msg);


#endif
