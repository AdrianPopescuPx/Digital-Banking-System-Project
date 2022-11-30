#include <bits/stdc++.h>
using namespace std;

#define DATABASE_LINK "database-users.txt"
mt19937 rng((unsigned int) chrono::steady_clock::now().time_since_epoch().count());
struct date {
    int day;
    int month;
    int year;
};

struct card {
    string type;
    string currency;
    string number;
    string cvv;
    date expire_date;
    int pin;
    double ballance;
    double interest_rate;
};

class Account {
    public:
        int number_of_cards;
        string iban;
        vector<card> cards;
};

class User {
    public:
        string name;
        string unique_serial_number;
        date birth_date; 
        Account account;
};

class Database{
    public:
        int number_of_users;
        vector<User> users;
};

void read_date(date &date, ifstream &read);

void read_card_data(card &card, ifstream &read) {
    read >> card.type;
    read >> card.currency;
    read >> card.number;
    read >> card.cvv;
    read_date(card.expire_date, read);
    read >> card.pin;
    read >> card.ballance;
    read >> card.interest_rate;
}

void read_account_data(Account &account, ifstream &read) {
    read >> account.number_of_cards;
    read >> account.iban;
    for(int i = 1; i <= account.number_of_cards; ++i) {
        card current_card;
        read_card_data(current_card, read);
        account.cards.push_back(current_card);
    }
}

void read_date(date &date, ifstream &read) {
    read >> date.day >> date.month >> date.year;
}

void read_user_data(User &user, ifstream &read) {
    getline(read, user.name);
    if (user.name == "") {
        getline(read, user.name);
    }
    read >> user.unique_serial_number;
    read_date(user.birth_date, read);
    read_account_data(user.account, read);
}   

void read_existing_database(Database &database) {
    ifstream read (DATABASE_LINK);
    read >> database.number_of_users;
    for (int i = 1; i <= database.number_of_users; ++i) {
        User current_user;
        read_user_data(current_user, read);
        database.users.push_back(current_user);
    }
}

void display_welcome_message() {
    cout << "###########################################\n";
    cout << "#### Welcome to the International Bank ####\n";
    cout << "###########################################\n";
    cout << "\n\n";
}

void display_options() {
    cout << "Please select one of the following options:\n\n";
    cout << "1) Register an account\n";
    cout << "2) Log in with card details\n\n";

    cout << "Please type a number from 1 to 2: ";
}

void withdraw_money(Account &account, int current_card) {
cout << "Current Ballance in "<< account.cards[current_card].currency<<" is: " << account.cards[current_card].ballance << '\n';
cout << "Please select the amount of money you want to withdraw: ";
int amount;
cin >> amount;
account.cards[current_card].ballance -= amount; // de proba cu amount
cout << "\nSuccesfull!\n" << "Your new ballance is: " << (account.cards[current_card].ballance)<< "\n\n";
}

