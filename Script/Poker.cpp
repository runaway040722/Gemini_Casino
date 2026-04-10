#include "Poker.h"
#include "System.h" // 파산 및 연출 시스템 연동
#include <windows.h>
#include <iomanip>
#include <algorithm>
#include <random>

extern vector<string> globalNamePool;

// [보조] 카드 파워 계산 (A를 14로 처리)
static int GetCardPower(int rank) { return (rank == 1) ? 14 : rank; }

// [보조] 족보 수치 계산 (내부 로직용)
static long long GetHandStrength(vector<PokerCard>& hand) {
    if (hand.empty()) return 0;
    int counts[15] = { 0 }, suits[4] = { 0 };
    for (auto& c : hand) { counts[GetCardPower(c.rank)]++; suits[c.suit]++; }

    bool isFlush = false;
    for (int i = 0; i < 4; i++) if (suits[i] >= 5) isFlush = true;

    int sH = 0, consecutive = 0;
    for (int i = 14; i >= 2; i--) {
        if (counts[i] > 0) { consecutive++; if (consecutive >= 5) { sH = i + 4; break; } }
        else consecutive = 0;
    }
    if (sH == 0 && counts[14] && counts[2] && counts[3] && counts[4] && counts[5]) sH = 5;

    int f = 0, t = 0, h = 0; vector<int> p;
    for (int i = 14; i >= 2; i--) {
        if (counts[i] == 4) f = i; else if (counts[i] == 3) t = i; else if (counts[i] == 2) p.push_back(i);
        if (counts[i] > 0 && i > h) h = i;
    }

    if (isFlush && sH > 0) return 9000000LL + sH;
    if (f > 0) return 8000000LL + f;
    if (t > 0 && !p.empty()) return 7000000LL + (t * 100) + p[0];
    if (isFlush) return 6000000LL + h;
    if (sH > 0) return 5000000LL + sH;
    if (t > 0) return 4000000LL + t;
    if (p.size() >= 2) return 3000000LL + (p[0] * 100) + p[1];
    if (!p.empty()) return 2000000LL + p[0];
    return 1000000LL + h;
}

Poker::Poker(int& playerMoney) : playerRef(playerMoney), pot(0), currentBet(0) {
    players.clear();
    players.push_back({ "나 (Player)", {}, playerRef, false });

    random_device rd; mt19937 g(rd());
    vector<string> tempPool = globalNamePool;
    shuffle(tempPool.begin(), tempPool.end(), g);

    for (int i = 0; i < 3; i++) {
        players.push_back({ tempPool[i], {}, 100000, false });
    }
    initDeck();
}

void Poker::initDeck() {
    deck.clear();
    for (int s = 0; s < 4; s++)
        for (int r = 1; r <= 13; r++)
            deck.push_back({ s, r });
}

void Poker::shuffleDeck() {
    random_device rd; mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
}

string Poker::GetRankStr(int rank) {
    if (rank == 14 || rank == 1) return "A";
    if (rank == 11) return "J"; if (rank == 12) return "Q"; if (rank == 13) return "K";
    return to_string(rank);
}

string Poker::GetDetailedRankName(vector<PokerCard>& hand) {
    if (hand.empty()) return "없음";
    long long s = GetHandStrength(hand);
    int main = (int)(s % 100);
    int sub = (int)((s % 10000) / 100);

    if (s >= 9000000LL) return "스트레이트 플러시";
    if (s >= 8000000LL) return GetRankStr(main) + " 포카드";
    if (s >= 7000000LL) return GetRankStr(sub) + " & " + GetRankStr(main) + " 풀하우스";
    if (s >= 6000000LL) return "플러시";
    if (s >= 5000000LL) return GetRankStr(main) + " 스트레이트";
    if (s >= 4000000LL) return GetRankStr(main) + " 트리플";
    if (s >= 3000000LL) return GetRankStr(sub) + " & " + GetRankStr(main) + " 투 페어";
    if (s >= 2000000LL) return GetRankStr(main) + " 원 페어";
    return GetRankStr(main) + " 하이";
}

void Poker::showTable(bool revealAll) {
    system("cls");
    SetColor(14);
    cout << "==========================================================" << endl;
    cout << "                [ 5카드 포커 테이블 ]" << endl;
    cout << "           총 판돈(POT): " << pot << " 달러" << endl;
    cout << "==========================================================" << endl;
    for (int i = 0; i < (int)players.size(); i++) {
        SetColor(i == 0 ? 11 : 15);
        if (players[i].isFolded) {
            SetColor(8); cout << left << setw(15) << players[i].name << " : [기권]" << endl;
        }
        else {
            cout << left << setw(15) << players[i].name << " : ";
            for (auto& c : players[i].hand) {
                if (i == 0 || revealAll) {
                    if (c.suit == 1 || c.suit == 2) SetColor(12); else SetColor(15);
                    string sm; if (c.suit == 0) sm = "♠"; else if (c.suit == 1) sm = "♥"; else if (c.suit == 2) sm = "◆"; else sm = "♣";
                    cout << sm << GetRankStr(c.rank) << " ";
                    SetColor(i == 0 ? 11 : 15);
                }
                else cout << "?? ";
            }
            if (i == 0 || revealAll) {
                SetColor(10); cout << " [" << GetDetailedRankName(players[i].hand) << "] ";
            }
            SetColor(i == 0 ? 11 : 15);
            int moneyDisplay = (i == 0) ? playerRef : players[i].money;
            cout << "(" << moneyDisplay << "$)" << endl;
        }
    }
    SetColor(15);
}

