
from Techniovision import *

PATH = 'input.txt'

#scans the file and returns participants faculties.
def get_faculties(file_name):
    faculties =[]
    with open(file_name,'r') as file:
        for line in file:
            command_type,*_,faculty = line.split()
            if command_type =='staff':
                faculties.append(faculty)
    return faculties

def inside_contest(faculty,file_name):
    used_id = []
    valid_programs = []
    votes = {}

    with open(file_name,'r') as file:
        for line in file:
            command_type,_,*rest,faculty_name = line.split()

            #counts students votes.
            if command_type == 'inside' and faculty_name == faculty:
                id,program = rest[0:2]
                if id not in used_id:
                    used_id.append(id)
                    votes[program] = 1 if program not in votes.keys() \
                                                          else votes[program]+1

            #creates valid programs list and gives 20 pts to the chosen program.
            if command_type == 'staff' and faculty_name ==faculty:
                votes[rest[0]] = 20 if rest[0] not in votes.keys() \
                                                        else votes[rest[0]]+20
                valid_programs = list.copy(rest)

    #Removes votes of programs that dont exist.
    keys = list(votes.keys())
    for program in keys:
        if program not in valid_programs:
            votes.pop(program)

    #sorts the dictionary by value.
    sorted_programs = sorted(votes,key = votes.get)

    return sorted_programs[-1] if len(sorted_programs)>0 else None

def run_contest(elected_programs,file_name):
    technion_object= TechniovisionCreate()
    with open(file_name,'r') as file:
        for line in file:
            command,student_id,voted_program,student_faculty,*_= line.split()

            if command =='techniovision' and \
                                       voted_program in elected_programs.keys():

                TechniovisionStudentVotes(technion_object, int(student_id),
                        student_faculty, elected_programs.get(voted_program))

    TechniovisionWinningFaculty(technion_object)
    TechniovisionDestroy(technion_object)


if __name__ =='__main__':
    faculties = get_faculties(PATH)
    elected_programs ={} # contains the elected program of each faculty.
    for faculty in faculties:
        elected_programs[inside_contest(faculty,PATH)] = faculty
    run_contest(elected_programs,PATH)
