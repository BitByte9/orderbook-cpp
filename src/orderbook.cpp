#include "orderbook.hpp"

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

string OrderBook::getQuote(int qty)
{
    // Implementation of getQuote
    // We will need to find lowest ask prices till the qty passed in is met we keep displaying lowest ask prices

    stable_sort(asks.begin(), asks.end(), [](const order &a, const order &b) {
    // If prices are equal, maintain the original order
    if (a.price == b.price) {
        // Compare based on the order of insertion
        return a.insertion_ask_id < b.insertion_ask_id;
    }
    // Otherwise, sort by price
    return a.price < b.price;
});

    for (auto it = asks.begin(); it != asks.end(); ++it)
    {
        if (qty > 0 && qty <= it->quantity)
        {
            cout << ticker << "-> "
                 << "Quantity available: " << qty << " at " << it->price << " USD" << endl; // make the output look better

            return "Quote retrieved successfully.";
        }
        else if (qty > 0 && qty > it->quantity)
        {
            cout << ticker << "-> "
                 << "Quantity available: " << it->quantity << " at " << it->price << " USD" << endl;
            qty -= it->quantity;
        }
        else
        {

            return "Quote retrieved successfully.";
        }
    }
    cout << "Quote retrieved successfully." << endl;
    return "Quote retrieved successfully.";
}

// DONE
string OrderBook::getDepth()
{
    // Sort asks in descending order of price
    sort(asks.begin(), asks.end(), [](const order &a, const order &b)
              { return a.price > b.price; });

    // Sort bids in descending order of price
    sort(bids.begin(), bids.end(), [](const order &a, const order &b)
              { return a.price > b.price; });
    string depthString = ticker + " Depth:\n";
    // Construct a string representation of the depth

    for (const auto &ask : asks)
    {
        depthString += "\x1b[31m"; // Set color to red
        depthString += "Price: " + to_string(ask.price) + ", Quantity: " + to_string(ask.quantity) + "\n";
        depthString += "\x1b[0m"; // Reset color to default
    }
    depthString += "Asks above:\n";
    depthString += "Bids below:\n";
    for (const auto &bid : bids)
    {
        depthString += "\x1b[32m"; // Set color to green
        depthString += "Price: " + to_string(bid.price) + ", Quantity: " + to_string(bid.quantity) + "\n";
        depthString += "\x1b[0m"; // Reset color to default
    }

    cout << depthString << endl;
    return depthString;
}


string OrderBook::getBalance(string username)
{
    // Check if username exists is users array if it does we cout the balances else we return an error message saying user not found
    if(users.find(username)!=users.end())
    {
        cout<<"User found" <<endl;
        cout<<"User balance is as follows: "<<endl;
        // Balance is a map of string and int so we need to iterate through the map to get the values
        for(auto it=users[username].user_balance.balance.begin();it!=users[username].user_balance.balance.end(); ++it)
        {
            // Print key : value format
            cout<<it->first<<" : "<<it->second<< endl;
        }
        return "Balance retrieved successfully.";
    }
    else
    {
        cout<<"User not found!!"<< endl;
        return "User not found";
    }
}




string OrderBook::addBalanace(string Username,string market, int value)
{
    if (users.find(Username)!=users.end())
    {
        users[Username].user_balance.add_balance(market, value);
        cout<<"Balance added successfully"<<endl;
        return "Balance added successfully";
    }
    cout<<"User not found!! Please enter the right Username to add balance!" << endl;
    return "User not found";
}

OrderBook::~OrderBook()
{
    //Destructor called
}

void OrderBook::cancelAsk(string user_name,int price,int quantity)
{
    for(auto it=asks.begin();it!=asks.end();it++)
    {
        if(it->user_name==user_name && it->price==price && it->quantity==quantity)
        {
            asks.erase(it);
            cout<<"Ask cancelled successfully";
            return;
        }
        else if(it->user_name==user_name && it->price==price && it->quantity>quantity)
        {
            it->quantity-=quantity;
            cout<<"ask is cancelled successfully";
            return ;
        }
        else if(it->user_name==user_name && it->price==price && it->quantity<quantity)
        {
            cout<<"Ask quantity is less than the quantity you want to cancel\n\n";
            cout<<"Please enter the right quantity to cancel and retry!\n\n";
            return;
        }

    }
}

