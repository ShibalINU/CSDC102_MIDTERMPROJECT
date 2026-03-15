#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
using namespace std;

//-----color codes for console output-----
const string RESET = "\033[0m";      // Reset to default
const string RED = "\033[31m";       // Red text
const string GREEN = "\033[32m";     // Green text
const string YELLOW = "\033[33m";    // Yellow text
const string BLUE = "\033[34m";      // Blue text
const string MAGENTA = "\033[35m";   // Magenta text
const string CYAN = "\033[36m";      // Cyan text
const string WHITE = "\033[37m";     // White text
const string BOLD = "\033[1m";       // Bold text
const string FAINT = "\033[2m";      // Faint text
const string ITALIC = "\033[3m";     // Italic text
const string UNDERLINE = "\033[4m";  // Underline text
const string SLOW_BLINK = "\033[5m"; // Slow blink text

const string BOLD_OFF = "\033[22m";      // Bold off
const string ITALIC_OFF = "\033[23m";    // Italic off
const string UNDERLINE_OFF = "\033[24m"; // Underline off

// Background color
const string BLACK_BG = "\033[40m";   // Set background color to black
const string RED_BG = "\033[41m";     // Set background color to red
const string GREEN_BG = "\033[42m";   // Set background color to green
const string YELLOW_BG = "\033[43m";  // Set background color to yellow
const string BLUE_BG = "\033[44m";    // Set background color to blue
const string MAGENTA_BG = "\033[45m"; // Set background color to magenta
const string CYAN_BG = "\033[46m";    // Set background color to cyan
const string WHITE_BG = "\033[47m";   // Set background color to white
const string DEFAULT_BG = "\033[49m"; // Reset background color to default

//-----------Global variables for in-memory storage )-----------------------
// bank and cash data
const int NUM_BANKS = 4;
const int NUM_DENOMINATIONS = 3;
string bankNames[NUM_BANKS] = {"BDO", "BPI", "Metrobank", "Security Bank"};
double localFees[NUM_BANKS] = {25, 20, 30, 15};
double intlFees[NUM_BANKS] = {150, 125, 200, 100};
double dailyLimits[NUM_BANKS] = {50000, 75000, 100000, 60000};

int denominations[NUM_DENOMINATIONS] = {100, 500, 1000};
int billCount[NUM_DENOMINATIONS] = {500, 500, 500};

// Vectors for transaction history
vector<string> transactionTypes;
vector<double> transactionAmounts;
vector<double> transactionFees;
vector<string> transactionDates;
vector<string> transactionTimes;
vector<string> transactionCards;

//--------------------------------------------------------------------Function declarations---------------------------------------------
int login(string &adminPasscode, vector<string> &cardNumbers, vector<string> &encodedPINs);

void clientMenu(vector<string> &cardNumbers,
                vector<string> &encodedPINs,
                vector<double> &balances,
                vector<string> &userBanks,
                vector<string> &accountTypes);

void adminMenu(vector<string> &cardNumbers,
               vector<string> &encodedPINs,
               vector<double> &balances,
               vector<string> &userBanks,
               vector<string> &accountTypes,
               string &adminPasscode);

string encodeString(string plain);
string decodeString(string encoded);
void displayDateTime();
void clearScreen();
bool validateCardNumber(vector<string> &cardNumbers, string userCard, int &accountIndex);
bool withdraw(vector<double> &balances, int accountIndex, double withdrawAmount);
bool withdraw(vector<double> &balances, int accountIndex, string presetAmount);
double calculateFeeRecursive(vector<double> &balances, string currentUserBank, string userAccountType, int accountIndex, int iterations);
void calculateBills(double withdrawAmount, int &bills1000, int &bills500, int &bills100);
void logTransaction(const string &cardNum, const string &type, double amount, double fee);
void refillCash(int denom[], int billCount[]);
void viewAccounts(vector<string> &cardNumbers, vector<double> &balances);
void showAccountDetails(vector<string> &cardNumbers, vector<string> &userBanks, vector<string> &accountTypes, int i);
//--------------------------------------------------------------------Main-----------------------------------------------------------------

