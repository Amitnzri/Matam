#include "../includes/eurovision.h"//TODO:Change before submission.
using std::cout;
using std::endl;

//TODO:Check if needs to remove all the votes while returning to the registration phase.

/* ------------------------------------Vote----------------------------------*/
//TODO:Check if passing a list the regular is valid.

Vote::Vote(Voter& voter ,const string v1,const string v2,
    const string v3,const string v4,const string v5,const string v6,
    const string v7,const string v8,const string v9,const string v10):
          voter(voter),votes(new string[10]{v1,v2,v3,v4,v5,v6,v7,v8,v9,v10}){}

Vote::~Vote(){
    delete[] votes;
}
/* --------------------------------------------------------------------------*/

/* -----------------------------Participant----------------------------------*/
Participant::Participant(const string state_name, const string song_name,
                      const unsigned int song_length,const string singer_name):
    state_name(state_name),song_name(song_name),song_length(song_length),
                                 singer_name(singer_name),is_registered(false){}

string Participant::state() const{
    return state_name;
}

string Participant::song() const{
  return song_name;
}


unsigned int Participant::timeLength() const{
  return song_length;
}

string Participant::singer() const{
  return singer_name;
}

bool Participant::isRegistered() const{
  return is_registered;
}

void Participant::updateRegistered(const bool status){
    is_registered = status ;
}


void Participant::update(const string song_name,const unsigned int song_length,
                                                  const  string singer_name){
if (is_registered) return ;
this->song_name = (song_name == "") ? (this->song_name) : (song_name);
this->song_length = (song_length == 0) ? (this->song_length) : (song_length);
this->singer_name = (singer_name == "") ? (this->singer_name) : (singer_name);
}

ostream& operator<<(ostream& os,const Participant& participant){
    os << '[' << participant.state() << '/' << participant.song()<<'/' <<
        participant.timeLength() << '/' << participant.singer() <<']';
    return os;
}
/*---------------------------------------------------------------------------*/

/*--------------------------------Voter--------------------------------------*/

Voter::Voter(const string voter_state,const VoterType voter_type):
              voter_state(voter_state),voter_type(voter_type),num_of_votes(0){}

string Voter::state() const{
  return voter_state;
}

ostream& operator<<(ostream& os,const Voter& voter){
  os << '<' << voter.state() << '/' << voter.voterType() << '>';
  return os;
}

VoterType Voter::voterType() const{
  return voter_type;
}

unsigned int Voter::timesOfVotes() const{
  return num_of_votes;
}

Voter& Voter::operator++(){
  num_of_votes++;
  return *this;
}
/*---------------------------------------------------------------------------*/

/*-----------------------------MainControl-----------------------------------*/

MainControl::MainControl(const unsigned int time_limit,
          const unsigned int max_participant,const unsigned int votes_limit):
          time_limit(time_limit), max_participant(max_participant),
          votes_limit(votes_limit), phase(Registration),
          votes(new VotesCount[max_participant]{nullptr},
          participants(new Participant[max_participant]{nullptr})){}
//TODO:Check if c'structor and = operator is needed.
MainControl::~MainControl(){
    delete[] votes;
    delete[]participants;
}

MainControl::VotesCount::VotesCount(){
    state_name ="";
    regular_votes=0;
    judges_votes=0;
}

MainControl& MainControl::operator+=(const Participant& participant){
    if(legalParticipant(participant)&&phase==Registration){
        addParticipant(participant);
    }
    return *this;
}

MainControl& MainControl::operator-=(const participant& participant){
    if(participate(participant)&&phase==Registration){
      removeParticipant(participant);
    }
    return *this;
}

MainControl& MainControl::operator+=(const Vote& vote){
    if(phase == Voting){
        giveVotes(vote);
    }
    return *this;
}

MainControl& operator<<(ostream& os, const MainControl& main_control){
    switch(main_control.phase){
        case Registration:
            os<< '{' <<endl << "Registration" <<endl<<
            main_control.getParticipants() << endl <<'}';
        case Voting:
            os<< '{' <<endl << "Voting" <<endl<<
            main_control.getVotes() << endl <<'}';

    return *this;
    }
}
//TODO: Check if 0 time length is valid.
bool MainControl::legalParticipant(const Participant& participant) const {
    if(participant.song()=="" || participant.state()=="" ||
       participant.singer()==""|| participant.timeLength()> time_limit){
           return false;
    }else{
           return true;
       }

}

bool MainControl::participate(string state)const{
    for(i=0;participants[i]!=nullptr;i++){
        if (participant[i].state() == state) return true;
    }
    return false;
}

void MainControl::setPhase(Phase phase){
  switch(phase){
    case Contest:
        if(this->phase == "Registration") this->phase = phase;
        if(this->phase == "Voting") this->phase = phase;
    case Voting:
        if(this->phase == "Contest") this->phase = phase;
    case Registration:
        if(this->phase == "Contest") this->phase = phase;
  }
}
