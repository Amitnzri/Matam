#ifndef EUROVISION_H_
#define EUROVISION_H_

#include <iostream>
#include <string>

// it's allowed to define here any using statements, according to needs.
// do NOT define here : using namespace std;
using std::string;
using std::ostream;
//---------------------------------------------------

enum VoterType { All, Regular, Judge };
enum Phase { Registration, Contest, Voting };

//---------------------------------------------------

class Participant
{
// relevant private members can be defined here, if necessary.
string state_name;
string song_name;
unsigned int song_length; //TODO:Check if unsigned is valid.
string singer_name;
bool is_registered;

public:

    Participant() = delete;
    Participant(Participant& copy) = delete;
    Participant& operator=(Participant& participant) = delete;
    ~Participant() =default;
    Participant(const string state_name,const string song_name
                 ,const unsigned int song_length, const string singer_name);
    string state() const;
    string song() const;
    unsigned int timeLength() const;
    string singer() const;
    bool isRegistered() const;
    void update(const string song_name,const unsigned int song_length,
                                       const string singer_name);
    void updateRegistered(const bool status);

// need to define here possibly c'tr and d'tr and ONLY methods that
// are mentioned and demonstrated in the test example that has been published.
// NO OTHER METHODS SHOULD APPEAR HERE.

// NO friend is allowed here.

};

//---------------------------------------------------


class Voter
{
// relevant private members can be defined here, if necessary.
//TODO: Check c'structor
string voter_state;
VoterType voter_type;
unsigned int num_of_votes;//TODO: Check if allowed.

public :
Voter() =delete; //TODO:Check
Voter(const string voter_state,const VoterType voter_type = Regular);
~Voter()=default; //TODO:Check
Voter& operator++();//TODO:Check with refferrence.
string state() const;
VoterType voterType() const;
unsigned int timesOfVotes() const;



// need to define here possibly c'tr and d'tr and ONLY methods that
// are mentioned and demonstrated in the test example that has been published.
// NO OTHER METHODS SHOULD APPEAR HERE.

// NO friend is allowed here.

};


// -----------------------------------------------------------

struct Vote
{
Voter& voter; //TODO:Check if pointer;
string* votes;
//TODO:Check Convention.
Vote(Voter& voter ,const string v1="",const string v2="",const string v3="",
const string v4="",const string v5="",const string v6="",const string v7="",
const string v8="",const string v9="",const string v10="");
~Vote();
// ALL is public here.
// need to define ONLY data members and c'tr and d'tr.
// NO NEED to define anything else.

};

// -----------------------------------------------------------


class MainControl
{
// relevant private members can be defined here, if necessary.
//TODO:Check convention.
    struct VotesCount{ //TODO:Check why typedef isn't needed.
        VotesCount(string state="");
        string state_name;
        unsigned int regular_votes;
        unsigned int judges_votes;
    };
    unsigned int findParticipantLocation(string state_name);
  //arrange the votes array by the order of the registered participants.
    void setVotesCount(); //
    void registerParticipant(Participant& participant);
    void removeParticipant(Participant& participant);
    void giveVotes(const Vote& vote);
    ostream& getParticipants(ostream& os) const;
    ostream& getVotes(ostream& os) const;

    const unsigned int time_limit;
    const unsigned int max_participant;
    const unsigned int votes_limit;
    Phase phase;
    VotesCount** votes;
    Participant** participants;


public:
    MainControl(const unsigned int time_limit=180,
       const unsigned int max_participant=26, const unsigned int votes_limit=5);
    ~MainControl();
    MainControl& operator+=(Participant& participant);
    MainControl& operator-=(Participant& participant);
    MainControl& operator+=(const Vote& vote);
    friend ostream& operator<<(ostream& os ,const MainControl& main_control);
    void setPhase(const Phase phase);
    bool legalParticipant(const Participant& participant)const;
    bool participate(const string state) const;
// need to define here possibly c'tr and d'tr and ONLY methods that
// are mentioned and demonstrated in the test example that has been published.
// NO OTHER METHODS SHOULD APPEAR HERE.

// Also it's allowed here to define friend.

};

// -----------------------------------------------------------

//OverLoads:
ostream& operator<<(ostream& os,const Participant& participant);
ostream& operator<<(ostream& os,const Voter& voter);


#endif
