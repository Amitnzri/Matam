#include "map.h"
#include "eurovision.h"
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>





int main(){
    Eurovision eurovision = eurovisionCreate();
    //eurovisionAddState(eurovision,0,"israel","home");
    int arr[10] = {1,2,3,4,5,6,7,8,9,10};
    int res = eurovisionAddJudge(eurovision,0,"amit",arr);
    eurovisionRemoveState(eurovision,1);
    eurovisionRemoveJudge(eurovision,0);
    eurovisionDestroy(eurovision);

  return 0;
}