bool Poker::bettingRound() {
    bool isShowdown = false;
    int maxOpponentCanPay = 0;
    for (int i = 1; i < (int)players.size(); i++) {
        if (!players[i].isFolded) maxOpponentCanPay = max(maxOpponentCanPay, players[i].money);
    }

    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded) continue;

        if (i == 0) { // 플레이어 턴
            if (playerRef <= 0) continue;
            cout << "\n[1]콜 [2]하프 [3]올인 [4]다이 : ";
            int choice; cin >> choice;

            if (choice == 3) {
                int actualBet = min(playerRef, maxOpponentCanPay + currentBet);
                pot += actualBet; playerRef -= actualBet;
                currentBet = max(currentBet, actualBet); isShowdown = true;
                cout << " >> [올인] " << actualBet << "$ 베팅!" << endl;
            }
            else if (choice == 4) players[i].isFolded = true;
            else if (choice == 2) {
                int raise = (pot / 2) + currentBet;
                int act = min(playerRef, raise);
                playerRef -= act; pot += act; currentBet = act;
            }
            else {
                int pay = min(playerRef, currentBet);
                playerRef -= pay; pot += pay;
            }
        }
        else { // AI 턴
            if (players[i].money <= 0) continue;
            cout << players[i].name << " 베팅 중..."; Sleep(500);
            long long pwr = GetHandStrength(players[i].hand);
            int& aiMoney = players[i].money;

            if (currentBet >= aiMoney) {
                if (pwr >= 2500000LL || (rand() % 10 < 3)) {
                    pot += aiMoney; aiMoney = 0; isShowdown = true;
                    cout << "[올인 콜!]" << endl;
                }
                else { players[i].isFolded = true; cout << "[다이]" << endl; }
            }
            else {
                int pay = min(aiMoney, currentBet);
                aiMoney -= pay; pot += pay; cout << "[콜]" << endl;
            }
        }
    }
    return isShowdown;
}

// AI 파산 시 교체 로직
void Poker::CheckAndReplaceAI() {
    for (int i = 1; i < (int)players.size(); i++) {
        if (players[i].money <= 0) {
            // [연동] AI가 끌려가는 공통 연출 실행
            DragToMine(players[i].name, false);

            string newName = globalNamePool[rand() % globalNamePool.size()];
            players[i].name = newName;
            players[i].money = 100000;

            SetColor(11);
            cout << "\n [★] 빈자리에 새로운 참가자 [" << newName << "]가 입장했습니다." << endl;
            Sleep(1000);
        }
    }
}

void Poker::play() {
    system("cls");
    // [연동] 입장 시 플레이어 파산 체크
    if (CheckBankruptcy("당신", playerRef, 1, true)) return;

    SetColor(11);
    cout << "\n\n          +------------------------------------------+" << endl;
    cout << "          |          [ 포커 테이블 입장 ]            |" << endl;
    cout << "          +------------------------------------------+" << endl;
    cout << "          |    1. [ 초보 ] 판돈 :  1,000 $           |" << endl;
    cout << "          |    2. [ 일반 ] 판돈 :  5,000 $           |" << endl;
    cout << "          |    3. [ 고수 ] 판돈 : 10,000 $           |" << endl;
    cout << "          |    4. [ 메이저] 판돈 : 20,000 $          |" << endl;
    cout << "          +------------------------------------------+" << endl;
    SetColor(15);
    cout << "\n           선택 (1~4): ";
    int menu, baseBet; cin >> menu;
    if (menu == 1) baseBet = 1000; else if (menu == 3) baseBet = 10000;
    else if (menu == 4) baseBet = 20000; else baseBet = 5000;

    while (true) {
        // [연동] 매 판 시작 전 자산 체크 (입장컷)
        if (CheckBankruptcy("당신", playerRef, baseBet, true)) return;

        CheckAndReplaceAI();

        initDeck(); shuffleDeck(); pot = 0; currentBet = baseBet;
        for (int i = 0; i < (int)players.size(); i++) {
            int entryFee = baseBet / 2;
            if (i == 0) playerRef -= entryFee; else players[i].money -= entryFee;
            pot += entryFee;
            players[i].hand.clear(); players[i].isFolded = false;
        }

        int dIdx = 0;
        for (int i = 0; i < 2; i++) for (auto& p : players) p.hand.push_back(deck[dIdx++]);

        for (int r = 1; r <= 4; r++) {
            showTable(false);
            if (bettingRound()) {
                SetColor(12); cout << "\n [!] 올인 발생! 최종 카드를 공개합니다!" << endl; Sleep(1500);
                while (players[0].hand.size() < 5) {
                    for (auto& p : players) if (!p.isFolded) p.hand.push_back(deck[dIdx++]);
                }
                break;
            }
            if (r < 4) for (auto& p : players) if (!p.isFolded) p.hand.push_back(deck[dIdx++]);
        }

        showTable(true);
        int winIdx = -1; long long best = -1;
        for (int i = 0; i < (int)players.size(); i++) {
            if (players[i].isFolded) continue;
            long long s = GetHandStrength(players[i].hand);
            if (s > best) { best = s; winIdx = i; }
        }

        if (winIdx != -1) {
            SetColor(10);
            cout << "\n ★ 승자: " << players[winIdx].name << " ★" << endl;
            cout << " 완성 패: " << GetDetailedRankName(players[winIdx].hand) << endl;
            cout << " 획득 금액: " << pot << "$" << endl;
            if (winIdx == 0) playerRef += pot; else players[winIdx].money += pot;
        }

        // [연동] 판이 끝난 후 플레이어 파산 여부 최종 확인
        if (CheckBankruptcy("당신", playerRef, 1, true)) return;

        SetColor(15); cout << "\n [1]계속 [0]종료 : ";
        int c; cin >> c; if (c == 0) break;
    }
}

string Poker::rankToString(HandRank rank) { return ""; }