int main()
{
    // vectors
    vector<string> cardNumbers = {"12345678910", "10987654321", "11111111111"};
    vector<string> encodedPINs = {encodeString("6767"), encodeString("9876"), encodeString("6543")};
    vector<double> balances = {9000, 5600, 6700};
    vector<string> userBanks = {"BPI", "BDO", "Metrobank"};
    vector<string> accountTypes = {"Local", "International", "International"};

    string adminPasscode = encodeString("6767");
    while (true)
    {
        int role = login(adminPasscode, cardNumbers, encodedPINs);

        if (role == 1)
            clientMenu(cardNumbers, encodedPINs, balances, userBanks, accountTypes);
        else if (role == 2)
            adminMenu(cardNumbers, encodedPINs, balances, userBanks, accountTypes, adminPasscode);
        else if (role == 3)
            break;
        else
        {
            system("pause");
            return 0;
            break;
        }
    }

    system("pause");
    return 0;
}

//--------------------------------------------------------------------Login function----------------------------------------------

int login(string &adminPasscode, vector<string> &cardNumbers, vector<string> &encodedPINs)
{
    int roleChoice;
    cout << BOLD << CYAN << ITALIC << "======================================================" << endl;
    cout << "        K H G :  B A N K I N G  S Y S T E M           "
         << endl;
    cout << "======================================================" << ITALIC_OFF << endl;
    cout << "Supported Banks: BDO, BPI, Metrobank, Security Bank" << endl;
    cout << "======================================================" << RESET << endl
         << endl;
    cout << BOLD << YELLOW << "\n[1] Client   [2] Admin   [3] Shutdown" << RESET << endl;
    cout << BOLD << BLUE << "Enter your choice: " << RESET;
    cin >> roleChoice;

    clearScreen();

    if (roleChoice == 1)
    {
        return 1;
    }
    else if (roleChoice == 2)
    {

        for (int attempts = 0; attempts < 3; ++attempts)
        {

            string passcode;
            cout << BOLD << BLUE << "Enter admin passcode: " << RESET;
            cin >> passcode;
            clearScreen();

            if (encodeString(passcode) == adminPasscode)
            {
                cout << BOLD << GREEN << "\nAdmin access granted." << RESET;
                return 2; // admin role
            }

            cout << BOLD << RED << "Incorrect passcode (" << attempts + 1 << "/3).\n"
                 << RESET;
        }
    }
    else if (roleChoice == 3)
    {
        cout << BOLD << RED << "System shutting down..." << RESET << endl;
        return 0;
    }
    return 0;
}

//--------------------------------------------------------------------Security functions---------------------------------------------

string encodeString(string plain)
{
    string encoded = plain;
    for (char &c : encoded)
        c += 3; // Simple Caesar cipher
    return encoded;
}

string decodeString(string encoded)
{
    string decoded = encoded;
    for (char &c : decoded)
        c -= 3; // Reverse Caesar cipher
    return decoded;
}

//--------------------------------------------------------------------Utilities---------------------------------------------
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displayDateTime()
{
    time_t now = time(0);
    tm *timeinfo = localtime(&now);

    cout << BOLD << MAGENTA << "Date: " << (timeinfo->tm_mon + 1) << "/"
         << timeinfo->tm_mday << "/"
         << (timeinfo->tm_year + 1900);
    cout << BOLD << MAGENTA << " \nTime: " << timeinfo->tm_hour << ":"
         << timeinfo->tm_min << RESET << endl;
}

//--------------------------------------------------------------------Client Menu---------------------------------------------

