#include <assert.h>
<<<<<<< HEAD
=======
#include <stdarg.h> //TODO: Check if allowed.
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9
#include <stdio.h>
#include <stdlib.h>
#include "../include/map.h"

/*****************************Defines&Typedefs*******************************/

typedef struct dictionary_t {

  MapKeyElement key;
  MapDataElement data;

  struct dictionary_t* previous_block;
  struct dictionary_t* next_block;

}* Dictionary;

struct Map_t{

  Dictionary dictionary;

  copyMapDataElements copyDataFunction;
  copyMapKeyElements  copyKeyFunction;
  freeMapDataElements freeDataFunction;
  freeMapKeyElements  freeKeyFunction;
  compareMapKeyElements compareKeysFunction;
};

typedef enum LocationType {
    ASSIGN_BEFORE,ASSIGN_AFTER,ASSIGN_HERE
} LocationType;

<<<<<<< HEAD
/****************************InnerFunctions**********************************/
//Assigns values into the map.
static MapResult assignValues(Map map,Dictionary block,
                              MapKeyElement key, MapDataElement data){
    assert(map&&key&&data);
    copyMapKeyElements copyKey = map->copyKeyFunction;
    copyMapDataElements copyData = map->copyDataFunction;
    //If the block has values already, free them.
    if(block->key) map->freeKeyFunction(block->key);
    if(block->data) map->freeDataFunction(block->data);
    //Assigns the new values.
    block->key = copyKey(key);
    block->data = copyData(data);
    if(!map->dictionary->key ||!map->dictionary->data) return MAP_OUT_OF_MEMORY;
    return MAP_SUCCESS;
}
//Steps to the next key in a given dictionary.
static void stepForward(Map map){
    assert(map);
    if(map->dictionary->next_block){
        map->dictionary = map->dictionary->next_block;
    }
}
//Steps back to the previous key in a given dictionary.
static void stepBackward(Map map){
    assert(map);
    if(map->dictionary->previous_block){
        map->dictionary = map->dictionary->previous_block;
    }
=======
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
static MapResult assignValues(Map map,Dictionary block,
                              MapKeyElement key, MapDataElement data){
  assert(map&&key&&data);
  copyMapKeyElements copyKey = map->copyKeyFunction;
  copyMapDataElements copyData = map->copyDataFunction;

  if(block->key) map->freeKeyFunction(block->key);
  if(block->data) map->freeDataFunction(block->data);
  block->key = copyKey(key);
  block->data = copyData(data);
  if(!map->dictionary->key ||!map->dictionary->data) return MAP_OUT_OF_MEMORY;
  return MAP_SUCCESS;
}
//Steps to the next key in a given dictionary.
static void stepForward(Map map){

  assert(map);
  if(map->dictionary->next_block){
    map->dictionary = map->dictionary->next_block;
  }
}
//Steps back to the previous key in a given dictionary.
static void stepBackward(Map map){
  assert(map);
  if(map->dictionary->previous_block){
    map->dictionary = map->dictionary->previous_block;
  }
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9
}
//Steps back to the first item.
static void goToFirstItem(Map map){
    assert(map);
    while(map->dictionary->previous_block){
        stepBackward(map);
    }
}
//Changes the dictionary ptr inside map to points on requested key.
static Dictionary jumpTo (Map map, MapKeyElement key){
<<<<<<< HEAD
    //Checks if the key is in the dictionary.
    if(!mapContains(map,key))return NULL;
    //If contains,the iterator moved to the wanted location.
    return map->dictionary;
=======

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
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9
}
//Creates new dictionary block.
static Dictionary createDictionaryBlock(Map map,Dictionary previous_block
                                        ,Dictionary next_block){
<<<<<<< HEAD
    assert(map);
    Dictionary new_block = malloc(sizeof(*(new_block)));
    assert(new_block);
    if(!new_block) return NULL;
    new_block->previous_block = previous_block; //Can be NULL.
    new_block->next_block = next_block; //Can be NULL.
    new_block->key  = NULL;
    new_block->data = NULL;
    return new_block;
}
//Places new block between two blocks.
static void placeBetweenKeys(Dictionary block){
    if(block->next_block) block->next_block->previous_block = block;
    if(block->previous_block) block->previous_block->next_block = block;
=======
  assert(map);
  Dictionary new_block = malloc(sizeof(*(new_block)));
  assert(new_block);
  if(!new_block) return NULL;
  new_block->previous_block = previous_block; //Can be NULL.
  new_block->next_block = next_block; //Can be NULL.
  new_block->key  = NULL;
  new_block->data = NULL;
  return new_block;
}
//Places new block between two blocks.
static void placeBetweenKeys(Dictionary block){
  if(block->next_block) block->next_block->previous_block = block;
  if(block->previous_block) block->previous_block->next_block = block;
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9
}
//Finds and return the sorted location for placing a key.
static LocationType findSortedPosition(Map map,MapKeyElement key){
    assert(key&&map->dictionary);
    compareMapKeyElements compareKeys = map->compareKeysFunction;
    assert(map->dictionary);
    while(compareKeys(key,map->dictionary->key)>0){
        if(!map->dictionary->next_block){//if last item.
            return ASSIGN_AFTER;

<<<<<<< HEAD
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
=======
  assert(key&&map->dictionary);
  compareMapKeyElements compareKeys = map->compareKeysFunction;
  assert(map->dictionary);
  while(compareKeys(key,map->dictionary->key)>0){

    if(map->dictionary->next_block == NULL){//if last item.
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
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9
    }
    return ASSIGN_HERE; //if the keys are equal.
}
//Connects two blocks to each other.
static void connectBlocks(Dictionary first, Dictionary second){
<<<<<<< HEAD
    assert(first || second);
    if(first) first->next_block = second;
    if(second) second->previous_block = first;
}
//Deletes the current block that a given map is points on.
static void DeleteCurrentBlock(Map map){
=======
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
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9
    assert(map&&map->dictionary);
    if(!map||!map->dictionary) return;
    Dictionary requested_block = map->dictionary
    ,previous_block = map->dictionary->previous_block
<<<<<<< HEAD
    ,next_block = map->dictionary->next_block;
=======
    ,next_block = map->dictionary->next_block; //TODO: check with convention.
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9

    map->freeKeyFunction(requested_block->key);
    map->freeDataFunction(requested_block->data);

    if(previous_block || next_block){
<<<<<<< HEAD
        connectBlocks(previous_block,next_block);
=======
    connectBlocks(previous_block,next_block);
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9
    }
    if(next_block){
        stepForward(map);
    }else{
        stepBackward(map);
    }
    free(requested_block);
    if(!previous_block&&!next_block) {
<<<<<<< HEAD
        map->dictionary = NULL;
=======
    map->dictionary = NULL;
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9
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

    if(!source->dictionary)return MAP_NULL_ARGUMENT;
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


/********************************Functions***********************************/

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

<<<<<<< HEAD
    map->dictionary = NULL;

    //Assigns function pointers
    map->copyDataFunction = copyDataElement;
    map->copyKeyFunction = copyKeyElement;
    map->freeKeyFunction = freeKeyElement;
    map->freeDataFunction = freeDataElement;
    map->compareKeysFunction = compareKeyElements;
=======
  return map;} //CHECKED.

MapResult mapPut(Map map,MapKeyElement keyElement,MapKeyElement dataElement){
  assert(map);
  if(map == NULL) return MAP_NULL_ARGUMENT;
  if(map->dictionary == NULL){ //If the map has no dictionary yet.
    map->dictionary = createDictionaryBlock(map,NULL,NULL);

    if(map->dictionary == NULL) return MAP_OUT_OF_MEMORY;
    return assignValues(map,map->dictionary,keyElement,dataElement);
  }else{ //If has items in it already.
    Dictionary previous_block=NULL;
    Dictionary next_block=NULL;
    switch (findSortedPosition(map,keyElement)){

      case ASSIGN_AFTER:
        previous_block = map->dictionary;
        if(map->dictionary->next_block == NULL)break;
        next_block = map->dictionary->next_block;
        stepForward(map);
        break;
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9

    return map;
}

<<<<<<< HEAD
MapResult mapPut(Map map,MapKeyElement keyElement,MapKeyElement dataElement){
    assert(map&&keyElement&&dataElement);
    if(!map || !keyElement) return MAP_NULL_ARGUMENT;
    if(!map->dictionary){ //If the map has no dictionary yet.
        map->dictionary = createDictionaryBlock(map,NULL,NULL);
        if(!map->dictionary) return MAP_OUT_OF_MEMORY;
        return assignValues(map,map->dictionary,keyElement,dataElement);
    }else{ //If has items in it already.
        Dictionary previous_block=NULL;
        Dictionary next_block=NULL;
        switch (findSortedPosition(map,keyElement)){

            case ASSIGN_HERE:
                placeBetweenKeys(map->dictionary);
                assignValues(map,map->dictionary,keyElement,dataElement);
                return MAP_SUCCESS;

            case ASSIGN_AFTER:
                previous_block = map->dictionary;
                if(!map->dictionary->next_block)break;
                next_block = map->dictionary->next_block;
                stepForward(map);
                break;

            case ASSIGN_BEFORE:
                next_block = map->dictionary;
                if(!map->dictionary->previous_block)break;
                previous_block = map->dictionary->previous_block;
                stepBackward(map);
                break;
        }
    Dictionary new_block = createDictionaryBlock(map,previous_block,next_block);
    if(!new_block) return MAP_OUT_OF_MEMORY;
=======
      case ASSIGN_HERE:
        placeBetweenKeys(map->dictionary);
        assignValues(map,map->dictionary,keyElement,dataElement);
        return MAP_SUCCESS;
    }
    Dictionary new_block = createDictionaryBlock(map,previous_block,next_block);
    if(new_block == NULL) return MAP_OUT_OF_MEMORY;
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9
    placeBetweenKeys(new_block);
<<<<<<< HEAD
    return assignValues(map,new_block,keyElement,dataElement);
  }

} //CHECKED.
=======
    assignValues(map,new_block,keyElement,dataElement);
    return MAP_SUCCESS;
<<<<<<< HEAD
    }
}
>>>>>>> develop
=======
  }} //CHECKED.
>>>>>>> f21ef7da81a85dcff1a60282781b2bb30edee716
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9

MapKeyElement mapGetFirst(Map map){
    assert(map);
    if(!map || !map->dictionary) return NULL;
    goToFirstItem(map);
    return map->dictionary->key;
}

MapKeyElement mapGetNext(Map map){
    assert(map);
    if(!map|| !map->dictionary || !map->dictionary->next_block) return NULL;
    stepForward(map);
    return map->dictionary->key;
}


int mapGetSize(Map map){
    if(!map) return -1;
    int counter = 0;
    if(map->dictionary){
        counter++;
        goToFirstItem(map);
        while(map->dictionary->next_block){
            stepForward(map);
            counter++;
        }
    }
    return counter;
}

bool mapContains(Map map, MapKeyElement element){
<<<<<<< HEAD

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
  Dictionary requested_block = jumpTo(map,keyElement);
  if(requested_block == NULL)return NULL;
  return requested_block->data;} //CHECKED.
=======
    assert(map&&element);
    if(!map||!map->dictionary||!element)return false;
    compareMapKeyElements compareKeys = map->compareKeysFunction;
    findSortedPosition(map,element); //moves the iterator to the closest key.
    if(compareKeys(element,map->dictionary->key)==0){
        return true;
    }else{
        return false;
    }
}

<<<<<<< HEAD
MapDataElement mapGet(Map map, MapKeyElement keyElement){
    if(!map || !keyElement) return NULL;
    Dictionary requested_block = jumpTo(map,keyElement);
    if(!requested_block)return NULL;
    return requested_block->data;
}
>>>>>>> develop
=======

MapResult mapRemove(Map map, MapKeyElement keyElement){
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9

MapResult mapRemove(Map map, MapKeyElement keyElement){
    assert(map&&map->dictionary);
    if(!map||!map->dictionary||!keyElement) return MAP_NULL_ARGUMENT;
    if(!jumpTo(map,keyElement)) return MAP_ITEM_DOES_NOT_EXIST;
    DeleteCurrentBlock(map);
    return MAP_SUCCESS;
}

MapResult mapClear(Map map){
    assert(map);
    if(!map) return MAP_NULL_ARGUMENT;

<<<<<<< HEAD
    goToFirstItem(map);

    while(map->dictionary->next_block){
        DeleteCurrentBlock(map);
    }
    DeleteCurrentBlock(map);
    return MAP_SUCCESS;
=======
  goToFirstItem(map);

  while(map->dictionary->next_block){
    DeleteCurrentBlock(map);
  }
  DeleteCurrentBlock(map);
  return MAP_SUCCESS;
>>>>>>> ced78c5d1c27117fa9f0d1d45f881e6ef10ec3d9
}

void mapDestroy(Map map){
    if(!map) return;
    if(map->dictionary)mapClear(map);
    free(map);
}

Map mapCopy(Map map){
<<<<<<< HEAD

  assert(map);
  if(map == NULL)return NULL;
  Map new_map = malloc(sizeof(*new_map));
  if(new_map == NULL)return NULL;
  if(duplicateMap(new_map,map) != MAP_SUCCESS)return NULL;
  return new_map;
=======
    if(!map)return NULL;
    Map new_map = mapCreate(map->copyDataFunction,map->copyKeyFunction,
    map->freeDataFunction,map->freeKeyFunction,map->compareKeysFunction);
    if(!new_map)return NULL;
    if(duplicateMap(new_map,map) != MAP_SUCCESS)return NULL;
    return new_map;
>>>>>>> develop
}
