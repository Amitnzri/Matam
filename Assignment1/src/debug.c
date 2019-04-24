#include "map.h"
#include "eurovision.h"
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>





int main(){
    Eurovision eurovision = eurovisionCreate();
  int res_state = eurovisionAddState(eurovision,0,"israel","home");
  eurovisionRemoveState(eurovision,0);
  free(eurovision);
  return 0;
}
