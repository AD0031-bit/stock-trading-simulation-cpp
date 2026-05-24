#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

// Base Class
class Person
{
protected:
    string name;

public:
    Person(string n = "")
    {
        name = n;
    }

    virtual void display()
    {
        cout << "User Name: " << name << endl;
    }
};

// Stock Class
class Stock
{
public:
    string symbol;
    string company;
    float price;

    Stock(string s = "", string c = "", float p = 0)
    {
        symbol = s;
        company = c;
        price = p;
    }

    void showStock()
    {
        cout << symbol << " - " << company
             << " | Price: Rs." << price << endl;
    }
};

// Transaction Class
class Transaction
{
public:
    string type;
    string symbol;
    int qty;
    float amount;

    Transaction(string t, string s, int q, float a)
    {
        type = t;
        symbol = s;
        qty = q;
        amount = a;
    }

    void show()
    {
        cout << type << " " << qty << " shares of "
             << symbol << " for Rs." << amount << endl;
    }
};

// Portfolio Class
class Portfolio
{
private:
    vector<pair<string, int>> holdings;

public:
    void buyStock(string symbol, int qty)
    {
        for (int i = 0; i < holdings.size(); i++)
        {
            if (holdings[i].first == symbol)
            {
                holdings[i].second += qty;
                return;
            }
        }
        holdings.push_back(make_pair(symbol, qty));
    }

    void sellStock(string symbol, int qty)
    {
        for (int i = 0; i < holdings.size(); i++)
        {
            if (holdings[i].first == symbol)
            {
                if (holdings[i].second >= qty)
                {
                    holdings[i].second -= qty;
                }

                if (holdings[i].second == 0)
                {
                    holdings.erase(holdings.begin() + i);
                }
                return;
            }
        }
    }

    int getQuantity(string symbol)
    {
        for (int i = 0; i < holdings.size(); i++)
        {
            if (holdings[i].first == symbol)
                return holdings[i].second;
        }
        return 0;
    }

    void showPortfolio()
    {
        cout << "\n--- Portfolio ---\n";
        for (int i = 0; i < holdings.size(); i++)
        {
            cout << holdings[i].first
                 << " : " << holdings[i].second << " shares\n";
        }
    }
};

// User Class (Inheritance)
class User : public Person
{
private:
    float balance;

public:
    Portfolio portfolio;
    vector<Transaction> history;

    User(string n, float b) : Person(n)
    {
        balance = b;
    }

    float getBalance()
    {
        return balance;
    }

    void addBalance(float amt)
    {
        balance += amt;
    }

    void deductBalance(float amt)
    {
        balance -= amt;
    }

    void addTransaction(Transaction t)
    {
        history.push_back(t);
    }

    void showHistory()
    {
        cout << "\n--- Transaction History ---\n";
        for (int i = 0; i < history.size(); i++)
        {
            history[i].show();
        }
    }

    void display() // Polymorphism
    {
        cout << "\nUser: " << name
             << "\nBalance: Rs." << balance << endl;
    }
};

// Market Class
class Market
{
public:
    vector<Stock> stocks;

    Market()
    {
        stocks.push_back(Stock("TCS", "Tata Consultancy", 3500));
        stocks.push_back(Stock("INFY", "Infosys", 1450));
        stocks.push_back(Stock("RELI", "Reliance", 2600));
    }

    void showMarket()
    {
        cout << "\n--- Stock Market ---\n";
        for (int i = 0; i < stocks.size(); i++)
        {
            stocks[i].showStock();
        }
    }

    Stock* findStock(string symbol)
    {
        for (int i = 0; i < stocks.size(); i++)
        {
            if (stocks[i].symbol == symbol)
                return &stocks[i];
        }
        return NULL;
    }

    void updatePrices()
    {
        for (int i = 0; i < stocks.size(); i++)
        {
            int change = rand() % 201 - 100;
            stocks[i].price += change;

            if (stocks[i].price < 100)
                stocks[i].price = 100;
        }
    }
};

// Save Data
void saveData(User &u)
{
    ofstream file("history.txt");

    for (int i = 0; i < u.history.size(); i++)
    {
        file << u.history[i].type << " "
             << u.history[i].symbol << " "
             << u.history[i].qty << " "
             << u.history[i].amount << endl;
    }

    file.close();
}

int main()
{
    srand(time(0));

    User user("User", 100000);
    Market market;

    int choice;

    do
    {
        cout << "\n====== STOCK PORTFOLIO SIMULATOR ======\n";
        cout << "1. View Market\n";
        cout << "2. Buy Stock\n";
        cout << "3. Sell Stock\n";
        cout << "4. View Portfolio\n";
        cout << "5. View Balance\n";
        cout << "6. View History\n";
        cout << "7. Update Prices\n";
        cout << "8. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        if (choice == 1)
        {
            market.showMarket();
        }

        else if (choice == 2)
        {
            string sym;
            int qty;

            cout << "Enter Symbol: ";
            cin >> sym;
            cout << "Enter Quantity: ";
            cin >> qty;

            Stock *s = market.findStock(sym);

            if (s != NULL)
            {
                float cost = s->price * qty;

                if (user.getBalance() >= cost)
                {
                    user.deductBalance(cost);
                    user.portfolio.buyStock(sym, qty);
                    user.addTransaction(Transaction("BUY", sym, qty, cost));
                    cout << "Stock Purchased Successfully!\n";
                }
                else
                {
                    cout << "Insufficient Balance!\n";
                }
            }
        }

        else if (choice == 3)
        {
            string sym;
            int qty;

            cout << "Enter Symbol: ";
            cin >> sym;
            cout << "Enter Quantity: ";
            cin >> qty;

            Stock *s = market.findStock(sym);

            if (s != NULL &&
                user.portfolio.getQuantity(sym) >= qty)
            {
                float amount = s->price * qty;

                user.addBalance(amount);
                user.portfolio.sellStock(sym, qty);
                user.addTransaction(Transaction("SELL", sym, qty, amount));

                cout << "Stock Sold Successfully!\n";
            }
            else
            {
                cout << "Not enough shares!\n";
            }
        }

        else if (choice == 4)
        {
            user.portfolio.showPortfolio();
        }

        else if (choice == 5)
        {
            cout << "Available Balance: Rs."
                 << user.getBalance() << endl;
        }

        else if (choice == 6)
        {
            user.showHistory();
        }

        else if (choice == 7)
        {
            market.updatePrices();
            cout << "Prices Updated!\n";
        }

        else if (choice == 8)
        {
            saveData(user);
            cout << "Data Saved. Exiting...\n";
        }

    } while (choice != 8);

    return 0;
}