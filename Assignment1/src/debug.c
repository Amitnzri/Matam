#include "map.h"
#include <assert.h>
#include <string.h>

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

static void insertValues(Map map,int* keys,char* data){ //Temporary
/***************************************************************
Loops through keys and data arrays and insert them into the map.
TODO: make it Generic.
***************************************************************/
  int len =strlen(data);
  for (int i=0;i<len;i++){
    printf("index: %d, key: %d\n",i,keys[i]);
    mapPut(map,&keys[i],&data[i]);
  }
}

int main(){
  int keys[] = {3,1,1,2};
  char data[] = {'A','B','C','D'};
  Map map = mapCreate(copyKeyInt,copyDataChar,
                       freeChar,freeInt,compareInts);
  insertValues(map,keys,data);
  printKeysByOrder(map);

  return 0;
}
