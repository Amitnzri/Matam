#include "map.h"
#include <assert.h>

//Typedefs..
typedef enum LocationType {
    ASSIGN_BEFORE,ASSIGN_AFTER,ASSIGN_HERE
} LocationType;

typedef void *element; //change name.

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
static MapResult assignValues(Map map,dictionary block,
                              element key, element data){
  assert(map&&key&&data);
  block->key = map->copyKeyFunction(key);
  block->data = map->copyKeyFunction(data);
  if(!map->dictionary->key ||!map->dictionary->data) return MAP_OUT_OF_MEMORY;
  return MAP_SUCCESS;
}

//Steps to the next key in a given dictionary
static void stepForward(Map map){
  assert(map&&map->dictionary->next_block);
  if(map->dictionary->next_block){
    map->dictionary = map->dictionary->next_block;
  }
}

//Steps back to the previous key in a given dictionary
static void stepBackward(Map map){
  assert(map&&map->dictionary->previous_block);
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

static dictionary jumpTo (Map map, MapKeyElement key){
  /***********************
  TODO: make sure it works
  ***********************/
  compareMapKeyElements compareKey = map->CompareKeysFunction;
  //checks if the key is in the dictionary.
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
  return map->dictionary;
}

static dictionary createDictionaryBlock(Map map,dictionary previous_block
                                        ,dictionary next_block){
  assert(map);
  dictionary new_block = malloc(sizeof(*(new_block)));
  assert(new_block);
  if(!new_block) return NULL;
  new_block->previous_block = previous_block;
  new_block->next_block = next_block;
  return new_block;
}

static void placeBetweenKeys(dictionary block){
  block->next_block->previous_block = block;
  block->previous_block->next_block = block;
}

//finds and return the sorted location for placing a key.
static LocationType findSortedPosition(Map map,MapKeyElement key){
/**************************************
TODO: 1.move the create function outside.
**************************************/
  assert(key&&map->dictionary);
  compareMapKeyElements compareKeys = map->CompareKeysFunction;
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

MapResult mapPut(Map map,MapKeyElement keyElement,MapKeyElement dataElement){
/*****************************
TODO: 1.Fix the return values.
      2.change findSorted type.
      3.Look for bugs.
*****************************/
  //Checks if the map has dictionary already.
  assert(map);
  if(!map) return MAP_NULL_ARGUMENT;
  if(!map->dictionary){
    map->dictionary = createDictionaryBlock(map,NULL,NULL);
    assert(map->dictionary);
    if(!map->dictionary) return MAP_OUT_OF_MEMORY;
    assignValues(map,map->dictionary,keyElement,dataElement);
    return MAP_SUCCESS;
  }else{ //if has items in it already.
    dictionary previous_block=NULL;
    dictionary next_block=NULL;
    switch (findSortedPosition(map,keyElement)){

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

      case ASSIGN_HERE:
        placeBetweenKeys(map->dictionary);
        assignValues(map,map->dictionary,keyElement,dataElement);
        return MAP_SUCCESS; //TODO:check returns

      default:
        return MAP_NULL_ARGUMENT;

    dictionary new_block = createDictionaryBlock(map,previous_block,next_block);
    if(!new_block) return MAP_OUT_OF_MEMORY;
    placeBetweenKeys(new_block);
    assignValues(map,new_block,keyElement,dataElement);
    return MAP_SUCCESS;
    }
  }
  return MAP_NULL_ARGUMENT; //Shouldn't get here.
}

MapKeyElement mapGetFirst(Map map){
  assert(map);
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
      stepForward(map);
      counter++;
    }
  }
  return counter;
}

bool mapContains(Map map, MapKeyElement element){
  /************************************************************
  TODO: 1.Look for bugs.
        2.make sure that findSorted does not create new blocks.
  ************************************************************/
  findSortedPosition(map,element);
  if(map->dictionary->key == element){
    return true;
  }else{
    return false;
  }
}


MapDataElement mapGet(Map map, MapKeyElement keyElement){

  dictionary requested_block = jumpTo(map,keyElement);
  if(!requested_block){
    return NULL;
  }else{
  return requested_block->data;
  }
}
