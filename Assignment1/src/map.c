#include "map.h"
#include <assert.h>
#include <stdarg.h> //TODO: Check if allowed.
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct dictionary_t {

  MapKeyElement key;
  MapDataElement data;

  struct dictionary_t* previous_block;
  struct dictionary_t* next_block;

}* dictionary;

struct Map_t{

  dictionary dictionary;

  copyMapDataElements copyDataFunction;
  copyMapKeyElements  copyKeyFunction;
  freeMapDataElements freeDataFunction;
  freeMapKeyElements  freeKeyFunction;
  compareMapKeyElements compareKeysFunction;
};

typedef enum LocationType {
    ASSIGN_BEFORE,ASSIGN_AFTER,ASSIGN_HERE
} LocationType;

/*##############################InnerFunctions###############################*/

//Returns false if one of the arguments is NULL.
static bool checkIfNull(int n,...){
  va_list arg;
  va_start(arg,n);
  for(int i=0;i<n;i++){
    if(va_arg(arg,int) == 0)return false;
  }
  va_end(arg);
  return true;
} //TODO: Check if allowed.
//Assigns a given key and data to a block.
static MapResult assignValues(Map map,dictionary block,
                              MapKeyElement key, MapDataElement data){
  assert(map&&key&&data);
  block->key = map->copyKeyFunction(key);
  block->data = map->copyKeyFunction(data);
  if(!map->dictionary->key ||!map->dictionary->data) return MAP_OUT_OF_MEMORY;
  return MAP_SUCCESS;
}
//Steps to the next key in a given dictionary.
static void stepForward(Map map){

  assert(map&&map->dictionary->next_block);
  if(map->dictionary->next_block){
    map->dictionary = map->dictionary->next_block;
  }
}
//Steps back to the previous key in a given dictionary.
static void stepBackward(Map map){
  assert(map&&map->dictionary->previous_block);
  if(map->dictionary->previous_block){
    map->dictionary = map->dictionary->previous_block;
  }
}
//Steps back to the first item.
static void goToFirstItem(Map map){
  assert(map);
  while(map->dictionary->previous_block){
    stepBackward(map);
  }
}
//Changes the dictionary ptr inside map to points on requested key.
static dictionary jumpTo (Map map, MapKeyElement key){

  compareMapKeyElements compareKey = map->compareKeysFunction;
  //check if the key is in the dictionary.
  if(!mapContains(map,key))return NULL;

  //locate the key in the dictionary.
  while(compareKey(key,map->dictionary->key)>0){
    assert(map->dictionary->next_block);
    stepForward(map);
  }
  while(compareKey(key,map->dictionary->key)<0){
    assert(map->dictionary->previous_block);
    stepBackward(map);
  }
  //The key was found,return his block.
  return map->dictionary;
}
//Creates new dictionary block.
static dictionary createDictionaryBlock(Map map,dictionary previous_block
                                        ,dictionary next_block){
  assert(map);
  dictionary new_block = malloc(sizeof(*(new_block)));
  assert(new_block);
  if(!new_block) return NULL;
  new_block->previous_block = previous_block; //Can be NULL.
  new_block->next_block = next_block; //Can be NULL.
  return new_block;
}
//Places new block between two blocks.
static void placeBetweenKeys(dictionary block){
  if(block->next_block) block->next_block->previous_block = block;
  if(block->previous_block) block->previous_block->next_block = block;
}
//Finds and return the sorted location for placing a key.
static LocationType findSortedPosition(Map map,MapKeyElement key){

  assert(key&&map->dictionary);
  compareMapKeyElements compareKeys = map->compareKeysFunction;
  while(compareKeys(key,map->dictionary->key)>0){
    if(!map->dictionary->next_block){ //if last item.
      return ASSIGN_AFTER;
  }else if(compareKeys(key,map->dictionary->next_block->key)<0){
      return ASSIGN_AFTER;
    }
    stepForward(map);
  }
  while(compareKeys(key,map->dictionary->key)<0){
    if(!map->dictionary->previous_block){ //if first item.
      return ASSIGN_BEFORE;
    }else if(compareKeys(key,map->dictionary->previous_block->key)>0){
      return ASSIGN_BEFORE;
    }
    stepBackward(map);
  }
  return ASSIGN_HERE; //if the keys are equal.
}
//Connects two blocks to each other.
static void connectBlocks(dictionary first, dictionary second){
  assert(first || second);
  if(first) first->next_block = second;
  if(second) second->previous_block = first;
}
//Deletes the current block that a given map is points on.
static void DeleteCurrentBlock(Map map){
/*******************X
TODO: 1.Look for bugs.
      2.Change return type.
*******************/
  if(!map||!map->dictionary) return;
  dictionary requested_block = map->dictionary
  ,previous_block = requested_block->previous_block
  ,next_block = requested_block->next_block; //TODO: check with convention.

  map->freeKeyFunction(requested_block->key);
  map->freeDataFunction(requested_block->data);

  if(previous_block || next_block){
    connectBlocks(previous_block,next_block);
  }
  free(requested_block);
  if(!previous_block&&!next_block) {
    map->dictionary = NULL;
  }
}
//Creates a duplicate of a map.
static MapResult duplicateMap(Map destination,Map source){

  assert(destination&&source);
  destination->copyKeyFunction = source->copyKeyFunction;
  destination->copyDataFunction = source->copyDataFunction;
  destination->freeKeyFunction = source->freeKeyFunction;
  destination->freeDataFunction = source->freeDataFunction;
  destination->compareKeysFunction = source->compareKeysFunction;

  if(source->dictionary == NULL)return MAP_NULL_ARGUMENT;
  goToFirstItem(source);
  while(source->dictionary){
    MapKeyElement key = source->dictionary->key;
    MapDataElement data = source->dictionary->data;
    MapResult copy_result = mapPut(destination,key,data);
    if(copy_result != MAP_SUCCESS) return copy_result;
    if(source->dictionary->next_block ==NULL) break;
    stepForward(source);
  }
  return MAP_SUCCESS;
}


