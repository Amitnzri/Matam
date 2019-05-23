
PATH = 'file'

#first type: inside contest
#second type: techniovision
#third type: staff choice

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



if __name__ =='__main__':
    print(inside_contest('Electrical_Engineering',PATH))
