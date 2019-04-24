#include "map.h"
#include "eurovision.h"
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>





int main(){
  Eurovision eurovision = eurovisionCreate();
  int res = eurovisionAddState(eurovision,0,"israel","home");
  printf("%d\n",res);


  return 0;
}
