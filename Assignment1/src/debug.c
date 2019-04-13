#include "map.h"
#include <assert.h>

static MapKeyElement copyKeyInt(MapKeyElement n) {
    if (!n) {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(int *) n;
    return copy;
}

/** Function to be used for copying a char as a data to the map */
static MapDataElement copyDataChar(MapDataElement n) {
    if (!n) {
        return NULL;
    }
    char *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(char *) n;
    return (MapDataElement) copy;
}

/** Function to be used by the map for freeing elements */
static void freeInt(MapKeyElement n) {
    free(n);
}

/** Function to be used by the map for freeing elements */
static void freeChar(MapDataElement n) {
    free(n);
}

/** Function to be used by the map for comparing elements */
static int compareInts(MapKeyElement n1, MapKeyElement n2) {
    return (*(int *) n1 - *(int *) n2);
}

int main(){
  int* tmp_key = 1;
  char* tmp_data = "uno";
  Map test = mapCreate(copyKeyInt,copyDataChar,
                       freeChar,freeInt,compareInts);

  printf("[+]Map size:%d\n",mapGetSize(test));
  //mapPut(test,tmp_key,tmp_data);
  //printf("[+]Map size:%d\n",mapGetSize(test));
  return 0;
}
