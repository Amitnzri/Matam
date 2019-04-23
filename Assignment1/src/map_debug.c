#include "map.h"
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

static MapKeyElement copyKeyInt(MapKeyElement n) {
    if (!n) {
        return NULL;
    }
    int *copy = malloc(sizeof(*copy));
    if (!copy) {
        return NULL;
    }
    *copy = *(int *) n;
    return (MapKeyElement) copy;
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

static void printValuesByOrder(Map map){
  if(map == NULL){printf("%s\n","[+] No map" );return;}
  if(mapGetSize(map)==0){printf("%s\n","[+] No dictionary"); return;}
  int counter =1;
  MapKeyElement key = mapGetFirst(map);
  do{
    printf("[+]Key %d is: %d | Data: %c\n",
    counter,*(int*)key,*(char*)mapGet(map,key));
    counter++;
}while(key = mapGetNext(map));
}

static void insertValues(Map map,int* keys,char* data){ //Temporary
/***************************************************************
Loops through keys and data arrays and insert them into the map.
TODO: make it Generic.
***************************************************************/
  int len =strlen(data);
  printf("%d\n",len );
  for (int i=0;i<len;i++){
    mapPut(map,&keys[i],&data[i]);
  }
}

#define ASSERT_TEST(b) do { \
  printf("\nAssertion failed at %s:%d %s ",__FILE__,__LINE__,#b); \
  return false; \
} while (0)

static bool checkIfNull(int n,...){
  va_list arg;
  va_start(arg,n);
  for(int i=0;i<n;i++){
    if(va_arg(arg,int) == 0)return false;
  }
  va_end(arg);
  return true;
}

int main(){


  int keys[] = {3,2,1,7,5,6,4};
  char data[] = {'A','B','C','E','S','A','l'};
  Map map = mapCreate(copyDataChar,copyKeyInt,
                       freeChar,freeInt,compareInts);
  insertValues(map,keys,data);
  printValuesByOrder(map);
  mapDestroy(map);

  return 0;
}
