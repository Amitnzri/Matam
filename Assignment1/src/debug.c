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
  int tmp_key = 1;
  int tmp_key_2 =2;
  int tmp_key_3 =5;
  char* tmp_data = "A";
  char* tmp_data_2 = "B";
  Map test = mapCreate(copyKeyInt,copyDataChar,
                       freeChar,freeInt,compareInts);
  log();
  printf("Map size:%d\n",mapGetSize(test));
  printf("[+]Creating Dictionary..\n");
  MapResult result = mapPut(test,&tmp_key,tmp_data);
  if(result == MAP_SUCCESS) printf("[+]1: Dictionary created successfully\n");
  log();
  printf("Map size:%d\n",mapGetSize(test));
  MapResult result_2 = mapPut(test,&tmp_key_2,tmp_data_2);
  if(result_2 == MAP_SUCCESS) printf("[+]2: Dictionary created successfully\n");
  log();
  printf("Map size:%d\n",mapGetSize(test));

  log();
  if(mapContains(test,&tmp_key))printf("The key 1 is in\n");
  if(mapContains(test,&tmp_key_2))printf("The key 2 is in\n");
  if(mapContains(test,&tmp_key_3))printf("The key 5 is in\n");
  char* rec = mapGet(test,&tmp_key_3);
  printf("%s\n",rec);
  return 0;
}
