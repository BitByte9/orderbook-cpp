#ifndef ORDERBOOK-HPP;
#define ORDERBOOK-HPP;
#define ll long long

#include <bits/stdc++.h>;
#include <string>;

using namespace std;


//ticker is the stock we are trading 
string ticker="GOOGLE";

struct Balances{
    unordered_map <string,ll> balance; // it store USD: val , "google":val
    //default constructor to intialize the user holding
    Balances()
    {
        balance["USD"]=0;
        balance[ticker]=0;


    }
    Balances(string market,ll value)
    {
        balance[market]=value;
    }
    string add_balance(string market, ll value)
    {
        //check if market is already present or not
        if(balance.find(market)!= balance.end())
        {
            balance[market]+=value;
            return "balance added succcesfully";
        }
        balance[market]=value;
        return "balance added successfully";
    }

};

//store unique user and balances 
struct Users{
     string user_name;
     Balances user_balance;

     Users(string name)
     {
          user_name=name;
          user_balance=Balances();
     }



};


struct order{
     string user_name;
     string side;
     ll price;
     ll quantity;

     static int counter_bid;//static counter to tranck order number
     static int counter_ask;

     int insertion_bid_id;
     int insertion_ask_id;

     order(string name,string type,ll pri, ll quan)
     {
          user_name=name;
          side=type;
          price=pri;
          quantity=quan;

          if(side=="bid")
          {
               insertion_bid_id=counter_bid++;
          }
          else{
               insertion_ask_id=counter_ask++;
          }
     }


};

class OrderBook{
private:

    vector<order> bids; // stores all bids in order of price and time --> O(n) lookup
    vector<order> asks; // stores all asks in order of price and time --> O(n) lookup
    unordered_map <std :: string,Users> users; // stores unique user name and balances for each user --> O(1) lookup no need to be in order 
    void flipBalance(const std::string& userId1, const std::string& userId2, double quantity, double price);

    public:
    OrderBook(); // constructor
   ~OrderBook(); // destructor 
    string add_bid(std :: string Username, int Price, int Quantity); // adds a bid or ask to the order book
    string add_ask(std :: string Username, int Price, int Quantity); // adds a bid or ask to the order book
    string getBalance(std::string username); // returns the balance of a user
    string getQuote(int qty); // returns the best bid and ask prices and quantities
    string getDepth(); // returns the entire order book and shows all bids and asks
    string makeUser(string); // creates a new user for people trying to join the market
    string addBalanace(string Username, string market, int value); // adds balance to a user
    void cancelBid(string Username, int Price, int Quantity); // cancels a bid or ask from the order book
    void cancelAsk(string Username, int Price, int Quantity); // cancels a bid or ask from the order book

};







#endif