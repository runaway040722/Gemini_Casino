#include <iostream>
#include <algorithm>
#include <random>
#include <string>
#include <windows.h>
#include <conio.h>

#include "Blackjack.h"
#include "System.h"

using namespace std;

// --- Card 구현 ---
int Card::getValue() const {
    if ((int)rank >= 10) return 10;
    if (rank == CardRank::ACE) return 11;
    return (int)rank;
}

// --- Deck 구현 ---
Deck::Deck() {
    for (int s = 0; s < 4; ++s) {
        for (int r = 1; r <= 13; ++r) {
            cards.push_back({ static_cast<CardRank>(r), static_cast<CardSuit>(s) });
        }
    }
    shuffleDeck();
}

void Deck::shuffleDeck() {
    random_device rd;
    mt19937 g(rd());
    shuffle(cards.begin(), cards.end(), g);
}

Card Deck::drawCard() {
    if (cards.empty()) { Deck temp; *this = temp; }
    Card c = cards.back();
    cards.pop_back();
    return c;
}

// --- Hand 구현 ---
void Hand::addCard(Card card) { cards.push_back(card); }

int Hand::getTotal() const {
    int total = 0;
    int aceCount = 0;
    for (const auto& card : cards) {
        total += card.getValue();
        if (card.rank == CardRank::ACE) aceCount++;
    }
    while (total > 21 && aceCount > 0) {
        total -= 10;
        aceCount--;
    }
    return total;
}

void Hand::showHand(string owner, bool hideFirstCard) const {
    string ranks[] = { "", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
    string suitIcons[] = { "SP", "HT", "DI", "CL" };

    cout << "\n--- " << owner << "'s Hand ---" << endl;
    for (int line = 0; line < 5; ++line) {
        for (size_t i = 0; i < cards.size(); ++i) {
            if (hideFirstCard && i == 0) {
                SetColor(8);
                if (line == 0) cout << ".-------.  ";
                else if (line == 1 || line == 3) cout << "| ?   ? |  ";
                else if (line == 2) cout << "|  ???  |  ";
                else if (line == 4) cout << "'-------'  ";
                continue;
            }

            if (cards[i].suit == CardSuit::HEARTS || cards[i].suit == CardSuit::DIAMONDS) SetColor(12);
            else SetColor(15);

            string r = ranks[(int)cards[i].rank];
            if (line == 0) cout << ".-------.  ";
            else if (line == 1) {
                if (r == "10") cout << "| " << r << "    |  ";
                else           cout << "| " << r << "     |  ";
            }
            else if (line == 2) cout << "|  " << suitIcons[(int)cards[i].suit] << "   |  ";
            else if (line == 3) {
                if (r == "10") cout << "|    " << r << " |  ";
                else           cout << "|     " << r << " |  ";
            }
            else if (line == 4) cout << "'-------'  ";
        }
        cout << endl;
    }
    SetColor(15);
    if (!hideFirstCard) cout << "Total: " << getTotal() << endl;
    else cout << "Total: ???" << endl;
}

// --- 메인 로직 ---
void PlayBlackjack(int& money) {
    Deck deck;
    system("cls");
    SetColor(11);
    cout << "===============================================" << endl;
    cout << "                [ 2. BLACKJACK ]" << endl;
    cout << "===============================================" << endl;
    SetColor(15);

    int bet = GetBetAmount(money);
    FlushBuffer();

    Hand player, dealer;
    player.addCard(deck.drawCard());
    dealer.addCard(deck.drawCard());
    player.addCard(deck.drawCard());
    dealer.addCard(deck.drawCard());

    bool playerDone = false;
    while (!playerDone) {
        system("cls");
        dealer.showHand("Dealer", true);
        player.showHand("Player", false);

        if (player.getTotal() >= 21) break;

        cout << "\n [1] 히트(Hit) [2] 스탠드(Stand) [3] 더블다운(Double): ";
        char choice = (char)_getch();

        if (choice == '1') player.addCard(deck.drawCard());
        else if (choice == '2') playerDone = true;
        else if (choice == '3') {
            if (money >= bet * 2) {
                bet *= 2;
                player.addCard(deck.drawCard());
                playerDone = true;
            }
            else {
                SetColor(12); cout << "\n [!] 잔액 부족!"; SetColor(15); Sleep(600);
            }
        }
    }

    int pTotal = player.getTotal();
    if (pTotal <= 21) {
        while (dealer.getTotal() < 17) {
            system("cls");
            dealer.showHand("Dealer", false);
            player.showHand("Player", false);
            cout << "\n 딜러가 카드를 뽑는 중..." << endl;
            Sleep(1000);
            dealer.addCard(deck.drawCard());
        }
    }

    system("cls");
    dealer.showHand("Dealer", false);
    player.showHand("Player", false);
    int dTotal = dealer.getTotal();

    cout << "\n===============================================" << endl;
    if (pTotal > 21) { SetColor(12); cout << "Bust! 패배"; money -= bet; }
    else if (dTotal > 21 || pTotal > dTotal) { SetColor(10); cout << "승리!"; money += bet; }
    else if (pTotal < dTotal) { SetColor(12); cout << "패배"; money -= bet; }
    else { SetColor(14); cout << "무승부"; }
    SetColor(15);
    cout << "\n===============================================" << endl;
    ClearBuffer();
}