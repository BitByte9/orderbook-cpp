#include <orderbook.hpp>
#include <iostream>
#include <iomanip> // For setw() and setfill()
#include <algorithm>

using namespace std;

//intialising the static varibale of struct order
int order::counter_ask=0; 
int order::counter_bid=0; 

void OrderBook:: flipBalance(const string& userId1, const string& userId2,double quantity,double price)
{
    if(users.find(userId1)!=users.end() && users.find(userId2)!=users.end())
    {
        if(users[userId1].user_balance.balance["USD"]>=quantity*price)
        {
            if(users[userId2].user_balance.balance[ticker]>=quantity)
            {
                users[userId1].user_balance.balance["USD"]-=quantity*price;
                users[userId1].user_balance.balance[ticker]+=quantity;

                users[userId2].user_balance.balance[ticker]-=quantity;
                users[userId2].user_balance.balance["USD"]+=quantity*price;
                cout<<"funds and stocks are transferred"<<endl;

            }
            else
            {
                cout<<"USer doesnt have enough balance to transfer fund"<<endl;
            }

        }
        else{
            cout<<"User not have enough balance to buy stocks"<<endl;
        }

    }
    else{
        cout<<"One of the two user dont exist"<<endl;
    }
}

OrderBook::OrderBook()
{
    // Implementation of OrderBook constructor

    // Creating a couple of market maker users with predefined balances and bids/asks
    Balances balance1("USD", 10000);
    balance1.add_balance(ticker, 1000);
    Users marketMaker1("MarketMaker1", balance1); // User is made
    users["MarketMaker1"] = marketMaker1;        // Adding bids and asks for market makers

    order bid1("MarketMaker1", "bid", 110, 10);
    order ask1("MarketMaker1", "ask", 115, 5);
    order bid2("MarketMaker2", "bid", 111, 8);
    order ask2("MarketMaker2", "ask", 119, 12);

    bids.push_back(bid1);
    asks.push_back(ask1);
    bids.push_back(bid2);
    asks.push_back(ask2);

    // Creating a couple of market maker users with predefined balances and bids/asks, with different balances to start maintaining liquidity in the order book
    Balances balance2("USD", 10000);
    balance2.add_balance(ticker, 2000);
    Users marketMaker2("MarketMaker2", balance2); // User is made
    users["MarketMaker2"] = marketMaker2;        // Adding bids and asks for market makers
    order bid3("MarketMaker2", "bid", 109, 10);
    order ask3("MarketMaker2", "ask", 125, 5);
    order bid4("MarketMaker2", "bid", 112, 8);
    order ask4("MarketMaker2", "ask", 120, 12);

    bids.push_back(bid3);
    asks.push_back(ask3);
    bids.push_back(bid4);
    asks.push_back(ask4);

    // Creating user 3 with predefined balances and bids/asks
    // market maker 3 has a lot of USD and wants to buy GOOGL
    Balances balance3("USD", 50000);
    balance3.add_balance(ticker, 0);
    Users marketMaker3("MarketMaker3", balance3); // User is made
    users["MarketMaker3"] = marketMaker3;        // Adding bids and asks for market makers
    order bid5("MarketMaker3", "bid", 105, 10);
    order bid6("MarketMaker3", "bid", 108, 10);
    bids.push_back(bid5);
    bids.push_back(bid6);
}


string OrderBook:: makeUser(string name)
{
    Users ob(name);
    users[name]=ob;
    cout<<"User: "<<name<<" "<<"created successfully"<<endl;
    return "user created successfully";


}

