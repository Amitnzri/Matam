
#include "../includes/seat.h"

using std::to_string;

/*------------------------------$ Seats $ -----------------------------------*/
Seat::Seat(int line, int chair): line(line),chair(chair){}

/*----------------------------* GreenRoom *----------------------------------*/
GreenRoomSeat::GreenRoomSeat(int line,int chair) : Seat(line,chair){}

string GreenRoomSeat::location() const {
    return "Green Room-> line: " + to_string(line) + ", chair: " +
                                                              to_string(chair);
}

int GreenRoomSeat::price() const {
  throw NoPrice();
}
/*--------------------------* MainHallSeats *--------------------------------*/
MainHallSeat::MainHallSeat(int line, int chair, int base_price):
    Seat(line,chair),base_price(base_price){}

/*--------------------------- RegularSeats ----------------------------------*/
RegularSeat::RegularSeat(char area,int line,int chair,int base_price):
    MainHallSeat(line,chair,base_price),area(area){}
    /*--------------------- RearRegularSeat -----------------------------*/
RearRegularSeat::RearRegularSeat(char area,int line, int chair,int base_price)
    : RegularSeat(area,line,chair,base_price){}

string RearRegularSeat::location()const {
    return "Rear-> area: " + string(1,area) + ", line: " + to_string(line) +
                                             ", chair: " + to_string(chair);
}

int RearRegularSeat::price()const{
        return base_price + MAIN_HALL_PRICE;
}
    /*--------------------- MiddleRegularSeat ------------------------------*/
MiddleRegularSeat::MiddleRegularSeat(char area,int line,int chair,
    int base_price): RegularSeat(area,line,chair,base_price){}

string MiddleRegularSeat::location() const {
  return "Middle-> area: " + string(1,area) + ", line: "+ to_string(line) +
                                                ", chair: " + to_string(chair);
}
int MiddleRegularSeat::price() const {
  return base_price + MAIN_HALL_PRICE + MIDDLE_REGULAR_PRICE;
}

    /*--------------------- FrontRegularSeat ------------------------------*/
FrontRegularSeat::FrontRegularSeat(char area,int line, int chair,int base_price)
    : RegularSeat(area,line,chair,base_price){}

string FrontRegularSeat::location()const {
    return "Front-> area: " + string(1,area) + ", line: " + to_string(line) +
                                                ", chair: " + to_string(chair);
}

int FrontRegularSeat::price()const {
    return base_price + MAIN_HALL_PRICE + FRONT_REGULAR_PRICE;
}


/*--------------------------- SpecialSeats ----------------------------------*/
SpecialSeat::SpecialSeat(int line,int chair,int base_price):
    MainHallSeat(line,chair,base_price){}

   /*-----------------------GoldenCircleSeat-----------------------------*/
GoldenCircleSeat::GoldenCircleSeat(int line,int chair,int base_price):
    SpecialSeat(line,chair,base_price){}

string GoldenCircleSeat::location() const {
  return "Golden Circle-> line: " + to_string(line) + ", chair: "
                                                        + to_string(chair);
}

int GoldenCircleSeat::price() const {
  return MAIN_HALL_PRICE + SPECIAL_SEAT_PRICE + GOLDEN_CIRCLE_PRICE +base_price;
}


    /*-----------------------DisablePodiumSeat---------------------------*/
DisablePodiumSeat::DisablePodiumSeat(int line,int chair,int base_price):
    SpecialSeat(line,chair,base_price){}

int DisablePodiumSeat::price()const {
    return DISABLE_BASE_PRICE;
}

string DisablePodiumSeat::location()const {
    return "Disable Podium-> line: " + to_string(line) + ", chair: " +
                                                            to_string(chair);
}

/*------------------------------- No Price ----------------------------------*/
const char* NoPrice::what()const noexcept{
  return "Not For Sale !";
}