void add_money(Account &account, int current_card) {
    cout << "Current Ballance in "<< account.cards[current_card].currency<<" is: " << account.cards[current_card].ballance << '\n';
    cout << "Please enter the amount of money you want to add: ";
    int amount;
    cin >> amount;
    account.cards[current_card].ballance += amount;
    cout << "\nSuccesfull!\n" << "Your new ballance is: " << account.cards[current_card].ballance << "\n\n";
}
// 
void make_loan(Account &account, int current_card) {
    double loan_amount, monthly_payment, years, amount_paid_back, monthly_interest, number_of_payments;

    cout << "Enter the loan amount (LoanAmount) --> ";
    cin >> loan_amount;
    cout << "The International Bank has a fixed interest rate of 5% \n";
    cout << "Enter the period of time for the loan: \n";
    cin >> years;
    number_of_payments = years * 12;
    monthly_interest = (5 / 100.0) / 12;
    monthly_payment = monthly_interest * pow (( 1 + monthly_interest ), number_of_payments) / (pow(( 1 + monthly_interest), number_of_payments) -1)  * loan_amount;
    cout << "Monthly Payment:  " << monthly_payment << endl;
    amount_paid_back = monthly_payment * number_of_payments;
    cout << "Amount Paid Back: " << amount_paid_back << endl;
    cout << "Interest Paid:  " << (amount_paid_back - loan_amount) << endl;

    cout << "Do you want to proceed further with the Load? (Yes / No) ";
    string decision;
    cin >> decision;
    if(decision == "Yes" || decision == "yes" || decision == "YES") {
        account.cards[current_card].ballance += loan_amount;
        int second_card;
        if(current_card == 1)
            second_card = 0;
        else 
            second_card = 1;
        if(account.cards[second_card].type == "credit") {
            if(account.cards[second_card].currency == "euro") {
                account.cards[second_card].ballance *= 4.9;
                account.cards[current_card].ballance -= account.cards[second_card].ballance;
                account.cards[second_card].ballance = 0;
            }
            else if(account.cards[second_card].currency == "ron") {
                account.cards[second_card].ballance /= 4.9;
                account.cards[current_card].ballance -= account.cards[second_card].ballance;
                account.cards[current_card].ballance -= account.cards[second_card].ballance;
                account.cards[second_card].ballance = 0;
            }
        }
    }
}


void economies_account_calculator(Account &account, int current_card) {
    cout << "Step 1: Initial Investment:\n";
    cout << "Your current ballance: " << account.cards[current_card].ballance << '\n'; 
    cout << "Amount of money that you have available to invest initially: ";
    int starting_amount;
    cin >> starting_amount;
    cout << "Length of time, in years, that you plan to save ";
    int years;
    cin >> years;
    cout << "Your interest rate for this account is: " << account.cards[current_card].interest_rate << '\n'; 
    cout << "In " << years << " years you will have: " << starting_amount * pow((1 + (account.cards[current_card].interest_rate / 100)), years) << '\n';
}

void manage_account(Account &account, int current_card) {
    int option;
    cout << "Please select The operation you want to make:\n";
    cout << "1) Withdraw Money\n" << "2) Add Money to your balance\n" << "3) Economies account calculator\n" << "4) Make a loan\n\n";
    cin >> option;
    if(option != 1 && option != 2 && option != 3 && option != 4) {
        while(option != 1 && option != 2 && option != 3 && option != 4) {
            cout << "Wrong Selection , please select between 1-3\n";
            cin >> option;
        }
    }
    if(option == 1)
        withdraw_money(account, current_card);
    else if(option == 2) 
        add_money(account, current_card);
    else if(option == 3)
        economies_account_calculator(account, current_card);
    else 
        make_loan(account, current_card);
}


void login_user(Database &database) {
    bool verified = false;
    while(verified == false) {
        cout << "Please enter your card number:\n";
        string number;
        cin >> number;
        cout << "Please enter your Pin:\n";
        int entered_pin;
        cin >> entered_pin;
        for(int i = 0; i < database.number_of_users; ++i) {
        if(verified)
            break;
        for(int j = 0; j < database.users[i].account.number_of_cards; ++j) {
            if(database.users[i].account.cards[j].number.find(number) != string::npos && database.users[i].account.cards[j].pin == entered_pin) {
                verified = true;
                cout << "##########   WELCOME BACK   ##########\n";
                cout << "##########  " << database.users[i].name << "  ##########\n\n";
                int current_card = j;
                manage_account(database.users[i].account, current_card);
                break;
                }
            }
        }
        if(verified == false)
            cout << "Details Incorrect, please try again!\n";
    }
}
void new_user_date(date &date);

