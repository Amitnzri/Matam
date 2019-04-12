#include "map.h"
#include <assert.h>
int main(){
  char* tmp_key = "uno";
  int tmp_data = 1;
  Map test = mapCreate(copyKeyInt,copyDataChar,
                       freeChar,freeInt,compareInts);

  printf("[+]Map size:%d\n",mapGetSize(map));
  mapPut(test,&tmp_data,tmp_key);
  printf("[+]Map size:%d\n",mapGetSize(map));
  return 0;
}
