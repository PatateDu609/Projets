#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <list>
#include <array>
#include <string>
#include <sstream>

typedef std::array<int, 2> Card;
typedef std::vector<Card> Pack;
typedef std::vector<Card> Hand;

struct Winner {
	Card c;
	int id;
};

//Définition cartes
char color[4] = {
		'S',
		'H',
		'D',
		'C'
};

char p[13] = {
	'2', //0
	'3', //1
	'4', //2
	'5', //3
	'6', //4
	'7', //5
	'8', //6
	'9', //7
	'I', //8
	'J', //9
	'Q', //10
	'R', //11
	'S'  //12
};

std::string translate(Card c) {
	std::ostringstream cardStream;

	cardStream << color[c[0]];
	switch (c[1]) {
	case 8:
		cardStream << "10";
		break;
	case 11:
		cardStream << "K";
		break;
	case 12:
		cardStream << "A";
		break;
	default:
		cardStream << p[c[1]];
	}
	return cardStream.str();
}

Card translate(char colC, char powC) {
	int col, pow;
	switch (colC) {
	case 'S':
		col = 0;
		break;
	case 'H':
		col = 1;
		break;
	case 'D':
		col = 2;
		break;
	case 'C':
		col = 3;
		break;
	default:
		col = -1;
	}

	switch (powC) {
	case '2':
		pow = 0;
		break;
	case '3':
		pow = 1;
		break;
	case '4':
		pow = 2;
		break;
	case '5':
		pow = 3;
		break;
	case '6':
		pow = 4;
		break;
	case '7':
		pow = 5;
		break;
	case '8':
		pow = 6;
		break;
	case '9':
		pow = 7;
		break;
	case '1':
		pow = 8;
		break;
	case 'J':
		pow = 9;
		break;
	case 'Q':
		pow = 10;
		break;
	case 'K':
		pow = 11;
		break;
	case 'A':
		pow = 12;
		break;
	default:
		pow = -1;
	}

	return { col, pow };
}

Card compare(int color, int contract, Card const& a, Card const& b) {
	Card c;
	int x = a[0] - b[0], y = a[1] - b[1];
	bool ac = (a[0] == color),
		bc = (b[0] == color),
		acontract = (a[0] == contract),
		bcontract = (b[0] == contract);

	if (x == 0 && ac) {
		if (a[1] == 0 || b[1] == 0) { //règle le cas de l'as
			if (a[1] == 0) c = a;
			if (b[1] == 0) c = b;
		}
		else c = (y > 0) ? a : b;
	}
	else if (bc && !ac) {
		if (!acontract) c = b;
		else c = a;
	}
	else if (ac && !bc) {
		if (!bcontract) c = a;
		else c = b;
	}
	else if (x == 0 && !ac) {
		if (acontract) {
			if (a[1] == 0 || b[1] == 0) { //règle le cas de l'as
				if (a[1] == 0) c = a;
				if (b[1] == 0) c = b;
			}
			else c = (y > 0) ? a : b;
		}
	}

	return c;
}

std::list<char>::iterator find(std::list<char>::iterator start, std::list<char>::iterator end, const char& value) {
	for (; start != end; ++start) {
		if (*start == value) return start;
	}
	return end;
}

std::vector<Card>::iterator find(std::vector<Card>::iterator start, std::vector<Card>::iterator end, const Card& value) {
	for (; start != end; ++start) {
		if (*start == value) return start;
	}
	return end;
}

void showCards(const Hand &h) {
	//Adapte l'affichage (classe les cartes par ordre de puissance) et affiche
	std::list<char> c[4];
	for (int i = 0; i < h.size(); i++) c[h[i][0]].push_back(p[h[i][1]]);

	for (int i = 0; i < 4; i++) {
		c[i].sort();
		c[i].reverse();

		std::list<char>::iterator searchS = find(c[i].begin(), c[i].end(), 'S');
		std::list<char>::iterator searchR = find(c[i].begin(), c[i].end(), 'R');

		if (searchS != c[i].end()) *searchS = 'A';
		if (searchR != c[i].end()) *searchR = 'K';
	}

	for (int i = 0; i < 4; i++) {
		c[i].push_front(':');
		c[i].push_front(color[i]);
	}
	for (int i = 0; i < 4; i++) {
		for (char cha : c[i]) {
			if (cha == 'I') std::cout << 10;
			else std::cout << cha;

			std::cout << " ";
		}
		std::cout << std::endl;
	}
}
bool playable(int color, Hand h, Card c) {
	if (find(std::begin(h), std::end(h), c) != std::end(h)) {
		if (c[0] == color) return true;
		else {
			for (int i = 0; i < 13; i++) {
				Card d = { c[0], i };
				if (find(std::begin(h), std::end(h), d) != std::end(h)) return false;
			}
			return true;
		}
	}

	return false;
}

