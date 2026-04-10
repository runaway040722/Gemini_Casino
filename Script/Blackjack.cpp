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
    cout << "Total: " << getTotal() << endl; // 모든 상황에서 합계 공개
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
    if (bet <= 0) return;

    // 베팅금 선차감
    money -= bet;
    FlushBuffer();

    Hand player, dealer;
    player.addCard(deck.drawCard());
    dealer.addCard(deck.drawCard());
    player.addCard(deck.drawCard());
    dealer.addCard(deck.drawCard());

    bool playerDone = false;
    // 1. 플레이어 턴
    while (!playerDone) {
        system("cls");
        dealer.showHand("Dealer", false); // 딜러 카드 처음부터 공개
        player.showHand("Player", false);

        if (player.getTotal() >= 21) break;

        cout << "\n [1] 히트(Hit) [2] 스탠드(Stand) [3] 더블다운(Double): ";
        char choice = (char)_getch();

        if (choice == '1') {
            player.addCard(deck.drawCard());
        }
        else if (choice == '2') {
            playerDone = true;
        }
        else if (choice == '3') {
            if (money >= bet) { // 이미 bet이 차감된 상태이므로 추가로 bet만큼 있는지 확인
                money -= bet;   // 추가 베팅
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
    int dTotal = dealer.getTotal();

    // 2. 딜러 턴 (플레이어가 버스트되지 않았을 때만 진행)
    if (pTotal <= 21) {
        // 딜러의 합이 플레이어의 합보다 작으면 계속 뽑음 (요청하신 로직)
        while (dealer.getTotal() < pTotal && dealer.getTotal() < 21) {
            system("cls");
            dealer.showHand("Dealer", false);
            player.showHand("Player", false);
            cout << "\n 딜러가 당신을 이기기 위해 카드를 뽑는 중..." << endl;
            Sleep(1200);
            dealer.addCard(deck.drawCard());
        }
    }

    // 3. 최종 결과 화면
    system("cls");
    dealer.showHand("Dealer", false);
    player.showHand("Player", false);
    dTotal = dealer.getTotal();

    int winAmount = 0; // 정산 함수에 넘겨줄 최종 금액

    if (pTotal > 21) {
        SetColor(12); cout << "\n [ RESULT ] Player Bust! 패배했습니다." << endl;
        winAmount = 0;
    }
    else if (dTotal > 21) {
        SetColor(10); cout << "\n [ RESULT ] Dealer Bust! 승리했습니다!" << endl;
        winAmount = bet * 2;
    }
    else if (pTotal > dTotal) {
        SetColor(10); cout << "\n [ RESULT ] 승리했습니다!" << endl;
        winAmount = bet * 2;
    }
    else if (pTotal < dTotal) {
        SetColor(12); cout << "\n [ RESULT ] 패배했습니다." << endl;
        winAmount = 0;
    }
    else {
        SetColor(14); cout << "\n [ RESULT ] 무승부입니다." << endl;
        winAmount = bet; // 무승부는 원금 회수
    }

    // 공통 정산 함수 호출
    PrintResult(money, (winAmount == bet * 2 ? bet : bet), winAmount);

    ClearBuffer();
}