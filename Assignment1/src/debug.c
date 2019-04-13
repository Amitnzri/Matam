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

static void log(){
  static int counter =0;
  counter ++;
  printf("[?]Log %d: ",counter);
}

static void printKeysByOrder(Map map){
  assert(map);
  int counter =1;
  MapKeyElement key = mapGetFirst(map);
  while(key){
    printf("[+]Key %d is: %d\n",counter,*(int*)key);
    key = mapGetNext(map);
    counter++;
  }
}

int main(){
  int keys[] = {1,3,12,2};
  char data[] = {'A','B'};
  Map map = mapCreate(copyKeyInt,copyDataChar,
                       freeChar,freeInt,compareInts);
  mapPut(map,&keys[0],data);
  //printKeysByOrder(map);
  mapPut(map,&keys[1],&data[1]);
  //printKeysByOrder(map);
  mapPut(map,&keys[3],&data[0]);
  //log();
  printf("[+]Whatt.\n");
  printKeysByOrder(map);
  //printf("[+] Key:%d\nData:%c\n",keys[0], *(char*)(mapGet(map,keys)));
  return 0;
}
