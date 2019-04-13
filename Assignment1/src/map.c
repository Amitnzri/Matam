#include "map.h"
#include <assert.h>

//Typedefs..
typedef void *element;

//DataTypes..
typedef struct dictionary_t {

  element key;
  element data;

  struct dictionary_t* previous_block;
  struct dictionary_t* next_block;

}* dictionary;

struct Map_t{

  dictionary dictionary;

  copyMapDataElements copyDataFunction;
  copyMapKeyElements  copyKeyFunction;
  freeMapDataElements freeDataFunction;
  freeMapKeyElements  freeKeyFunction;
  compareMapKeyElements CompareKeysFunction;
};

//InnerFunctions..

//Assigns a given key and data to the current dictionary location.
static MapResult assignValues(Map map, element key, element data){
  assert(map&&key&&data);
  map->dictionary->key = map->copyKeyFunction(key);
  map->dictionary->data = map->copyKeyFunction(data);
  if(!map->dictionary->key ||!map->dictionary->data) return MAP_OUT_OF_MEMORY;
  return MAP_SUCCESS;
}

//Steps to the next key in a given dictionary
static void stepForward(Map map){
  assert(map);
  if(map->dictionary->next_block){
    map->dictionary = map->dictionary->next_block;
  }
}

//Steps back to the previous key in a given dictionary
static void stepBackward(Map map){
  assert(map);
  if(map->dictionary->previous_block){
    map->dictionary = map->dictionary->previous_block;
  }
}

static void goToFirstItem(Map map){
  assert(map);
  while(map->dictionary->previous_block){
    stepBackward(map);
  }

}

static dictionary createDictionaryBlock(Map map,dictionary previous_block
                                        ,dictionary next_block){
    /***********************
    TODO: Fix the structure.
    ***********************/
    
    dictionary current_block = map->dictionary;
    dictionary new_block = malloc(sizeof(*(new_block)));
    assert(new_block);
    if(!new_block) return NULL;
    map->dictionary = new_block;
    map->dictionary->previous_block = previous_block;
    map->dictionary->next_block = next_block;
    map->dictionary = current_block;
    return new_block;
}

//Functions..
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
  map->CompareKeysFunction = compareKeyElements;

  return map;
}

MapResult mapPut(Map map,MapKeyElement keyElement,MapDataElement dataElement){

  /******************************************************
  TODO: 1.has a bug when creating new blocks in the middle.
        2.add checks for NULL.
        3.replace code with inner function
  ******************************************************/

  assert(map&&keyElement&&dataElement);
  if(!map||!keyElement||!dataElement)return MAP_NULL_ARGUMENT;

  copyMapKeyElements copyKey = map->copyKeyFunction;
  copyMapKeyElements copyData = map->copyDataFunction;

  //Creates new dictionary if this is the first item.
  if(!map->dictionary){
    map->dictionary = createDictionaryBlock(map,NULL,NULL); //TODO: check
    assert(map->dictionary);
    if(!map->dictionary)return MAP_OUT_OF_MEMORY; //TODO: check if valid
    createDictionaryBlock(map,NULL,NULL);
    map->dictionary->key = copyKey(keyElement);
    map->dictionary->data = copyData(dataElement);
    //TODO:CHECKS
    return MAP_SUCCESS;
  }else{ //The map has items in it.

    //Checks if contains the key.
    if(!mapContains(map,keyElement)){
      //Assigns the values in sorted location.
      while(map->CompareKeysFunction(keyElement,map->dictionary->key)>0){
        if(map->dictionary->next_block){
          stepForward(map);
        }else{
          map->dictionary->next_block = createDictionaryBlock(map,map->dictionary,NULL);
          assert(map->dictionary->next_block);
          if(!map->dictionary->next_block)return MAP_OUT_OF_MEMORY;
          map->dictionary= map->dictionary->next_block;
          return assignValues(map,keyElement,dataElement);
        }
      }
      while(map->CompareKeysFunction(keyElement,map->dictionary->key)<0){
        if(map->dictionary->previous_block){
          stepBackward(map);
        }else{
          map->dictionary->previous_block = createDictionaryBlock(map,NULL,map->dictionary);
          assert(map->dictionary->previous_block);
          if(!map->dictionary->previous_block) return MAP_OUT_OF_MEMORY;
          map->dictionary = map->dictionary->previous_block;
          return assignValues(map,keyElement,dataElement);
        }
      }
    }else{
        return assignValues(map,keyElement,dataElement);
    }
  }
  #ifndef NDEBUG
  printf("[!]ERROR: got to the end of mapPut.\n");
  #endif
  return MAP_NULL_ARGUMENT; //Shouldn't get here.
}

MapKeyElement mapGetFirst(Map map){
  if(!map) return NULL;
  goToFirstItem(map);
  return map->dictionary->key;
}

MapKeyElement mapGetNext(Map map){
  if(map->dictionary->next_block){
    stepForward(map);
    return map->dictionary->key;
  }else{
    return NULL;
  }
}

int mapGetSize(Map map){
  if(!map) return -1;
  int counter = 0;
  if(map->dictionary){
    counter++;
    goToFirstItem(map);
    while(map->dictionary->next_block){
      printf("[!}Got Here\n");
      stepForward(map);
      counter++;
    }
  }
  return counter;
}

bool mapContains(Map map, MapKeyElement element){
  while(map->CompareKeysFunction(element,map->dictionary->key)>0){
    if(map->dictionary->next_block){
      map->dictionary = map->dictionary->next_block;
    }else{
      return false;
    }
  }
  while(map->CompareKeysFunction(element,map->dictionary->key)<0){
    if(map->dictionary->previous_block){
      map->dictionary = map->dictionary->previous_block;
    }else{
      return false;
    }

  }
  return true;
}
