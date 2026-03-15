#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

//--------------------------------------------------------------------Main functions
const int NUM_BANKS = 4;
string bankNames[NUM_BANKS] = {"BDO", "BPI", "Metrobank", "Security Bank"};
double localFees[NUM_BANKS] = {25, 20, 30, 15};
double intlFees[NUM_BANKS] = {150, 125, 200, 100};
double dailyLimits[NUM_BANKS] = {50000, 75000, 100000, 60000};

const int NUM_DENOMINATIONS = 3;
int denominations[NUM_DENOMINATIONS] = {100, 500, 1000};
int billCount[NUM_DENOMINATIONS] = {500, 500, 500}; // Current count of each

int login(string &adminPasscode)
{
	int roleChoice;
	cout << "\n==========ATM System!==========" << endl;
	cout << "\n[1] Client   [2] Admin   [3] Shutdown" << endl;
	cout << "Enter your choice: ";
	cin >> roleChoice;

	if (roleChoice == 1)
	{
		return 1;
	}

	else if (roleChoice == 2)
	{
		for (int attempts = 0; attempts < 3; ++attempts)
		{
			string passcode;
			cout << "Enter admin passcode: ";
			cin >> passcode;

			if (passcode == adminPasscode)
			{
				cout << "Admin access granted." << endl;
				return 2; // admin role
			}

			cout << "Incorrect passcode (" << attempts + 1 << "/3).\n";
		}
	}
	cout << "Access denied. System Locked." << endl;
	return 3;
}

string encodeString(string plain)
{
	string encoded = plain;
	for (char &c : encoded)
	{
		c += 3; // Simple Caesar cipher
	}
	return encoded;
};

string decodeString(string encoded)
{
	string decoded = encoded;
	for (char &c : decoded)
	{
		c -= 3; // Reverse the Caesar cipher
	}
	return decoded;
};

bool validateCardNumber(string card)
{
	// Implementation for validating card number
	return false;
}

double calculateFeeRecursive(double amount, int iterations)
{
	// Implementation for calculating fee recursively
	return 0.0;
};

void logTransaction(const string &cardNum,
					const string &type,
					double amount) {

};

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

	cout << "Date: " << (timeinfo->tm_mon + 1) << "/"
		 << timeinfo->tm_mday << "/"
		 << (timeinfo->tm_year + 1900);
	cout << " Time: " << timeinfo->tm_hour << ":"
		 << timeinfo->tm_min << endl;
}

// Display helper (const reference - read-only)
void displayMenu(const vector<string> &bankNames,
				 const vector<double> &localFees) {

};

void clientMenu(vector<string> &cardNumbers,
				vector<double> &balances,
				vector<string> &encodedPINs,
				vector<string> &userBanks,
				vector<string> &accountTypes
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

			if (userPin == encodedPINs[i])
			{
				clearScreen();

				cout << "WELCOME!\n";
				cout << "\nYour card has been identified as:\n";
				cout << "Bank: " << userBanks[i] << endl;
				cout << "Account Type: " << accountTypes[i] << endl;
				cout << "\nTransaction fees may apply depending on your account type.\n";
				cout << "Please proceed.\n";

				int accountIndex = i;
				int choiceUser;
				char userDecision;

				do
				{
					cout << "\n==== CLIENT MENU ====" << endl;
					cout << "1. Check Balance" << endl;
					cout << "2. Withdraw Cash" << endl;
					cout << "3. Transfer Cash" << endl;
					cout << "4. View Transaction History" << endl;
					cout << "5. Change PIN" << endl;
					cout << "6. Exit" << endl;
					//-----
					cout << "\nEnter your choice: ";
					cin >> choiceUser;

					clearScreen();

					if (choiceUser == 1)
					{
						// Display balance
						displayDateTime();
						cout << "Current Balance: Php " << balances[accountIndex] << endl;
					}
					else if (choiceUser == 2)
					{
						// Withdraw cash
						int withdrawAmount;
						cout << "Enter amount: ";
						cin >> withdrawAmount;

						// Deduct appropriate fee
						if (accountTypes[accountIndex] == "Local")
						{
							for (int i = 0; i < NUM_BANKS; i++)
							{
								if (userBanks[accountIndex] == bankNames[i])
								{
									balances[accountIndex] -= localFees[i];
								}
							}
						}
						else
						{
							for (int i = 0; i < NUM_BANKS; i++)
							{
								if (userBanks[accountIndex] == bankNames[i])
								{
									balances[accountIndex] -= intlFees[i];
								}
							}
						}
						balances[accountIndex] -= withdrawAmount;
					}

					else if (choiceUser == 3)
					{
						// NOT YET DONE
						//  Transfer cash
						string recipientCard;
						cout << "Enter recipient card number: ";
						cin >> recipientCard;
					}
					else if (choiceUser == 4)
					{
						// View transaction history
					}
					else if (choiceUser == 5)
					{
						// Change PIN
						string verifPIN;
						cout << "Re-enter PIN: ";
						cin >> verifPIN;

						if (verifPIN == encodedPINs[accountIndex])
						{
							// INCOMPLETE: NO ENCODING AND VALIDATE YET
							clearScreen();
							string newPIN;
							cout << "Enter new PIN: ";
							cin >> newPIN;
							encodedPINs[accountIndex] = newPIN;
						}
						else
						{
							cout << "Incorrect PIN. Please try again.";
						}
					}
					else if (choiceUser == 6)
					{
						break;
					}

					else
					{
						cout << "Invalid input.\n";
					}

					cout << "\nWould you like to perform another transaction? (Y/N): ";
					cin >> userDecision;

					clearScreen();

				} while (userDecision == 'Y' || userDecision == 'y');
			}

			else
			{
				cout << "Incorrect Pin. Please try again.\n";
				cout << endl;
			}
		}

		else
		{
			break;
		}
	}
};

