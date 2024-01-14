#include <iostream>
#include <ctime>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;
const int MAX_PLAYERS = 6;

int userReply(string s); //done

//Here is a class with all the funcations inside
class Game {
public:
	Game();

	void shuffleDeck(); //done
	void printDeck(); //done
	void printCard(int card); //done
	void printHand(int hand[]); //done
	void playGame(int, bool); //done
	int player(int deck[], int p_hand[], int next_card); //done
	int dealerTurn(int deck[], int d_hand[], int next_card); //done
	int sumResult(int hand[]); //done 
	void Winner(int p_sum[], int d_sum, int n, int score[]);//done
	int isBusted(int sum); //done
	void resetHand(int hand[]); //done
	void displayScore(int score[], int n);//done
	~Game(); // Destructor
private:
	int deck[52], finalscore[MAX_PLAYERS];
	int playersnum;
	bool newGame;
};

//In the int main() function player diside if he/she wants to play the game.
//And also it use a file to put the amount of players inside of it.
int main() {

	int Time = 1, reply = 0;
	char answer;
	int playersnum{};
	Game theGame;
	fstream myFile;

	cout << "------------------" << endl;
	cout << "| A card game of 31 |" << endl;
	cout << "------------------" << endl << endl;

	myFile.open("data.txt", ios::in);
	if (myFile.is_open())
	{
		reply = userReply("Would you like to continue last game? ");
		//	cout << "Press '1' to continue last game or '0' to start a new one: ";
		//	cin >> reply;
		if (reply == 0)
			myFile >> playersnum;
		myFile.close();
	}
	else
		reply = 1;

	if (reply == 1)
	{
		do {
			if (Time)
			{
				cout << "Number of players? ";
				Time = 0;
			}
			else
				cout << "Valid number of player are between 2 and 6: ";
			cin >> playersnum;
		} while (playersnum < 2 || playersnum>MAX_PLAYERS);
		cout << endl;
	}


	do {
		do {
			cout << endl;
			cout << "Do you want to play (y/n)? ";
			cin >> answer;
			answer = tolower(answer);
		} while (answer != 'y' && answer != 'n');

		if (answer == 'y')
		{
			cout << "\nShufled Deck:\n";
			if (reply == 0)
				theGame.playGame(playersnum, false);
			else
				theGame.playGame(playersnum, true);
		}

	} while (answer != 'n');
	return 0;
}

//This function asks player if to start the game or not.
int userReply(string s)
{
	char answer;
	do
	{
		cout << s << "(0-Yes"  "1-No): ";
		while (answer = cin.get(), answer != '1' && answer != '0');
	} while (answer != '1' && answer != '0');
	return answer == '1';
}

//This part here told us that they are gonna be only 52 playing cards.
Game::Game()
{
	for (int i = 0; i < 52; i++)
		deck[i] = i;
	printDeck();
}
//Ths function is diplays the score of they players in the file.
Game::~Game()
{
	fstream myFile;
	myFile.open("data.txt", ios::out);
	myFile << playersnum << endl;
	for (int i = 0; i < playersnum; i++)
		myFile << finalscore[i] << endl;
}

//This function here sheuffle the deck before that start of the game.
void Game::shuffleDeck()
{
	int pos1, pos2, temp;
	srand(time(NULL));
	for (int i = 0; i < 1000; i++)
	{
		pos1 = rand() % 52;
		pos2 = rand() % 52;
		temp = deck[pos1];
		deck[pos1] = deck[pos2];
		deck[pos2] = temp;
	}

}

//This function here is to show us the cards and the compination of them.
void Game::printCard(int card)
{
	cout << "|";
	switch (card % 13)
	{
	case 0: cout << "A "; break;
	case 10: cout << "J "; break;
	case 11: cout << "Q "; break;
	case 12: cout << "K "; break;
	default: cout << card % 13 + 1 << " "; break;
	}

	switch (card / 13)
	{
	case 0: cout << 'D'; break;
	case 1: cout << 'H'; break;
	case 2: cout << 'C'; break;
	case 3: cout << 'S'; break;
	}
	cout << "| ";

}

//This function here is to print the deck.
void Game::printDeck()
{
	for (int count = 0; count < 52; count++)
	{
		printCard(deck[count]);
		if ((count + 1) % 13 == 0)	cout << endl;
	}
	cout << endl;
}

//This function is use to give each player the first 2 cards.
void Game::printHand(int hand[])
{
	int i;
	for (i = 1; i <= hand[0]; i++) {
		printCard(hand[i]);
		cout << "\n";
	}
}

//This function it reset the hand of the players.
void Game::resetHand(int hand[])
{
	hand[0] = 2;
}

