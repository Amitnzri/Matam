#include "map.h"
#include <assert.h>
int main(){
  char* tmp_key = "uno";
  int tmp_data = 1;
  Map test = mapCreate(copyKeyInt,copyDataChar,
                       freeChar,freeInt,compareInts);
                       

  return 0;
}
