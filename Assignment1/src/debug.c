#include "map.h"
#include "eurovision.h"
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>





int main(){
  Eurovision eurovision = eurovisionCreate();
  int arr[10]= {0};
  eurovisionAddState(eurovision,0,"israel","home");
  int res = eurovisionAddJudge(eurovision,0,"tomer",arr);
  printf("%d\n",res);


  return 0;
}