/*###############################Functions###################################*/

Map mapCreate(copyMapDataElements copyDataElement,
              copyMapKeyElements copyKeyElement,
              freeMapDataElements freeDataElement,
              freeMapKeyElements freeKeyElement,
              compareMapKeyElements compareKeyElements){

  assert(copyDataElement&&copyKeyElement&&freeKeyElement&&
         freeDataElement&&compareKeyElements);

  //Checks Params
  if(!copyDataElement||!copyKeyElement||!freeKeyElement)return NULL;
  if(!freeDataElement||!compareKeyElements)return NULL;

  Map map = malloc(sizeof(*map));
  assert(map);
  if(!map)return NULL;

  map->dictionary = NULL;
  //Assigns function pointers
  map->copyDataFunction = copyDataElement;
  map->copyKeyFunction = copyKeyElement;
  map->freeKeyFunction = freeKeyElement;
  map->freeDataFunction = freeDataElement;
  map->compareKeysFunction = compareKeyElements;

  return map;} //CHECKED.

MapResult mapPut(Map map,MapKeyElement keyElement,MapKeyElement dataElement){

  assert(map);
  if(map == NULL) return MAP_NULL_ARGUMENT;
  if(map->dictionary == NULL){ //If the map has no dictionary yet.
    map->dictionary = createDictionaryBlock(map,NULL,NULL);
    assert(map->dictionary);
    if(map->dictionary == NULL) return MAP_OUT_OF_MEMORY;
    return assignValues(map,map->dictionary,keyElement,dataElement);
  }else{ //If has items in it already.
    dictionary previous_block=NULL;
    dictionary next_block=NULL;
    switch (findSortedPosition(map,keyElement)){

      case ASSIGN_AFTER:
        previous_block = map->dictionary;
        if(map->dictionary->next_block == NULL)break;
        next_block = map->dictionary->next_block;
        stepForward(map);
        break;

      case ASSIGN_BEFORE:
        next_block = map->dictionary;
        if(map->dictionary->previous_block == NULL)break;
        previous_block = map->dictionary->previous_block;
        stepBackward(map);
        break;

      case ASSIGN_HERE:
        placeBetweenKeys(map->dictionary);
        assignValues(map,map->dictionary,keyElement,dataElement);
        return MAP_SUCCESS;
    }
    dictionary new_block = createDictionaryBlock(map,previous_block,next_block);
    if(new_block == NULL) return MAP_OUT_OF_MEMORY;
    placeBetweenKeys(new_block);
    return assignValues(map,new_block,keyElement,dataElement);
  }

} //CHECKED.

MapKeyElement mapGetFirst(Map map){
  assert(map);
  if(map == NULL || map->dictionary == NULL) return NULL;
  goToFirstItem(map);
  return map->dictionary->key;} //CHECKED.

MapKeyElement mapGetNext(Map map){

  assert(map);
  if(map == NULL) return NULL;
  if(map->dictionary->next_block == NULL) return NULL;
  stepForward(map);
  return map->dictionary->key;

} //CHECKED.

int mapGetSize(Map map){

  if(map == NULL) return -1;
  int counter = 0;
  if(map->dictionary){
    counter++;
    goToFirstItem(map);
    while(map->dictionary->next_block){
      stepForward(map);
      counter++;
    }
  }
  return counter;} //CHECKED.

bool mapContains(Map map, MapKeyElement element){

  if(map == NULL || element == NULL) false;
  compareMapKeyElements compareKeys = map->compareKeysFunction;
  findSortedPosition(map,element);
  if(compareKeys(element,map->dictionary->key)==0){
    return true;
  }else{
    return false;
  }
} //CHECKED.

MapDataElement mapGet(Map map, MapKeyElement keyElement){

  if(map == NULL || keyElement == NULL) return NULL;
  dictionary requested_block = jumpTo(map,keyElement);
  if(requested_block == NULL)return NULL;
  return requested_block->data;} //CHECKED.

MapResult mapRemove(Map map, MapKeyElement keyElement){

  assert(map&&map->dictionary);
  if(!map||!map->dictionary) return MAP_NULL_ARGUMENT;
  if(jumpTo(map,keyElement) == NULL) return MAP_ITEM_DOES_NOT_EXIST;
  DeleteCurrentBlock(map);
  return MAP_SUCCESS;
}

MapResult mapClear(Map map){
/*******************
TODO: Look for bugs.
*******************/
  assert(map);
  if(!map) return MAP_NULL_ARGUMENT;

  goToFirstItem(map);
  while(map->dictionary->next_block){
    DeleteCurrentBlock(map);
    stepForward(map);
  }
  DeleteCurrentBlock(map);
  return MAP_SUCCESS;
}

void mapDestroy(Map map){

  assert(map);
  if(!map) return;
  if(map->dictionary)mapClear(map);
  free(map);


}

Map mapCopy(Map map){

  assert(map);
  if(map == NULL)return NULL;
  Map new_map = malloc(sizeof(*new_map));
  if(new_map == NULL)return NULL;
  if(duplicateMap(new_map,map) != MAP_SUCCESS)return NULL;
  return new_map;
}
