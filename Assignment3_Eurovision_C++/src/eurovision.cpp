#include "../includes/eurovision.h"//TODO:Change before submission.
#include "../includes/get.h"
using std::cout;
using std::endl;
using std::to_string;

/* ------------------------------------Vote----------------------------------*/

Vote::Vote(Voter& voter ,const string vote_a,const string vote_b,
    const string vote_c,const string vote_d,const string vote_e,
    const string vote_f,const string vote_g,const string vote_h,
    const string vote_i,const string vote_j):
          voter(voter),votes(new string[10]{vote_a,vote_b,vote_c,vote_d,vote_e,
                                          vote_f,vote_g,vote_h,vote_i,vote_j}){}

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
    //Updates the fields only if hasn't been given by default values ("" or 0).
    this->song_name=(song_name == "") ? (this->song_name) : (song_name);
    this->song_length=(song_length == 0) ? (this->song_length) : (song_length);
    this->singer_name=(singer_name == "") ? (this->singer_name) : (singer_name);
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
    switch(voter.voterType()){
        case Regular:
            os << '<' << voter.state() << "/Regular>";
            break;
        case Judge:
            os << '<' << voter.state() << "/Judge>";
            break;

        default:
            break; //Shouldn't get here.
    }
    return os;
}

VoterType Voter::voterType() const{
    return voter_type;
}

//Return the number of times a Voter has voted.
unsigned int Voter::timesOfVotes() const{
    return num_of_votes;
}

Voter& Voter::operator++(){
    num_of_votes++; //Increments the number of times a voter had voted.
    return *this;
}
/*---------------------------------------------------------------------------*/
/*-----------------------------MainControl-----------------------------------*/

MainControl::MainControl(const unsigned int time_limit,
          const unsigned int max_participant,const unsigned int votes_limit):
          time_limit(time_limit), max_participant(max_participant),
          votes_limit(votes_limit), phase(Registration),
          votes(new VotesCount*[max_participant]{}),
          participants(new Participant*[max_participant]{}){}

MainControl::~MainControl(){
    for(unsigned int i=0; i<max_participant&&participants[i]!=nullptr;i++){
        //Unregister all the participants.
        *participants[i].updateRegistered(false);

        //Deletes all the VotesCounts (if phase isn't Registration).
        if(phase != Registration){
            delete votes[i];
        }
    }
    delete[]votes;
    delete[]participants;
}

MainControl::Iterator::Iterator(Participant** ptr):participant_ptr(ptr){}

MainControl::Iterator& MainControl::Iterator::operator++(){
    participant_ptr++; //Increments the iterator by one.
    return *this;
}

bool MainControl::Iterator::operator<(Iterator iterator) const{
    /*Checks if an address that a given iterator points to,
      is bigger than an address that this iterator points to.*/
    return participant_ptr < iterator.participant_ptr;
}

bool MainControl::Iterator::operator==(Iterator iterator) const{
    //Checks if a given iterator points on the same participant.
    return participant_ptr == iterator.participant_ptr;
}

Participant& MainControl::Iterator::operator*() const{
    return **participant_ptr;
}

MainControl::Iterator MainControl::begin() const{
    //Returns an iterator that points to the begginning of participants.
    return Iterator(participants);
}

MainControl::Iterator MainControl::end() const{
    //Returns an iterator that points to the end of participants.
    return Iterator(participants+countParticipants());
}

MainControl::VotesCount::VotesCount(string state): state_name(state),
                                            regular_votes(0),judges_votes(0){}


//Creates a VotesCounts for every registered state and stores it in votes.
void MainControl::setVotesCount(){
    for(unsigned int i=0;i<max_participant && participants[i]!=nullptr;i++){
        votes[i] = new VotesCount(participants[i]->state());
    }
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
        for(unsigned int i =0; i< max_participant;i++){
            if (participants[i] == &participant) removeParticipant(participant);
        }
    }
    return *this;
}

MainControl& MainControl::operator+=(const Vote& vote){
    if(phase == Voting){
        giveVotes(vote);
    }
    return *this;
}

