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
            // 1. 숨겨진 카드 처리 (딜러용)
            if (hideFirstCard && i == 0) {
                SetColor(8); // 회색
                if (line == 0)      cout << ".-------.  ";
                else if (line == 1) cout << "| ?     |  ";
                else if (line == 2) cout << "|  ???  |  ";
                else if (line == 3) cout << "|     ? |  ";
                else if (line == 4) cout << "'-------'  ";
                continue;
            }

            // 2. 색상 설정 (하트, 다이아는 빨간색)
            if (cards[i].suit == CardSuit::HEARTS || cards[i].suit == CardSuit::DIAMONDS)
                SetColor(12);
            else
                SetColor(15);

            string r = ranks[(int)cards[i].rank];

            // 3. 카드 라인별 출력 (너비 9칸 고정)
            if (line == 0) cout << ".-------.  ";
            else if (line == 1) {
                if (r == "10") cout << "| " << r << "    |  ";
                else           cout << "| " << r << "     |  ";
            }
            else if (line == 2) cout << "|   " << suitIcons[(int)cards[i].suit] << "  |  ";
            else if (line == 3) {
                if (r == "10") cout << "|    " << r << " |  ";
                else           cout << "|     " << r << " |  ";
            }
            else if (line == 4) cout << "'-------'  ";
        }
        cout << endl; // 한 줄 출력 후 줄바꿈
    }
    SetColor(15);
    cout << "Total: " << getTotal() << endl;
}

// --- 메인 로직 ---
void PlayBlackjack(int& money) {
    // [연동] 입장 시 파산 체크
    if (CheckBankruptcy("당신", money, 1, true)) return;

    while (true) {
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
        int totalBet = bet; // 정산 기록을 위한 실제 판돈
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
            dealer.showHand("Dealer", false);
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
                if (money >= bet) {
                    money -= bet;
                    totalBet *= 2;
                    player.addCard(deck.drawCard());
                    playerDone = true;
                }
                else {
                    SetColor(12); cout << "\n [!] 잔액 부족!"; SetColor(15); Sleep(600);
                }
            }
        }

        int pTotal = player.getTotal();

        // 2. 딜러 턴 (플레이어가 버스트되지 않았을 때만 진행)
        if (pTotal <= 21) {
            while (dealer.getTotal() < pTotal && dealer.getTotal() < 21) {
                system("cls");
                dealer.showHand("Dealer", false);
                player.showHand("Player", false);
                cout << "\n 딜러가 카드를 뽑는 중..." << endl;
                Sleep(1000);
                dealer.addCard(deck.drawCard());
            }
        }

        // 3. 최종 결과 판정
        system("cls");
        dealer.showHand("Dealer", false);
        player.showHand("Player", false);
        int dTotal = dealer.getTotal();

        int winAmount = 0;

        if (pTotal > 21) {
            SetColor(12); cout << "\n [ RESULT ] Player Bust! 패배했습니다." << endl;
            winAmount = 0;
        }
        else if (dTotal > 21) {
            SetColor(10); cout << "\n [ RESULT ] Dealer Bust! 승리했습니다!" << endl;
            winAmount = totalBet * 2;
        }
        else if (pTotal > dTotal) {
            SetColor(10); cout << "\n [ RESULT ] 승리했습니다!" << endl;
            winAmount = totalBet * 2;
        }
        else if (pTotal < dTotal) {
            SetColor(12); cout << "\n [ RESULT ] 패배했습니다." << endl;
            winAmount = 0;
        }
        else {
            SetColor(14); cout << "\n [ RESULT ] 무승부입니다." << endl;
            winAmount = totalBet;
        }

        // [연동] 공통 정산 함수 호출
        PrintResult(money, totalBet, winAmount);

        // [연동] 판이 끝난 후 플레이어 파산 체크 (0원이면 즉시 광산행)
        if (CheckBankruptcy("당신", money, 1, true)) return;

        SetColor(15);
        cout << "\n [1] 다시하기 [0] 나가기 : ";
        int choice;
        cin >> choice;
        if (choice == 0) break;
    }
}