void clientMenu(vector<string> &cardNumbers,
                vector<string> &encodedPINs,
                vector<double> &balances,
                vector<string> &userBanks,
                vector<string> &accountTypes)
{
    string userCard;
    cout << BOLD << BLUE << "Enter Card Number: " << RESET;
    cin >> userCard;
    int accountIndex;

    if (validateCardNumber(cardNumbers, userCard, accountIndex))
    {
        string userPIN;
        cout << BOLD << BLUE << "Enter PIN: " << RESET;
        cin >> userPIN;

        clearScreen();

        if (encodeString(userPIN) == encodedPINs[accountIndex])
        {
            displayDateTime();
            showAccountDetails(cardNumbers, userBanks, accountTypes, accountIndex);
            cout << BOLD << ITALIC << FAINT << YELLOW << "\nTransaction fees may apply depending on your account type " << RESET << ITALIC_OFF << endl;
            int choiceUser;

            do
            {
                cout << BOLD << CYAN << "======= CLIENT MENU ========" << RESET << endl;
                cout << BOLD << ITALIC << "[1] Check Balance" << endl;
                cout << "[2] Withdraw Cash" << endl;
                cout << "[3] Transfer Cash" << endl;
                cout << "[4] View Transaction History" << endl;
                cout << "[5] Change PIN" << endl;
                cout << "[6] Log Out" << ITALIC_OFF << endl;
                //-----
                cout << BOLD << BLUE << "\nEnter your choice: " << RESET;
                cin >> choiceUser;

                clearScreen();

                switch (choiceUser)
                {
                case 1: // Check balance
                {
                    displayDateTime();
                    showAccountDetails(cardNumbers, userBanks, accountTypes, accountIndex);
                    cout << BOLD << YELLOW << "Current Balance: Php " << balances[accountIndex] << endl;
                    break;
                }

                case 2: // Withdraw cash
                {
                    displayDateTime();
                    showAccountDetails(cardNumbers, userBanks, accountTypes, accountIndex);

                    int withdrawChoice;
                    cout << BOLD << CYAN << "Select Withdrawal Option:\n"
                         << RESET;
                    cout << ITALIC << "1. Predefined Amount (500, 1000, 2000, 5000, 10000)\n";
                    cout << "2. Custom Amount\n";
                    cout << "3. Exit\n"
                         << RESET;
                    cout << BOLD << BLUE << "Enter choice: " << RESET;
                    cin >> withdrawChoice;
                    cout << "------------------------------------------" << endl;

                    double withdrawAmount = 0;
                    string presetAmount;

                    if (withdrawChoice == 1)
                    {
                        int presetChoice;
                        cout << BOLD << CYAN << "Select Amount:\n"
                             << RESET;
                        cout << "[1] Php 500\n";
                        cout << "[2] Php 1000\n";
                        cout << "[3] Php 2000\n";
                        cout << "[4] Php 5000\n";
                        cout << "[5] Php 10000\n";
                        cout << "Enter Choice: ";
                        cin >> presetChoice;

                        if (presetChoice == 1)
                        {
                            presetAmount = "500";
                        }
                        else if (presetChoice == 2)
                        {
                            presetAmount = "1000";
                        }
                        else if (presetChoice == 3)
                        {
                            presetAmount = "2000";
                        }
                        else if (presetChoice == 4)
                        {
                            presetAmount = "5000";
                        }
                        else if (presetChoice == 5)
                        {
                            presetAmount = "10000";
                        }
                        else
                        {
                            cout << BOLD << RED << "Invalid input.\n"
                                 << RESET;
                            break;
                        }

                        withdrawAmount = stod(presetAmount);
                    }
                    else if (withdrawChoice == 2)
                    {
                        cout << BOLD << CYAN << "Enter amount to withdraw: " << RESET;
                        cin >> withdrawAmount;

                        if ((int)withdrawAmount % 100 != 0)
                        {
                            cout << BOLD << RED << "Amount must be a multiple of 100.\n"
                                 << RESET;
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }

                    // Check withdrawal limit based on user's bank
                    for (int i = 0; i < NUM_BANKS; i++)
                    {
                        if (userBanks[accountIndex] == bankNames[i])
                        {
                            if (withdrawAmount > dailyLimits[i])
                            {
                                cout << BOLD << RED << "Exceeds daily withdrawal limit.\n"
                                     << RESET;
                                break;
                            }
                        }
                    }

                    // Variables to track number of bills dispensed
                    int bills1000 = 0;
                    int bills500 = 0;
                    int bills100 = 0;

                    // Recursive function to calculate optimal bill distribution
                    calculateBills(withdrawAmount, bills1000, bills500, bills100);

                    // Check if ATM has enough bills available
                    if (bills1000 > billCount[2] || bills500 > billCount[1] || bills100 > billCount[0])
                    {
                        cout << BOLD << RED << "ATM does not have enough cash. Withdrawal failed.\n"
                             << RESET;
                        break;
                    }

                    bool success;

                    // Perform withdrawal using overloaded function
                    if (withdrawChoice == 1)
                    {
                        success = withdraw(balances, accountIndex, presetAmount);
                    }
                    else
                    {
                        success = withdraw(balances, accountIndex, withdrawAmount);
                    }

                    if (!success)
                    {
                        cout << BOLD << RED << "Insufficient balance. Withdrawal failed.\n"
                             << RESET;
                        break;
                    }

                    // Update ATM bill count
                    billCount[2] -= bills1000;
                    billCount[1] -= bills500;
                    billCount[0] -= bills100;

                    // Calculate transaction fee using recursive function
                    double balanceAfterDeduction = calculateFeeRecursive(balances, userBanks[accountIndex], accountTypes[accountIndex], accountIndex, 0);
                    double deductedFee = balances[accountIndex] - balanceAfterDeduction;

                    balances[accountIndex] = balanceAfterDeduction;

                    clearScreen();
                    cout << BOLD << GREEN << "\nWithdrawal Successful!\n"
                         << RESET;
                    cout << BOLD << CYAN << "\n==================RECEIPT===================" << RESET << endl;
                    cout << BOLD << YELLOW << "Transaction fee deducted: Php " << deductedFee << RESET << endl;
                    cout << BOLD << CYAN << "Updated Balance: Php " << balances[accountIndex] << RESET << endl;

                    // Display withdrawal receipt
                    cout << BOLD << YELLOW << "\nWithdrawn: Php " << withdrawAmount << endl;
                    cout << "Transaction Fee: Php " << deductedFee << RESET << endl;

                    // Bill distribution
                    cout << BOLD << CYAN << "\nBills Dispensed:\n"
                         << RESET;
                    cout << "1000 x " << bills1000 << endl;
                    cout << "500 x " << bills500 << endl;
                    cout << "100 x " << bills100 << endl;
                    cout << endl;

                    // Record transaction
                    logTransaction(cardNumbers[accountIndex], "Withdrawal", withdrawAmount, deductedFee);
                }
                break;

                case 3: // Transfer cash
                {
                    displayDateTime();
                    string recipientCard;
                    double transferAmount;

                    cout << BOLD << CYAN << "Enter recipient card number: " << RESET;
                    cin >> recipientCard;

                    int recIndex;
                    if (!validateCardNumber(cardNumbers, recipientCard, recIndex))
                    {
                        cout << BOLD << RED << "Recipient card not found.\n"
                             << RESET;
                        break;
                    }
                    cout << BOLD << BLUE << "Enter amount to transfer: " << RESET;
                    cin >> transferAmount;

                    if (balances[accountIndex] >= transferAmount) // Check if balance is sufficient for the transfer amount
                    {
                        balances[accountIndex] -= transferAmount; // Deduct from sender
                        balances[recIndex] += transferAmount;     // Add to recipient

                        logTransaction(cardNumbers[accountIndex], "Transfer Sent", transferAmount, 0);
                        logTransaction(cardNumbers[recIndex], "Transfer Received", transferAmount, 0);
                        cout << BOLD << GREEN << "Transfer successful! Php " << transferAmount << " sent to card " << recipientCard << RESET << endl;
                    }
                    else
                    {
                        cout << BOLD << RED << "Insufficient balance for transfer.\n"
                             << RESET;
                    }
                }
                break;

                case 4: // View transaction history
                {
                    displayDateTime();

                    cout << BOLD << CYAN << "\n====== Last Transactions ======\n"
                         << RESET;

                    int count = 0;

                    // start from latest transaction
                    for (int i = transactionTypes.size() - 1; i >= 0 && count < 10; i--)
                    {
                        if (transactionCards[i] == cardNumbers[accountIndex])
                        {
                            cout << BOLD << YELLOW
                                 << "Type: " << transactionTypes[i]
                                 << " | Amount: Php " << transactionAmounts[i]
                                 << " | Fee: Php " << transactionFees[i]
                                 << " | Date: " << transactionDates[i]
                                 << " | Time: " << transactionTimes[i]
                                 << RESET << endl;
                            cout << endl;

                            count++;
                        }
                    }

                    if (count == 0)
                    {
                        cout << RED << "No transactions found.\n"
                             << RESET;
                    }
                }
                break;

                case 5: // Change PIN
                {
                    displayDateTime();
                    string newpin;

                    cout << BOLD << BLUE << "Enter current PIN: " << RESET;
                    cin >> userPIN;

                    if (encodeString(userPIN) != encodedPINs[accountIndex])
                    {
                        cout << BOLD << RED << "Incorrect current PIN.\n"
                             << RESET;
                        break;
                    }

                    cout << BOLD << CYAN << "Enter new PIN: " << RESET;
                    cin >> newpin;
                    encodedPINs[accountIndex] = encodeString(newpin);
                    cout << BOLD << GREEN << "PIN changed successfully!\n"
                         << RESET;
                    break;
                }
                case 6: // Log out
                    cout << BOLD << CYAN << "Logging out safely...\n"
                         << RESET;
                    break;
                }

            } while (choiceUser != 6);
        }

        else
        {
            cout << BOLD << RED << "Incorrect Passcode. Please Try Again.\n"
                 << RESET;
            return;
        }
    }
    else
    {
        cout << BOLD << RED << "Card number not found. Please try again.\n"
             << RESET;
        return;
    }
}
// show account details function to display bank, account type, and balance
void showAccountDetails(vector<string> &cardNumbers, vector<string> &userBanks, vector<string> &accountTypes, int i)
{
    cout << BOLD << UNDERLINE << YELLOW << "Bank: " << userBanks[i] << endl;
    cout << "Account Type: " << accountTypes[i] << RESET << UNDERLINE_OFF << endl;
}
// validateCardNumber function to check if the entered card number exists and return its index
bool validateCardNumber(vector<string> &cardNumbers,
                        string userCard,
                        int &accountIndex)
{
    for (int i = 0; i < cardNumbers.size(); i++)
    {
        if (cardNumbers[i] == userCard)
        {
            accountIndex = i;
            return true;
        }
    }
    return false;
}
// calculateBills function to determine optimal bill count for a given withdrawal amount
void calculateBills(double withdrawAmount, int &bills1000, int &bills500, int &bills100)
{
    if (withdrawAmount >= 1000)
    {
        bills1000++;
        calculateBills(withdrawAmount - 1000, bills1000, bills500, bills100);
    }
    else if (withdrawAmount >= 500)
    {
        bills500++;
        calculateBills(withdrawAmount - 500, bills1000, bills500, bills100);
    }
    else if (withdrawAmount >= 100)
    {
        bills100++;
        calculateBills(withdrawAmount - 100, bills1000, bills500, bills100);
    }
}
// logtransaction function to store transaction details in vectors (for in-memory storage, can be extended to file storage)
void logTransaction(const string &cardNum, const string &type,
                    double amount, double fee)
{
    time_t now = time(0);
    tm *timeinfo = localtime(&now);

    string date =
        to_string(timeinfo->tm_mon + 1) + "/" +
        to_string(timeinfo->tm_mday) + "/" +
        to_string(timeinfo->tm_year + 1900);

    string time =
        to_string(timeinfo->tm_hour) + ":" +
        to_string(timeinfo->tm_min);

    // store in vectors (This is for in-memory storage, you can also write to a file if needed)
    transactionCards.push_back(cardNum);
    transactionTypes.push_back(type);
    transactionAmounts.push_back(amount);
    transactionFees.push_back(fee);
    transactionDates.push_back(date);
    transactionTimes.push_back(time);

    // transaction logging to CSV file
    ofstream file("transactions.csv", ios::app);

    if (file.is_open())
    {
        file << date << ","
             << time << ","
             << cardNum << ","
             << type << ","
             << amount << ","
             << fee << endl;

        file.close();
    }
    else
    {
        cerr << "Error: Could not open transactions.csv\n";
    }
}
// Version 1: Custom amount withdrawal
bool withdraw(vector<double> &balances, int accountIndex, double withdrawAmount)
{
    // Process withdrawal
    if (balances[accountIndex] >= withdrawAmount) // Check if balance is sufficient for the withdrawal amount
    {
        balances[accountIndex] -= withdrawAmount;
        return true;
    }
    return false;
}

// Version 2: Preset amount
bool withdraw(vector<double> &balances, int accountIndex, string presetAmount)
{
    double withdrawAmount = stod(presetAmount);              // Convert string to double
    return withdraw(balances, accountIndex, withdrawAmount); // Call Version 1
}
// Recursive fee calculation based on bank and account type
double calculateFeeRecursive(vector<double> &balances, string currentUserBank, string userAccountType, int accountIndex, int iterations)
{
    // Base case: end of bank list
    if (iterations == NUM_BANKS)
    {
        return balances[accountIndex];
    }
    // If bank matches
    if (currentUserBank == bankNames[iterations])
    {
        if (userAccountType == "Local")
        {
            return balances[accountIndex] - localFees[iterations];
        }
        else
        {
            return balances[accountIndex] - intlFees[iterations];
        }
    }
    // Recursive call: to the next bank
    return calculateFeeRecursive(balances, currentUserBank, userAccountType, accountIndex, iterations + 1);
};

//--------------------------------------------------------------------Admin Menu-------------------------------------------------------------

void adminMenu(vector<string> &cardNumbers,
               vector<string> &encodedPINs,
               vector<double> &balances,
               vector<string> &userBanks,
               vector<string> &accountTypes,
               string &adminPasscode)
{
    int choiceAdmin;

    do
    {

        cout << BOLD << CYAN << "\n=========Admin Menu:========== " << RESET << endl;
        cout << ITALIC << "[1] View Current Cash" << endl;
        cout << "[2] Refill Cash" << endl;
        cout << "[3] Create Account" << endl;
        cout << "[4] View accounts" << endl;
        cout << "[5] Delete Account" << endl;
        cout << "[6] Reset account passwords" << endl;
        cout << "[7] Change admin Passcode" << endl;
        cout << "[8] List of all admin users" << endl;
        cout << "[9] Log out" << ITALIC_OFF << endl;

        cout << BOLD << BLUE << "\nEnter your choice: " << RESET;
        cin >> choiceAdmin;

        clearScreen();

        switch (choiceAdmin)
        {
        case 1: // View current cash
        {
            cout << BOLD << CYAN << UNDERLINE << "Current cash in machine:\n"
                 << RESET << UNDERLINE_OFF;
            int totalCash = 0;
            for (int i = 0; i < NUM_DENOMINATIONS; i++)
            {
                totalCash += denominations[i] * billCount[i];
                cout << BOLD << YELLOW << "PHP " << denominations[i] << ": " << billCount[i] << " bills" << RESET << endl;
            }
            cout << BOLD << YELLOW << UNDERLINE << "Total cash available: PHP " << totalCash << RESET << UNDERLINE_OFF << endl;
            break;
        }
        break;
        case 2: // Refill cash
        {
            refillCash(denominations, billCount);
            break;
        }

        break;
        case 3: // Create account
        {
            string card, pin, bank, type;
            double bal;

            cout << BOLD << BLUE << "Card: " << RESET;
            cin >> card;

            cout << BOLD << BLUE << "PIN: " << RESET;
            cin >> pin;

            cout << BOLD << BLUE << "Initial Balance: " << RESET;
            cin >> bal;

            cout << BOLD << BLUE << "Bank (BDO, BPI, Metrobank, Security Bank): " << RESET;
            cin >> bank;

            cout << BOLD << BLUE << "Type (Local/International): " << RESET;
            cin >> type;

            cardNumbers.push_back(card);
            encodedPINs.push_back(encodeString(pin));
            balances.push_back(bal);
            userBanks.push_back(bank);
            accountTypes.push_back(type);

            cout << BOLD << GREEN << "Account created!\n"
                 << RESET;
        }
        break;
        case 4: // View accounts
        {
            viewAccounts(cardNumbers, balances);
        }
        break;
        case 5: // Delete account
        {
            viewAccounts(cardNumbers, balances);
            int index;
            cout << BOLD << BLUE << "Enter index to delete: " << RESET;
            cin >> index;

            cardNumbers.erase(cardNumbers.begin() + index);
            encodedPINs.erase(encodedPINs.begin() + index);
            balances.erase(balances.begin() + index);
            userBanks.erase(userBanks.begin() + index);
            accountTypes.erase(accountTypes.begin() + index);

            cout << BOLD << GREEN << "Index: " << index << " Account Succesfully deleted! \n"
                 << RESET;
        }
        break;
        case 6: // Reset account passwords
        {
            viewAccounts(cardNumbers, balances);
            int index;
            cout << BOLD << BLUE << "Enter index to reset PIN: " << RESET;
            cin >> index;

            string newPIN;
            cout << BOLD << BLUE << "Enter new PIN: " << RESET;
            cin >> newPIN;

            encodedPINs[index] = encodeString(newPIN);
            cout << BOLD << GREEN << "PIN reset successful!\n"
                 << RESET;
        }
        break;
        case 7: //  Change admin passcode
        {
            string currentPasscode;
            cout << BOLD << BLUE << "Enter current passcode: " << RESET;
            cin >> currentPasscode;
            if (encodeString(currentPasscode) == adminPasscode)
            {
                string newPasscode;
                cout << BOLD << BLUE << "Enter new passcode: " << RESET;
                cin >> newPasscode;
                adminPasscode = encodeString(newPasscode);
                cout << BOLD << GREEN << "Admin passcode updated successfully!\n"
                     << RESET;
            }
            else
            {
                cout << BOLD << RED << "Incorrect current passcode.\n"
                     << RESET;
            }
        }
        break;
        case 8: // List of all admin users - Since there is only one admin, we will just show the admin passcode as the "admin user"
        {
            cout << BOLD << BLUE << "Admin Passcode: " << RESET << adminPasscode << endl;
            break;
        }
        case 9: // Exit
            cout << BOLD << YELLOW << "Exiting Admin Menu..." << RESET << endl;

            break;
        default:
            cout << BOLD << RED << "Invalid input.\n"
                 << RESET;
        }

    } while (choiceAdmin != 9);
}
// Admin menu options:
// 1. refill cash - allows admin to add more bills to the machine (only allowed between 8:00 and 8:15)
void refillCash(int denom[], int billCount[])
{
    time_t now = time(0);
    tm *localTime = localtime(&now);

    // Check if current time is between 8:00 and 8:15
    if (localTime->tm_hour == 8 && localTime->tm_min <= 15)
    {
        // Refill bills
        int refillAmount;
        for (int i = 0; i < NUM_DENOMINATIONS; i++)
        {
            cout << BOLD << BLUE << "Enter number of PHP " << denom[i] << " bills to add: " << RESET;
            cin >> refillAmount;
            // Update bill count
            billCount[i] += refillAmount;
        }
        cout << BOLD << GREEN << "Refill successful." << RESET << endl;
    }
    else
    {
        cout << BOLD << RED << "Refill allowed only from 8:00 AM to 8:15 AM." << RESET << endl;
    }
}
// 2. view accounts - shows list of accounts with their card numbers and balances
void viewAccounts(vector<string> &cardNumbers, vector<double> &balances)
{
    for (int i = 0; i < cardNumbers.size(); i++)
    {
        cout << BOLD << BLUE << i << " " << cardNumbers[i] << " Balance: " << balances[i] << RESET << endl;
    }
}