void read_new_card_data(card &card, int n) {
    cout << "Card Number";
    cout << n << '\n';
    cout << "Debit or Credit Card?\n";
    cin >> card.type;
    cout << "Please select your desired currency\n";
    cin >> card.currency;
    cout << "Your card number is:\n";
    card.number = to_string(rng() % 10000000000 + 1000000000);
    card.ballance = 0;
    card.cvv = to_string(rng() % 900 + 100);
    card.pin = rng() % 9000 + 1000;
    card.interest_rate = rng() % 900 + 100, card.interest_rate /= 100;
    cout << "Your new card has been generated with the followind details:\n";
    cout << "Card Number: " << card.number <<'\n';
    cout << "Cvv: " << card.cvv << '\n';
    cout << "Pin: " << card.pin << '\n';
    cout << "Card Interest Rate: " << card.interest_rate << '\n';
}

void new_user_account(Account &account) {
    cout << "How many cards do you want?\n";
    cout << "Be aware: You can create a maximum of 2 Cards for your account type";
    cin >> account.number_of_cards;
    cout << "Your account IBAN is:\n";
    account.iban = "RORNCB" + to_string(rng() % 100000000000 + 1000000000);
    cout << account.iban << '\n';
    for(int i = 1; i <= account.number_of_cards; ++i) {
        card current_card;
        read_new_card_data(current_card, i);
        account.cards.push_back(current_card);
    }
}

void new_user_date(date &date) {
    cout << "Please type your Full Birthday\n";
    cout << "Note that in order to enter a correct Birthday you must type it as follows: XX - day || XX - month || XXXX - year\n";
    bool correct_date = false;
    while(correct_date == false) {
        cin >> date.day >> date.month >> date.year;
        if(date.day <= 31 && date.month <= 12 && date.year <= 2004)
            correct_date = true;
        else cout << "Incorrect Format, try again! \n";
    }
}

void read_new_user_data(User &user) {
    cout << "Please enter your Unique Serial Number:   ";
    cin >> user.unique_serial_number;
    new_user_date(user.birth_date);
}

void register_user(Database &database) {

    User user;
    cout << "Welcome to the registering page!\n\n";
    cout << "Enter your full name: ";
    
    string name;
    cin.get();
    getline(cin, name);
    user.name = name;
    read_new_user_data(user);
    new_user_account(user.account);
    database.number_of_users++;
    database.users.push_back(user);
}

void write_card(card &card, ofstream &write) {

    write << card.type << '\n';
    write << card.currency << '\n';
    write << card.number << '\n';
    write << card.cvv << '\n';
    write << card.expire_date.day << " " << card.expire_date.month << " " << card.expire_date.year << '\n';
    write << card.pin << '\n';
    write << card.ballance << '\n';
    write << card.interest_rate << '\n';

}

void write_account(Account &account, ofstream &write) {
    write << account.number_of_cards << '\n';
    write << account.iban << '\n';
    for (auto card : account.cards) {
        write_card(card, write);
    }
}

void write_user(User &user, ofstream &write) {
    write << user.name << '\n';
    write << user.unique_serial_number << '\n';
    write << user.birth_date.day << " " << user.birth_date.month << " " << user.birth_date.year << '\n';
    write_account(user.account, write);
}

void update_database(Database &database) {

    ofstream write(DATABASE_LINK);
    write << database.number_of_users << '\n';

    for (auto user : database.users) {
        write_user(user, write);
    }
}

int main() {

    Database *database = new Database();
    read_existing_database(*database);
    display_welcome_message();
    while (true) {

        display_options();
        int option;
        cin >> option;
        if (option != 1 && option != 2) {
            cout << "Error, command unknown\n";
            continue;
        }
        else {
            cout << "You selected option number\n" << option << '\n';
            if (option == 1) {
                register_user(*database);
            }
            else if(option == 2){
                login_user(*database);
            }
            cout << "Please type YES or NO if you want to make another operation: \n";
            string checker;
            cin >> checker;
            if(checker == "NO" || checker == "No" || checker == "no")
                break;
        }
    }

    update_database(*database);


    return 0;
}