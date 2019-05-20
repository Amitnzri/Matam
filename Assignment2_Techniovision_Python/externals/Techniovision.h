//
// Created by ameeraam on 3/22/2019.
//

#ifndef HW2_TECHNIOVISION_H
#define HW2_TECHNIOVISION_H

typedef struct techniovision_t* Techniovision;

Techniovision TechniovisionCreate();

void TechniovisionStudentVotes(Techniovision t, int student, const char* studentsFaculty, const char* votingFaculty);

void TechniovisionWinningFaculty(Techniovision t);

void TechniovisionDestroy(Techniovision t);

#endif //HW2_TECHNIOVISION_H
