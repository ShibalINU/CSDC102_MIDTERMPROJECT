#include <iostream>
#include <string>
#include <vector>
using namespace std;

void clientMenu(vector<string> &cardNumbers,
                vector<string> &pins,
                vector<double> &balances
                /*, ... other vectors if needed */)
{

    string cardNum_user;
    cout << "Enter Card Number: ";
    cin >> cardNum_user;

    for (int i = 0; i < cardNumbers.size(); i++)
    {
        if (cardNum_user == cardNumbers[i])
        {

            string userPin;
            cout << "Enter PIN: ";
            cin >> userPin;

            if (userPin == pins[i])
            {
                break;
            }

            else
            {
                cout << "Wrong pw";
            }
        }
        else
        {
            cout << "Account not found.\n";
        }
    }
}

int main()
{

    vector<string> cardNumbers = {"67891026", "12345678"};
    vector<string> pins = {"3456", "8976"};
    vector<double> balances = {2, 6};

    clientMenu(cardNumbers, pins, balances);

    return 0;
}
