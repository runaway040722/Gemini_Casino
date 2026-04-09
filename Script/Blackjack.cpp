#include "Blackjack.h"
#include "System.h"   
#include <algorithm>
#include <random>
#include <iostream>  
#include <conio.h>
#include <windows.h>

using namespace std;

// --- 클래스 메서드 구현 ---

int Card::getValue() const {
    if (rank >= CardRank::TEN) return 10;
    if (rank == CardRank::ACE) return 11;
    return (int)rank;
}

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
    if (cards.empty()) { *this = Deck(); }
    Card c = cards.back();
    cards.pop_back();
    return c;
}

void Hand::addCard(Card card) {
    cards.push_back(card);
}

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

void Hand::showHand(string owner) const {
    string ranks[] = { "", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
    string suitIcons[] = { "SP", "HT", "DI", "CL" };

    cout << "\n--- " << owner << "'s Hand ---" << endl;
    for (int line = 0; line < 5; ++line) {
        for (const auto& card : cards) {
            if (card.suit == CardSuit::HEARTS || card.suit == CardSuit::DIAMONDS) SetColor(12);
            else SetColor(15);

            string r = ranks[(int)card.rank];

            if (line == 0) cout << ".-------.  ";
            else if (line == 1) {
                if (r == "10") cout << "| " << r << "    |  ";
                else           cout << "| " << r << "     |  ";
            }
            else if (line == 2) cout << "|  " << suitIcons[(int)card.suit] << "   |  ";
            else if (line == 3) {
                if (r == "10") cout << "|    " << r << " |  ";
                else           cout << "|     " << r << " |  ";
            }
            else if (line == 4) cout << "'-------'  ";
        }
        cout << endl;
    }
    SetColor(15);
    cout << "Total: " << getTotal() << endl;
}

// --- 게임 실행 로직 ---

void PlayBlackjack(int& money) {
    Deck deck;
    system("cls");
    SetColor(11);
    cout << "===============================================" << endl;
    cout << "               [ 2. BLACKJACK ]" << endl;
    cout << "===============================================" << endl;
    SetColor(15);

    int bet;
    cout << "배팅 금액: ";
    if (!(cin >> bet) || bet <= 0 || bet > money) {
        cout << "잘못된 배팅 금액입니다!" << endl;
        ClearBuffer();
        return;
    }
    FlushBuffer();

    Hand player, dealer;

    // 초기 배분 애니메이션
    system("cls");
    cout << "\n카드를 나누어 줍니다..." << endl;
    player.addCard(deck.drawCard());
    player.showHand("Player");
    Sleep(600);
    dealer.addCard(deck.drawCard());
    system("cls");
    dealer.showHand("Dealer (Hidden)");
    player.showHand("Player");
    Sleep(600);
    player.addCard(deck.drawCard());
    system("cls");
    dealer.showHand("Dealer (Hidden)");
    player.showHand("Player");
    Sleep(600);
    dealer.addCard(deck.drawCard());
    system("cls");
    dealer.showHand("Dealer (Hidden)");
    player.showHand("Player");

    // --- 플레이어 턴 (1: Hit, 2: Stand, 3: Double) ---
    while (true) {
        if (player.getTotal() >= 21) break;

        while (_kbhit()) _getch(); // 키보드 버퍼 비우기

        cout << "\n[1] 히트(Hit) [2] 스탠드(Stand) [3] 더블다운(Double): ";
        char choice = (char)_getch();

        if (choice == '1') {
            player.addCard(deck.drawCard());
            system("cls");
            dealer.showHand("Dealer (Hidden)");
            player.showHand("Player");
            Sleep(400);
        }
        else if (choice == '2') {
            break;
        }
        else if (choice == '3') {
            if (money >= bet * 2) {
                bet *= 2;
                player.addCard(deck.drawCard());
                system("cls");
                dealer.showHand("Dealer (Hidden)");
                player.showHand("Player");
                Sleep(400);
                break;
            }
            else {
                cout << "\n잔액이 부족합니다!";
                Sleep(800);
            }
        }
    }

    // 딜러 턴
    int pTotal = player.getTotal();
    if (pTotal <= 21) {
        while (dealer.getTotal() < 17) {
            system("cls");
            cout << "\n딜러가 17점 미만이라 카드를 더 뽑습니다..." << endl;
            dealer.showHand("Dealer");
            player.showHand("Player");
            Sleep(1000);
            dealer.addCard(deck.drawCard());
        }
    }

    // 결과
    system("cls");
    dealer.showHand("Dealer");
    player.showHand("Player");

    int dTotal = dealer.getTotal();
    cout << "\n===============================================" << endl;
    cout << " [결과] ";

    if (pTotal > 21) {
        SetColor(12); cout << "Player Bust! 패배..." << endl;
        money -= bet;
    }
    else if (dTotal > 21) {
        SetColor(10); cout << "Dealer Bust! Player Win!" << endl;
        money += bet;
    }
    else if (pTotal > dTotal) {
        SetColor(10); cout << "Player Win! 축하합니다!" << endl;
        money += bet;
    }
    else if (pTotal < dTotal) {
        SetColor(12); cout << "Dealer Win! 패배..." << endl;
        money -= bet;
    }
    else {
        SetColor(14); cout << "Push! 무승부입니다." << endl;
    }

    SetColor(15);
    cout << "===============================================" << endl;
    cout << "현재 보유 자산: $" << money << endl;

    ClearBuffer();
}