void adminMenu(vector<string> &cardNumbers,
			   vector<double> &balances,
			   vector<string> &encodedPINs,
			   string &adminPasscode,
			   vector<string> &userBanks,
			   vector<string> &accountTypes)
{
	int choiceAdmin;
	cout << "Admin Menu: " << endl;
	cout << "1. Cash Management" << endl;
	cout << "2. User Management" << endl;
	cout << "3. Admin Credentials" << endl;
	cout << "4. Exit" << endl;
	cout << "\nEnter your choice: ";
	cin >> choiceAdmin;
	if (choiceAdmin == 1)
	{
		int ChoiceCashManagement;
		cout << "Cash Management: " << endl;
		cout << "1. View current cash" << endl;
		cout << "2. Replenish cash" << endl;
		cout << "3. Track total cash" << endl;
		cout << "4. Exit" << endl;
		cout << "\nEnter choice: ";
		cin >> ChoiceCashManagement;
		// Cash management
	}
	else if (choiceAdmin == 2)
	{
		int ChoiceUserManagement;
		cout << "User Management: " << endl;
		cout << "1. Add new account" << endl;
		cout << "2. View all accounts and balances" << endl;
		cout << "3. delete/ deactivate account" << endl;
		cout << "4. Reset account passwords" << endl;
		cout << "5. Exit" << endl;
		cout << "\nEnter choice: ";
		cin >> ChoiceUserManagement;
		// User management
		if (ChoiceUserManagement == 1)
		{
			for (size_t i = 0; i < cardNumbers.size(); ++i)
			{
				// this is just for checking purposes to see if the new account is added to the vectors
				cout << "Card: " << cardNumbers[i]
					 << ", Bank: " << userBanks[i]
					 << ", Type: " << accountTypes[i]
					 << ", Balance: " << balances[i] << endl;
			}
			// View all accounts and balances
			// IT'S GONNA LOOP INFINITELY BECAUSE NOTHING IS INSIDE THE IF STATEMENT YET!!!!!!!!!!
		}
		else if (ChoiceUserManagement == 2)
		{
			for (int i = 0; i < cardNumbers.size(); ++i)
			{
				cout << cardNumbers[i] << " - " << userBanks[i] << ", " << accountTypes[i] << ": " << balances[i] << endl;
			}
			// Add new accountX`
		}
		else if (ChoiceUserManagement == 3)
		{
			// delete/ deactivate account
		}
		else if (ChoiceUserManagement == 4)
		{
			// Reset account passwords
		}
	}
	else if (choiceAdmin == 3)
	{

		// Admin credentials
		int ChoiceAdminCredentials;
		cout << "Admin Credentials: " << endl;
		cout << "1. Change Admin passcode" << endl;
		cout << "2. View all Admin users" << endl;
		cout << "3. Exit" << endl;
		cout << "\nEnter choice: ";
		cin >> ChoiceAdminCredentials;
	}
};

//--------------------------------------------------------------------Utility Functions

bool validateCardNumber(string card);
double calculateFeeRecursive(double amount, int iterations);
void logTransaction(const string &cardNum,
					const string &type,
					double amount);

int main()
{
	vector<string> cardNumbers = {"12345678910", "10987654321", "11111111111"};
	// Parallel vectors - keep in sync!

	vector<string> encodedPINs = {"6767", "9876", "6543"};
	// Encoded passwords

	vector<double> balances = {9000, 5600, 6700};
	// Account balances

	vector<string> userBanks = {"BPI", "BDO", "Metrobank"};
	// Which bank

	vector<string> accountTypes = {"Local", "International", "International"};
	// "Local" or "International"

	// For each account's transaction history
	vector<string> transactionTypes;

	// "Withdrawal", "Deposit", "Transfer"
	vector<double> transactionAmounts;
	vector<double> transactionFees;
	vector<int> transactionQuantities;

	string adminPasscode = "6767"; // Default password

	while (true)
	{
		int role = login(adminPasscode);
		if (role == 1)
			clientMenu(cardNumbers, balances, encodedPINs, userBanks, accountTypes);
		else if (role == 2)
			adminMenu(cardNumbers, balances, encodedPINs, adminPasscode, userBanks, accountTypes);
		else if (role == 3)
		{
			break; // shutdown
		}
		else
		{
			cout << "Access Denied.\n";
			break;
		}
	}
	return 0;
}
