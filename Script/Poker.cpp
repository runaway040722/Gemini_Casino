#include "Poker.h"
#include <windows.h>
#include <iomanip>
#include <ctime>
#include <conio.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

using namespace std;

// [보조 함수] 카드 위력 점수 (A=14, 스다하클 적용)
// 스(3) > 다(2) > 하(1) > 클(0)
int GetCardScore(int rank, int suit) {
    int power = (rank == 1) ? 14 : rank;
    int score = power * 10;
    if (suit == 0) score += 3;      // ♠
    else if (suit == 2) score += 2; // ◆
    else if (suit == 1) score += 1; // ♥
    else if (suit == 3) score += 0; // ♣
    return score;
}

// 생성자
Poker::Poker(int& playerMoney) : playerRef(playerMoney), pot(0), currentBet(0) {
    srand((unsigned int)time(NULL));
    players.clear();

    // 요청하신 이름 리스트 그대로 유지
    vector<string> namePool = {
        "민지", "학진", "재용", "라이덴 쇼군", "서연", "규찬", "미토마", "이강인",
        "나카노 미쿠", "카인", "선바", "선우", "유현", "정우", "세현", "동현",
        "영환", "프리렌", "나카노 니노", "리쿠하치마 아루", "유린", "카프카",
        "호두", "감우", "응광", "클레", "엘리스", "부현", "은랑", "스파클",
        "시틀라리", "나히다", "마비카", "닐루", "유라 로렌스", "미소노 미카", "야란", "세진",
        "푸리나", "청작", "아글라이아", "사이퍼", "카스토리스", "제레", "히아킨", "키레네",
        "파이논", "아낙사", "마이데이", "야에 미코", "유메미즈키 미즈키", "치치", "타이나리",
        "각청", "진", "바르카", "느비예트", "플린스", "두린", "니콜",
        "호날두", "니코 오라일리", "아리마 카나", "쿠죠 아리사", "스오우 유키",
        "키타가와 마린", "페른", "슈타르크", "아우라", "위벨"
    };

    random_device rd;
    mt19937 g(rd());
    std::shuffle(namePool.begin(), namePool.end(), g);

    // AI 기본금: 판돈 설정에 대응하기 위해 10,000달러로 설정
    int initialBuyIn = 10000;

    players.push_back({ "나 (Player)", {}, playerRef, false });
    for (int i = 0; i < 3; i++) {
        players.push_back({ "AI " + to_string(i + 1) + " (" + namePool[i] + ")", {}, initialBuyIn, false });
    }

    initDeck();
}

void Poker::initDeck() {
    deck.clear();
    for (int s = 0; s < 4; s++) {
        for (int r = 1; r <= 13; r++) deck.push_back({ s, r });
    }
}

void Poker::shuffleDeck() {
    random_device rd;
    mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

string Poker::GetRankStr(int rank) {
    if (rank == 1) return "A";
    if (rank == 11) return "J";
    if (rank == 12) return "Q";
    if (rank == 13) return "K";
    return to_string(rank);
}

// 상세 족보 이름 생성 (예: "3 6 투 페어")
string Poker::GetDetailedRankName(vector<PokerCard>& hand) {
    int counts[15] = { 0 };
    for (auto& c : hand) counts[c.rank]++;
    vector<int> fours, triples, pairs, singles;
    int priority[] = { 1, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2 };

    for (int r : priority) {
        if (counts[r] == 4) fours.push_back(r);
        else if (counts[r] == 3) triples.push_back(r);
        else if (counts[r] == 2) pairs.push_back(r);
        else if (counts[r] == 1) singles.push_back(r);
    }

    if (!fours.empty()) return GetRankStr(fours[0]) + " 포카드";
    if (!triples.empty() && !pairs.empty()) return GetRankStr(triples[0]) + " " + GetRankStr(pairs[0]) + " 풀하우스";
    if (!triples.empty()) return GetRankStr(triples[0]) + " 트리플";
    if (pairs.size() >= 2) return GetRankStr(pairs[0]) + " " + GetRankStr(pairs[1]) + " 투 페어";
    if (pairs.size() == 1) return GetRankStr(pairs[0]) + " 원 페어";
    if (!singles.empty()) return GetRankStr(singles[0]);
    return "하이 카드";
}

void Poker::showTable(bool revealAll) {
    system("cls");
    SetColor(14);
    cout << "==========================================================" << endl;
    cout << "                [ 4인 포커 대결 모드 ]                    " << endl;
    cout << "          현재 총 판돈(POT): " << pot << " 달러" << endl;
    cout << "==========================================================" << endl;
    SetColor(15);

    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded) {
            SetColor(8);
            cout << left << setw(25) << players[i].name << " : [ 기권(FOLDED) ]" << endl;
        }
        else {
            SetColor(i == 0 ? 11 : 15);
            cout << left << setw(25) << players[i].name << " : ";
            for (auto& card : players[i].hand) {
                if (i == 0 || revealAll) {
                    string s_mark;
                    if (card.suit == 0) { SetColor(15); s_mark = "♠"; }
                    else if (card.suit == 1) { SetColor(12); s_mark = "♥"; }
                    else if (card.suit == 2) { SetColor(12); s_mark = "◆"; }
                    else { SetColor(15); s_mark = "♣"; }
                    cout << s_mark << GetRankStr(card.rank) << " ";
                    SetColor(i == 0 ? 11 : 15);
                }
                else cout << "?? ";
            }
            cout << " (잔고: " << players[i].money << "$)";
            if (i == 0 || revealAll) {
                SetColor(10);
                cout << " (" << GetDetailedRankName(players[i].hand) << ")";
            }
            cout << endl;
        }
    }
    SetColor(15);
    cout << "==========================================================" << endl;
}

