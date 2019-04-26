#include "map.h"
#include "eurovision.h"
#include <assert.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
//#include "eurovision.c"
#define NONE -1
#define TOP_TEN_LEN 10



int main(){
Eurovision eurovision = eurovisionCreate();
eurovisionAddState(eurovision,0,"israel","home");
eurovisionAddState(eurovision,1,"usa","something");
int res = eurovisionAddVote(eurovision,0,1);
printf("%d\n",res );
eurovisionDestroy(eurovision);

  return 0;
}
