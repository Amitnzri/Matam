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
  int res_state = eurovisionAddState(eurovision,0,"israel","home");
  int res_judge = eurovisionAddJudge(eurovision,0,"tomer",arr);
  printf("%d,%d\n",res_state,res_judge);


  return 0;
}
