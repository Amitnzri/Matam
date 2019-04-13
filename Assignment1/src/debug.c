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


static void log()
{
  static int counter =0;
  counter ++;
  printf("[?]Log %d: ",counter);
}

int main(){
  int keys[] = {1,2};
  char data[] = {'A','B'};
  Map map = mapCreate(copyKeyInt,copyDataChar,
                       freeChar,freeInt,compareInts);
  mapPut(map,keys,data);
  printf("[+] Key:%d\nData:%c\n",keys[0], *(char*)(mapGet(map,keys)));
  return 0;
}
