#include <iostream>
#include <string>
using std::string;
using std::exception;

const int DISABLE_BASE_PRICE = 200;
const int MAIN_HALL_PRICE = 100;
const int FRONT_REGULAR_PRICE = 500;
const int MIDDLE_REGULAR_PRICE = 250;
const int GOLDEN_CIRCLE_PRICE =1000;
const int SPECIAL_SEAT_PRICE = 300;

// ---------------------------------------------
class NoPrice : public exception
{
public:
    const char* what()const noexcept override;
};

// ---------------------------------------------
class Seat
{
protected:
    int line;
    int chair;

public:
    Seat(int line, int chair);
    virtual string location() const= 0;
    virtual int price() const = 0;
};

// ---------------------------------------------
class GreenRoomSeat : public Seat
{
public:
    GreenRoomSeat(int line,int chair);
    string location() const override;
    int price() const override;
};

// ---------------------------------------------
class MainHallSeat : public Seat
{
protected:
  int base_price;
public:
  MainHallSeat(int line,int chair,int base_price);
  string location() const override = 0;
  int price() const override = 0;
};

// ---------------------------------------------
class SpecialSeat : public MainHallSeat
{
public:
    SpecialSeat(int line,int chair,int base_price);
    string location()const override = 0;
    int price()const override = 0;
};

// ---------------------------------------------
class GoldenCircleSeat : public SpecialSeat
{
public:
    GoldenCircleSeat(int line,int chair, int base_price);
    string location() const override;
    int price()const override;
};

// ---------------------------------------------
class DisablePodiumSeat : public SpecialSeat
{
public:
  DisablePodiumSeat(int line, int chair, int base_price = DISABLE_BASE_PRICE);
  string location() const override;
  int price() const override;
};

// ---------------------------------------------
class RegularSeat : public MainHallSeat
{
protected:
    char area;
public:
  RegularSeat(char area,int line,int chair,int base_price);
  string location() const override =0;
  int price() const override =0;
};

// ---------------------------------------------
class FrontRegularSeat : public RegularSeat
{
public:
  FrontRegularSeat(char area, int line,int chair,int base_price);
  string location()const override;
  int price()const override;
};

// ---------------------------------------------
class MiddleRegularSeat : public RegularSeat
{
public:
  MiddleRegularSeat(char area, int line,int chair,int base_price);
  string location()const override;
  int price()const override;
};

// ---------------------------------------------
class RearRegularSeat : public RegularSeat
{
public:
  RearRegularSeat(char area, int line,int chair,int base_price);
  string location()const override;
  int price()const override;
};

// ---------------------------------------------
