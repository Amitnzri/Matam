#include "map.h"
#include <assert.h>

//Typedefs..
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

static dictionary jumpTo (Map map, MapKeyElement key){
  /***********************
  TODO: make sure it works
  ***********************/
  compareMapKeyElements compareKey = map->CompareKeysFunction;
  //checks if the key is in the dictionary.
  if(!mapContains(map,keyElement))return NULL;

  //locate the key in the dictionary.
  while(compareKey(keyElement,map->dictionary->key)>0){
    assert(map->dictionary->next_block);
    stepForward(map);
  }
  while(compareKey(keyElement,map->dictionary->key)<0){
    assert(map->dictionary->previous_block);
    stepBackward(map);
  }
  return map->dictionary;
}


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

static void placeBetweenKeys(dictionary wanted_key){
  wanted_key->next_block->previous_block = wanted_key;
  wanted_key->previous_block->next_block = wanted_key;
}

//finds and return the sorted location for placing a key.
static dictionary findSortedPosition(Map map,MapKeyElement key){

  assert(key&&map->dictionary);
  compareMapKeyElements compareKeys = map->CompareKeysFunction;
  while(compareKeys(key,map->dictionary->key)>0){
    if(!map->dictionary->next_block){ //if last item.
      map->dictionary->next_block = createDictionaryBlock(map,map->dictionary,
                                                          NULL);
      assert(map->dictionary->next_block);//TODO: check if need to be removed.
      stepForward(map);
      return map->dictionary;
  }else if(compareKeys(key,map->dictionary->next_block->key)<0){
      dictionary new_block = createDictionaryBlock(map,
                  map->dictionary,map->dictionary->next_block);
      assert(new_block);
      placeBetweenKeys(new_block);
      stepForward(map);
      return map->dictionary;
    }
    stepForward(map);
  }
  while(compareKeys(key,map->dictionary->key)<0){
    if(!map->dictionary->previous_block){ //if first item.
      map->dictionary->previous_block = createDictionaryBlock(map,NULL,
                                                         map->dictionary);
      assert(map->dictionary->previous_block; //TODO: same(89).
      stepBackward(map);
      return map->dictionary;
    }else if(compareKeys(key,map->dictionary->previous_block->key)>0){
      dictionary new_block = createDictionaryBlock(map,
                  map->dictionary->previous_block,map->dictionary);
      assert(new_block);
      placeBetweenKeys(new_block);
      stepBackward(map);
      return map->dictionary;
    }

    stepBackward(map);
  }
  return map->dictionary; //if the keys are equal.
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
*****************************/

  //Checks if the map has dictionary already.
  assert(map);
  if(!map) return MAP_NULL_ARGUMENT;
  if(!map->dictionary){
    map->dictionary = createDictionaryBlock(map,NULL,NULL);
    assert(map->dictionary);
    if(!map->dictionary) return MAP_OUT_OF_MEMORY;
    assignValues(map,keyElement,dataElement);
    return MAP_SUCCESS;
  }else{ //if has items in it already.
    //looking for the location.
    if(!findSortedPosition(map,keyElement))return MAP_NULL_ARGUMENT;
    assignValues(map,keyElement,dataElement);
    return MAP_SUCCESS;
  }

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
  compareMapKeyElements compareKeys = map->CompareKeysFunction;
  while(compareKeys(element,map->dictionary->key)>0){
    if(map->dictionary->next_block){
      stepForward(map);
    }else{
      return false;
    }
  }
  while(compareKeys(element,map->dictionary->key)<0){
    if(map->dictionary->previous_block){
      stepBackward(map);
    }else{
      return false;
    }
  }
  return true;
}


MapDataElement mapGet(Map map, MapKeyElement keyElement){

  dictionary requested_block = jumpTo(map,keyElement);
  if(!block){
    return NULL;
  }else{
  return requested_block->data;
  }
}