void Poker::play() {
    system("cls");
    SetColor(11);
    cout << "\n  [ 포커 게임 설정 ]" << endl;
    SetColor(15);
    cout << "  기본 베팅 단위를 선택하세요." << endl;
    cout << "  1. 50$  |  2. 100$  |  3. 200$  |  4. 500$" << endl;
    cout << "  선택: ";

    int menu, baseBet = 100;
    cin >> menu;
    if (menu == 1) baseBet = 50;
    else if (menu == 2) baseBet = 100;
    else if (menu == 3) baseBet = 200;
    else if (menu == 4) baseBet = 500;

    int ante = baseBet / 2;
    currentBet = baseBet;

    shuffleDeck();
    int deckIdx = 0;
    pot = 0;

    for (auto& p : players) {
        p.money -= ante;
        pot += ante;
        p.hand.clear();
        p.isFolded = false;
    }

    for (int i = 0; i < 2; i++) {
        for (auto& p : players) p.hand.push_back(deck[deckIdx++]);
    }

    for (int round = 1; round <= 3; round++) {
        showTable(false);
        bettingRound();

        int active = 0;
        for (auto& p : players) if (!p.isFolded) active++;
        if (active <= 1) break;

        cout << "\n[" << round << "회차 카드 배분 중...]" << endl;
        Sleep(800);
        for (auto& p : players) if (!p.isFolded) p.hand.push_back(deck[deckIdx++]);
    }

    showTable(true);
    int winnerIdx = evaluateWinner();

    SetColor(10);
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << " 승자: " << players[winnerIdx].name << endl;
    cout << " 결과: " << GetDetailedRankName(players[winnerIdx].hand) << endl;
    cout << " 상금: " << pot << "$" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;

    players[winnerIdx].money += pot;
    playerRef = players[0].money;

    SetColor(15);
    system("pause");
}

bool Poker::bettingRound() {
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded || players[i].money <= 0) continue;

        if (i == 0) {
            int halfBet = pot / 2;
            cout << "\n[1] 콜(" << currentBet << "$) [2] 하프(" << halfBet << "$) [3] 다이: ";
            int choice; cin >> choice;
            if (choice == 2) {
                players[i].money -= halfBet; pot += halfBet; currentBet = halfBet;
            }
            else if (choice == 3) {
                players[i].isFolded = true;
            }
            else {
                players[i].money -= currentBet; pot += currentBet;
            }
        }
        else {
            cout << players[i].name << " 생각 중..."; Sleep(600);
            HandRank aiRank = checkHandForPlayer(players[i].hand);
            int prob = rand() % 100;
            if (aiRank == HandRank::HIGH_CARD && prob < 40) {
                players[i].isFolded = true; cout << " [다이]" << endl;
            }
            else if (aiRank >= HandRank::TWO_PAIR && prob < 20) {
                int halfBet = pot / 2;
                players[i].money -= halfBet; pot += halfBet; currentBet = halfBet; cout << " [하프!!]" << endl;
            }
            else {
                players[i].money -= currentBet; pot += currentBet; cout << " [콜]" << endl;
            }
        }
    }
    return true;
}

HandRank Poker::checkHandForPlayer(vector<PokerCard>& hand) {
    int counts[15] = { 0 };
    for (auto& c : hand) counts[c.rank]++;
    int pairs = 0, triples = 0, fours = 0;
    for (int i = 1; i <= 13; i++) {
        if (counts[i] == 4) fours++;
        else if (counts[i] == 3) triples++;
        else if (counts[i] == 2) pairs++;
    }
    if (fours >= 1) return HandRank::FOUR_OF_A_KIND;
    if (triples >= 1 && pairs >= 1) return HandRank::FULL_HOUSE;
    if (triples >= 1) return HandRank::THREE_OF_A_KIND;
    if (pairs >= 2) return HandRank::TWO_PAIR;
    if (pairs == 1) return HandRank::ONE_PAIR;
    return HandRank::HIGH_CARD;
}

int Poker::evaluateWinner() {
    int winnerIdx = 0;
    HandRank bestRank = HandRank::HIGH_CARD;
    int bestHighCardScore = -1;

    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded) continue;
        HandRank currentRank = checkHandForPlayer(players[i].hand);
        int currentMaxScore = -1;
        for (auto& c : players[i].hand) {
            int s = GetCardScore(c.rank, c.suit);
            if (s > currentMaxScore) currentMaxScore = s;
        }

        if (currentRank > bestRank) {
            bestRank = currentRank;
            winnerIdx = i;
            bestHighCardScore = currentMaxScore;
        }
        else if (currentRank == bestRank) {
            if (currentMaxScore > bestHighCardScore) {
                winnerIdx = i;
                bestHighCardScore = currentMaxScore;
            }
        }
    }
    return winnerIdx;
}