void OrderBook::cancelBid(string Username,int Price,int Quantity){


    
    for(auto it = bids.begin(); it != bids.end(); ++it){
        if(it->user_name == Username && it->price == Price && it->quantity == Quantity){
            bids.erase(it);
            cout << "Bid cancelled successfully" << endl;
            return;
        }else if (it->user_name == Username && it->price == Price && it->quantity > Quantity){
            it->quantity -= Quantity;
            cout << "Bid cancelled successfully" << endl;
            return;
        }else if(it->user_name == Username && it->price == Price && it->quantity < Quantity){
            cout << "Bid quantity is less than the quantity you want to cancel\n\n";
            cout << "Please enter the right quantity to cancel and retry!\n\n";
            return;
           

        }   
    }
    cout <<"Bid not found!! Please enter the right Username, Price and Quantity to cancel the bid!" << endl;
    return;
}

int main()
{
    OrderBook EXCH;

   string market;
    int choice;
   string username;
    int price, quantity;

    cout<< "\n=========== " <<"WELCOME TO THE " << ticker << " MARKET "  << "AND HAPPY TRADING" << " =========== \n\n" << endl;
    cout<< "\n=========== " << "CURRENT MARKET PRICES " << " =========== " << endl;
    EXCH.getDepth(); // getDepth() is called to display the current market

    while (true)
    {
        cout<<"\n=========== " << ticker << " Trading Platform ===========\n\n";
        cout<<"\n========== Trading Platform Menu ==========\n";
        cout<<"1. Sign Up User\n";
        cout<<"2. Add Balance to User Account\n";
        cout<<"3. Check Current Market Prices\n";
        cout<<"4. Add Bid to " << ticker << " v USD market\n";
        cout<<"5. Sell your stocks in " << ticker << " v USD Market\n";
        cout<<"6. Get Current Quote to buy " << ticker << " stocks\n";
        cout<<"7. Check your current User Balance\n";
        cout<<"8. Cancel Bid\n";
        cout<<"9. Cancel Ask\n";
        cout<<"10. Exit\n\n";
        cout<<"Enter your choice: ";

       cin >> choice;

        switch (choice)
        {
        case 1:
           cout<<"Enter username for new user: \n";
           cin >> username;
            EXCH.makeUser(username);
            break;
        case 2:
           cout<<"Enter username to add balance: \n";
           cin>>username;
           cout<<"Enter market (e.g., USD): \n";
           cin>>market;
           cout<<"Enter balance value: \n";
            int value;
           cin>>value;
            EXCH.addBalanace(username, market, value);
            break;
        case 3:
            EXCH.getDepth();
            break;
        case 4:
           cout<<"Enter username for bid: \n";
           cin>>username;
           cout<<"Enter bid price: \n";
           cin>>price;
           cout<<"Enter bid quantity: \n";
           cin>>quantity;
            EXCH.add_bid(username, price, quantity);
            break;
        case 5:
           cout<<"Enter username for ask: \n";
           cin>>username;
           cout<<"Enter ask price: \n";
           cin>>price;
           cout<<"Enter ask quantity: \n";
           cin>>quantity;
            EXCH.add_ask(username, price, quantity);
            break;
        case 6:
           cout<<"Enter quantity for quote: \n";
           cin>>quantity;
            EXCH.getQuote(quantity);
            break;
        case 7:
           cout<<"Enter username to get balance: \n";
           cin>>username;
            EXCH.getBalance(username);
            break;
        case 8:
           cout<<"Enter username to cancel bid: \n";
           cin>>username;
           cout<<"Enter bid price: \n";
           cin>>price;
           cout<<"Enter bid quantity: \n";
           cin>>quantity;
            EXCH.cancelBid(username, price, quantity);
            break;
        case 9:
           cout<<"Enter username to cancel ask:\n";
           cin>>username;
           cout<<"Enter ask price: \n";
           cin>>price;
           cout<<"Enter ask quantity:\n";
           cin>>quantity;
            EXCH.cancelAsk(username,price,quantity);
            break;
        case 10:
           cout<<"Exiting the trading platform. Goodbye!\n\n";
            return 0;
        default:
           cout<<"Invalid choice. Please try again.\n\n";
        }
    }

    return 0;
}