std::array<Hand, 4> deal() {
	std::srand(std::time(0));
	Pack pack, pack1;
	std::array<Hand, 4> hands;
	Card c;

	//Remplis le paquet
	for (int i = 0; i < 52; i++) {
		c[0] = i / 13;
		c[1] = i % 13;
		pack.push_back(c);
	}
	//Mélange
	for (int i = 51; i >= 0; i--) {
		std::vector<Card>::iterator it = pack.begin();
		int j = (i != 0) ? (rand() % i) : 0;
		pack1.push_back(pack.at(j));
		pack.erase(it + j);
	}
	pack = pack1;

	//Distribue
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			hands[i].push_back(pack.back());
			pack.pop_back();
		}
	}
	return hands;
}


int main() {
	std::array<Hand, 4> hands = deal();
	std::string command; //commande play, pour jouer son tour
	char col, pow;	//carte du joueur
	Card cardT, defcard = { -1, -1 }, w;
	std::array<Card, 4> playedCards;
	int color, contract = -1, turn = 3, fp = turn;
	Winner winner;
	std::array<std::string, 4> players = { "North", "East", "South", "West" };
	bool verify = 1;
	std::array<int, 2> score = { 0, 0 };

	for (int l = 0; l < 13; l++) {
		for (int i = 0; i < 4; i++, ++turn) {
			turn %= 4;
			std::cout << players[turn] << " plays" << std::endl;
			showCards(hands[turn]);

			do {//lecture de la commande et affectation aux variables
				std::cout << "Enter command" << std::endl;
				std::cin.clear();
				std::getline(std::cin, command);
				if (command == "stop") {
					return(0);
				}

				else if (command == "play") {
					std::cout << "Enter card" << std::endl;
					std::cin >> col >> pow;
					std::cin.ignore();
					cardT = translate(col, pow);

					if (cardT == defcard) std::cout << "The card you entered isn't existing" << std::endl;
					else {
						playedCards[turn] = cardT;
						if(i == 0) color = playedCards[turn][0];
						if (!playable(color, hands[turn], cardT)) std::cout << "The card you entered isn't in your hand, or you can't discard" << std::endl;
						else {
							verify = 0;
							hands[turn].erase(find(std::begin(hands[turn]), std::end(hands[turn]), cardT));
						}
					}
				}
				else if (command == "contract") {
					std::string c;

					switch (contract) {
					case -1:
						c = "Notrump";
						break;
					case 0:
						c = "Spades";
						break;
					case 1:
						c = "Heart";
						break;
					case 2:
						c = "Diamond";
						break;
					case 3:
						c = "Club";
						break;
					}

					std::cout << c << std::endl;
				}
				else if (command == "game" && i > 0) {
					for (int j = 0, jp = fp; j < i; j++, jp++) {
						jp %= 4;
						std::cout << players[jp] << " : " << translate(playedCards[jp]) << std::endl;
					}
				}
				else if (command == "score")
					std::cout << "Team North/South : " << score[0] << std::endl << "Team East/West : " << score[1] << std::endl;
				else if (command == "help" || command == "?") {
					std::cout << "Type play to play, then enter your card value (colour then power)\n";
					std::cout << "Type stop to stop the program\n";
					std::cout << "Type contract to see the current contract\n";
					std::cout << "Type score to see the current score\n";
					std::cout << "Type game to see the cards played by the others\n";
					std::cout << "Type help to see all the commands\n";
				}
			} while (verify);
			verify = 1;

			// Comparaison des cartes et détermine le vainqueur du pli
			if (i == 0) {
				winner.c = playedCards[turn];
				winner.id = turn;
			}
			else {
				w = compare(color, contract, winner.c, playedCards[turn]);
				if (w != winner.c) winner.id = turn;
				winner.c = w;

				if (i == 3) {
					++score[winner.id % 2];
					turn = winner.id - 1;
					fp = turn;
					for (int j = 0; j < 4; ++j) playedCards[j] = defcard;
				}
			}
		}
	}
	system("PAUSE");
	return 0;
}