/* === V2 ===                                                                 */
/* In order not to calculate strlen(*dest) the length will be kept before the */
/* string. sizeof(size_t) bytes are wasted per string.                        */
/* Best usecase: Long strings or string with large variance of sizes.         */

#include <stdlib.h>
#include <string.h>
 
void str_free(char** ptr) {
  if (*ptr) free(*ptr - sizeof(size_t));
}
 
char* str_cpy(char** dest, const char* source) {
  if (source) {
    size_t src_len = strlen(source);
    size_t* result = malloc(src_len + 1 + sizeof(size_t));
    if (!result) return NULL;
    *result++ = src_len;
    memcpy(result, source, src_len + 1);
    str_free(dest);
    *dest = (char*)result;
  }
  return *dest;
}
 
char* str_cat(char** dest, const char* source) {
  if (!*dest) return str_cpy(dest, source);
  if (source) {
    char* orig_dest = *dest;
    size_t dst_len = *((size_t*)orig_dest - 1);
    size_t src_len = strlen(source);
    char* result = realloc(*dest - sizeof(size_t),
                           dst_len + src_len + 1 + sizeof(size_t));
    if (!result) return NULL;
    *(size_t*)result += src_len;
    result += sizeof(size_t);
    if (source >= *dest && source <= *dest + dst_len)
      source += result - *dest;
    memcpy(result + dst_len, source, src_len + 1);
    *dest = result;
  }
  return *dest;
}
