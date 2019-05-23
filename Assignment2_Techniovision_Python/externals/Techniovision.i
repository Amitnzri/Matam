%module Techniovision
%include "typemaps.i"
%include "Techniovision.h"
%{
#include "Techniovision.h"
typedef struct techniovision_t* Techniovision;
%}

Techniovision TechniovisionCreate();

void TechniovisionStudentVotes(Techniovision t, int student, const char* studentsFaculty, const char* votingFaculty);

void TechniovisionWinningFaculty(Techniovision t);

void TechniovisionDestroy(Techniovision t);
