
#ifndef DEFINE_H
#define  DEFINE_H

typedef struct contestValues_t{
    int audience_percent;
    int num_of_judges;
    int num_of_states;
}*ContestValues;

#define TOP_TEN_LEN 10
#define SPACE 32
#define NONE -1
#define SINGLE_VOTE 1

typedef enum {
    JUDGE,
    STATE
}Voter;

typedef enum {
  ADD = 1,
  SUBTRACT = -1
} Operation;

#endif