//This function it will tell in each player if it is there round to play and after they all,
//is the round of dealer to play.
void Game::playGame(int pNum, bool newGame)
{
	int hands[6][15];
	int dealer[15], p_sum[6];
	int i, j, card;

	if (!newGame)
	{
		fstream myFile;
		myFile.open("data.txt", ios::in);
		if (myFile.is_open())
		{
			myFile >> playersnum;
			for (int i = 0; i < playersnum; i++)
				myFile >> finalscore[i];
		}
	}
	else
	{
		playersnum = pNum;
		for (int i = 0; i < playersnum; i++)
			finalscore[i] = 0;
	}

	shuffleDeck();
	printDeck();


	//Give each player 2 cards
	for (i = 0; i < playersnum; i++)
		resetHand(hands[i]);
	for (i = 0; i < playersnum; i++)
		for (j = 0; j < 2; j++)
			hands[i][j + 1] = deck[i * 2 + j];
	//dealer gets the 2 cards after players						
	dealer[0] = 2;
	for (i = 0; i < 2; i++)
		dealer[i + 1] = deck[playersnum * 2 + i];
	

	//set the position of next card
	card = (playersnum + 1) * 2;
	card = playersnum * 2;

	//playing game
	for (i = 0; i < playersnum; i++) {

		cout << endl << "-------->  Player " << i + 1 << "\' s turn  <---------" << endl;
		card = player(deck, hands[i], card);
	}

	cout << endl << "--------> Dealer' s turn <--------" << endl;
	card = dealerTurn(deck, dealer, card);
	int d_sum = sumResult(dealer);
	for (i = 0; i < playersnum; i++) {
		p_sum[i] = sumResult(hands[i]);
	}
	Winner(p_sum, d_sum, playersnum, finalscore);

}

//This funaction ASK players if they want another card and in the end it return that last card that they took.
int Game::player(int deck[], int p_hand[], int next_card)
{
	int give{};
	cout << "Player Hand:" << endl;
	printHand(p_hand);
	cout << endl;
	cout << "-->Card Values: " << sumResult(p_hand) << endl;


	cout << endl;
	cout << "Do you want more cards (0-yes 1-no)";
	cin >> give;
	while (give != 1 && !isBusted(sumResult(p_hand)))
	{
		p_hand[++p_hand[0]] = deck[next_card];
		cout << "Player Hand:";
		cout << endl;
		printHand(p_hand);
		cout << "-->Card Values: " << sumResult(p_hand) << endl;
		next_card++;
		if (isBusted(sumResult(p_hand)))
			cout << "* Player Busted with sum " << sumResult(p_hand) << "!!" << endl;
		else if (sumResult(p_hand) == 31)
		{
			cout << "Player has 31\n";
			give = 1;
		}
		else
		{
			cout << "Do you want more cards (0-yes 1-no)";
			cin >> give;
		}
	}

	return next_card;
}

//This function here is for dealer to play he needs to keep getting cards until his sums of cards is more than 25.
int Game::dealerTurn(int deck[], int d_hand[], int next_card)
{
	cout << "Dealer Hand:\n";
	cout << endl;
	printHand(d_hand);
	cout << endl;
	cout << "--> Card Values: " << sumResult(d_hand) << endl;
	while (sumResult(d_hand) <= 25) {
		cout << endl;
		d_hand[++d_hand[0]] = deck[next_card];
		cout << endl;
		cout << "Dealer Hand:\n";
		printHand(d_hand);
		cout << endl;
		cout << "--> Card Values: " << sumResult(d_hand) << endl;
		next_card++;
		if (isBusted(sumResult(d_hand)))
			cout << "* Dealer is Busted with sum " << sumResult(d_hand) << "!! *\n\n";
	}

	return next_card;
}

//This function it sums all the card of each player and shows it.
int Game::sumResult(int hand[])
{
	int sum = 0;
	int count;
	for (count = 1; count <= hand[0] % 13; count++)
		switch (hand[count] % 13) {
		case 10:
		case 11:
		case 12: sum += 10;
			break;
		default: sum += hand[count] % 13 + 1;
		}

	return sum;
}

//This functions is to use if some player have more than 31 sum so he/she will be busted,
//and move to next player
int Game::isBusted(int sum)
{
	return sum > 31;
}

//This function decide which player will win.
void Game::Winner(int p_sum[], int d_sum, int n, int score[]) {
	int i, max = 0;
	if (d_sum > 31)d_sum = 0;

	for (i = 0; i < n; i++) {
		if (p_sum[i] <= 31 && p_sum[i] > d_sum)
			score[i]++;
	}
	displayScore(score, n);
}

//This function it displays the score of who player won, how many points he/she got.
void Game::displayScore(int score[], int n) {
	int i;
	cout << "\n\-------------------\n";
	cout << "-     Result     -\n";
	cout << "-------------------\n\n";

	for (i = 0; i < n; i++)
		cout << "Player " << setw(2) << i + 1 << " | ";
	cout << endl;
	for (i = 0; i < n; i++)
		cout << "   " << setw(2) << score[i] << "     | ";
	cout << "\n\n\n---->End of Round <----\n\n\n\n";

}