string MainControl::operator()(int i,VoterType type){
    if (i > countParticipants() || i < 1) return "";
    class Compare{ //Compare functor for sorting the states by votes.
        VoterType type;
        public:
        Compare(VoterType type):type(type){}
        bool operator()(VotesCount** state_a,VotesCount** state_b){
            int sum_of_votes_a;
            int sum_of_votes_b;
            switch (type){
                case Regular:
                {
                    sum_of_votes_a = (*state_a)->regular_votes;
                    sum_of_votes_b = (*state_b)->regular_votes;
                    break;
                }
                case Judge:
                {
                    sum_of_votes_a = (*state_a)->judges_votes;
                    sum_of_votes_b = (*state_b)->judges_votes;
                    break;
                }
                case All:
                {
                    sum_of_votes_a = (*state_a)->judges_votes +
                                     (*state_a)->regular_votes;
                    sum_of_votes_b = (*state_b)->regular_votes +
                                     (*state_b)->judges_votes;
                    break;
                }
            }
            return (sum_of_votes_a>=sum_of_votes_b);
        }
    };
    //Return the requsted states VotesCount, end if isn't one.
    auto begin = votes; //pointer the the start of votes array.
    auto end = votes + countParticipants(); //pointer to the end of votes array.
    //Returns the state name if exists or "" if isn't.
    return get(begin,end,i,Compare(type))->state_name;
 }

ostream& operator<<(ostream& os, const MainControl& main_control){
    switch(main_control.phase){
        case Registration:
            os << '{' <<endl << "Registration" <<endl;
            main_control.getParticipants(os)<<'}'<<endl;
            break;
        case Contest:
            os << '{' << endl << "Contest" <<endl << '}' <<endl;
            break;
        case Voting:
            os << '{' <<endl << "Voting" <<endl;
            main_control.getVotes(os) <<'}'<<endl;
            break;
    }
    return os;
}

//Checks if participant is legal.
bool MainControl::legalParticipant(const Participant& participant) const {
    if(participant.song()=="" || participant.state()=="" ||
       participant.singer()=="" || participant.timeLength() > time_limit){
        return false;
    }else{
        return true;
    }
}

bool MainControl::participate(string state)const{
    for(unsigned int i=0;i<max_participant && participants[i]!=nullptr;i++){
        if (participants[i]->state() == state) return true;
    }
    return false;
}

void MainControl::setPhase(Phase phase){
  switch(phase){
      case Contest:
          if(this->phase == Registration) {
              this->phase = phase;
              setVotesCount(); //Create the votes array from participants.
          }
          break;

      case Voting:
          if(this->phase == Contest) this->phase = phase;
          break;

      default:
          break; //shouldn't get here.
  }
}

ostream& MainControl::getVotes(ostream& os) const{
      for(unsigned int i=0;i<max_participant&& votes[i]!=nullptr;i++){
          os << votes[i]->state_name << " : Regular(" << votes[i]->regular_votes
          << ") Judge(" << votes[i]->judges_votes << ")"<<endl;
      }
      return os;
}

ostream& MainControl::getParticipants(ostream& os) const{
      for(unsigned int i=0;i<max_participant && participants[i]!=nullptr;i++){
          os << *participants[i]<<endl;
      }
      return os;
}
//Adds a given participant to participants array by alphabetical order.
void MainControl::registerParticipant(Participant& participant){
    if(participants[max_participant-1]!= nullptr) return;
    Participant* participant_ptr = &participant;
    unsigned int i=0;
    for(;participants[i]!=nullptr&&i<max_participant;i++)
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

//Returns the index of a participant in the participants array.
unsigned int MainControl::findParticipantLocation(string state_name){
    unsigned int i=0;
    for(;participants[i]->state()!=state_name;i++);
    return i;
}

//Removes a given participant from the participants array.
void MainControl::removeParticipant(Participant& participant){
      participant.updateRegistered(false);
      unsigned int i=findParticipantLocation(participant.state());
      for(;participants[i]!=nullptr&&(i+1)<max_participant;i++){
        participants[i]=participants[i+1];
      }
      participants[i]=nullptr;
}

void MainControl::giveVotes(const Vote& vote){
    Voter& voter = vote.voter;
    switch(voter.voterType()){
        case Judge:
            //Check if the vote is valid.
            if(!participate(voter.state()) || voter.timesOfVotes()!=0)return;
            for(unsigned int i=0;i<10;i++){
                if(voter.state()!=vote.votes[i] && participate(vote.votes[i])){
                  unsigned int j = findParticipantLocation(vote.votes[i]);
                  votes[j]->judges_votes+=(i<3)?(12-2*i):(10-i);
                  ++voter;
                }
            }
            break;

        case Regular:{
            //Check if voter's state exists.
            if(!participate(voter.state())||voter.state()==vote.votes[0])return;
            if(!participate(vote.votes[0])||voter.timesOfVotes()>=votes_limit){
                return;
            }
            unsigned int i = findParticipantLocation(vote.votes[0]);
            votes[i]->regular_votes++;
            ++voter;
            break;
        }

        default:
            break; //Shouldn't get here.
    }
}

int MainControl::countParticipants() const{
    for (int i=0;i<int(max_participant);i++){
        if(!participants[i]) return i;
    }
    return max_participant;
}

/*---------------------------------------------------------------------------*/
