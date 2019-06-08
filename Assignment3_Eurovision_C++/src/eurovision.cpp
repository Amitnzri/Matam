#include "../includes/eurovision.h"//TODO:Change before submission.
using std::cout;
using std::endl;
using std::to_string;
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
          votes(new VotesCount*[max_participant]{nullptr}),
          participants(new Participant*[max_participant]{nullptr}){}
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

MainControl& MainControl::operator+=(Participant& participant){
    if(!participate(participant.state()) && legalParticipant(participant)&&
                                                          phase==Registration){
        registerParticipant(participant);
    }
    return *this;
}

MainControl& MainControl::operator-=(Participant& participant){
    if(participate(participant.state())&&phase==Registration){
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

//TODO: Check why Ostream cannot get ostream by <<
ostream& operator<<(ostream& os, const MainControl& main_control){ //TODO: Understand why ostream << needs pf.
    switch(main_control.phase){
        case Registration:
            os << '{' <<endl << "Registration" <<endl;
            main_control.getParticipants(os) << endl <<'}';
        case Voting:
            os << '{' <<endl << "Voting" <<endl;
            main_control.getVotes(os) <<'}';

    }
    return os;
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
    for(int i=0;participants[i]!=nullptr;i++){
        if (participants[i]->state() == state) return true;
    }
    return false;
}

void MainControl::setPhase(const Phase phase){
  switch(phase){
      case Contest:
          if(this->phase == Registration) this->phase = phase;
      case Voting:
          if(this->phase == Contest) this->phase = phase;
  }
}

ostream& MainControl::getVotes(ostream& os) const{
      for(int i=0;votes[i]!=nullptr;i++){
          //<state> : Regular(num) Judge(num)
          os << votes[i]->state_name << " : Regular(" << votes[i]->regular_votes
          << ") Judge(" << votes[i]->judges_votes << ")";

      }
      return os;
}

ostream& MainControl::getParticipants(ostream& os) const{
      for(int i=0;participants[i]!=nullptr;i++){
          os << participants[i]<<endl;
      }
      return os;
}

void MainControl::registerParticipant(Participant& participant){
    if(participants[max_participant-1]!= nullptr) return;
    Participant* participant_ptr = &participant;
    int i=0;
    for(;participants[i]!=nullptr;i++)
    {
        if(participant_ptr->state() < participants[i]->state()){
            Participant& tmp = *participants[i];
            participants[i] = participant_ptr;
            participant_ptr = &tmp;
        }
    }
    participants[i] = participant_ptr;
    participant.updateRegistered(true);
}

/*---------------------------------------------------------------------------*/
