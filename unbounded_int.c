#include"unbounded_int.h"

unbounded_int string2unbounded_int(const char *e) {
  unbounded_int res;
  res.signe = *e;
  res.len = strlen(e);
}