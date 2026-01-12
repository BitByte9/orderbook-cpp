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

string OrderBook:: add_bid(string user_name,int price,int quan)
{
    int remQty=quan;

    //sort in ascendign order
    stable_sort(asks.begin(),asks.end(), [](const order &a , const order &b){
        if(a.price==b.price)
        return a.insertion_ask_id < b.insertion_ask_id ;
        else
        {
            return a.price<b.price;

        }

    });

    //use logic to make than add the bid
    for(auto it= asks.begin();  it != asks.end();  )
    {
        if(remQty > 0 && price >= it->price)
        {
            if(remQty < it->quantity)
            {
                it->quantity-=remQty;

                flipBalance(user_name,it->user_name,price,remQty);

                cout<<"Bid satisfied at price"<<" "<<it->price<<" "<< "and quantity: "<<remQty<<endl;
                break ;
            }
            else
            {
                remQty -= it->quantity;
                flipBalance(user_name, it->user_name, it->quantity, it->price);
                cout << "Bid Satisfied Partially at price: " << it->price << " and quantity: " << it->quantity << endl;
                it = asks.erase(it); // get the next valid iterator after erasing

            }

        }
        else
        {
            it++;
        }
        

    }
    if(remQty>0)
    {
        order bid(user_name,"bid",price,remQty);
        bids.push_back(bid);
        cout<<"Remaining Quantity of the bid added to orderbook "<<endl;

    }
    if(remQty==0)
    {
        cout<<"complete bid satisfied successfully"<<endl;

    }

    return"Bid added/satisfied successfully";

}

string OrderBook::add_ask(string Username, int Price, int Quantity)
{
    
    int remQty=Quantity;
    
    stable_sort(bids.begin(), bids.end(), [](const order &a, const order &b) {
   
    if (a.price==b.price) {
        
        return a.insertion_bid_id<b.insertion_bid_id;
    }

    return a.price>b.price;
});

    // use logic from the commented function above
    for(auto it=bids.begin();it!=bids.end(); )
    {
        if (remQty>0 && Price<=it->price)
        {
            if (it->quantity> remQty)
            {
                it->quantity-=remQty;
                flipBalance(it->user_name,Username, remQty,it->price);
                cout<<"Ask Satisfied Successfully at price: "<<it->price<<" and quantity: " << remQty << endl;
                remQty=0;
                break;
            }
            else
            {
                remQty-=it->quantity;
                flipBalance(it->user_name, Username, it->quantity, it->price);
                cout<<"Ask Satisfied Partially at price: " << it->price << " and quantity: " << it->quantity << endl;
                it=bids.erase(it); 
            }
        }
        else
        {
            it++; 
        }
    }

    if(remQty>0)
    {
        order ask(Username,"ask",Price,remQty);
        asks.push_back(ask);
        cout <<"Remaining quantity of asks added to Orderbook"<< endl;
    }

    if(remQty==0)
    {
        cout<<"Complete Ask Satisfied Successfully"<<endl;
    }
    return "Ask added successfully."; 
}





