#include "IndianPoker.h"
#include "System.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <windows.h>
#include <conio.h>

using namespace std;

// 전역 또는 외부에서 관리되는 이름 풀 (AI 교체용)
vector<string> globalNamePool = {
    "민지", "학진", "재용", "라이덴", "서연", "규찬", "미토마", "이강인",
    "미쿠", "카인", "선바", "선우", "유현", "정우", "세현", "동현",
    "영환", "프리렌", "니노", "아루", "유린", "카프카", "호두", "감우",
    "응광", "클레", "부현", "은랑", "스파클", "나히다", "마비카", "닐루",
    "푸리나", "청작", "제레", "호날두", "니코", "아리마", "키타가와", "페른"
};

IndianPoker::IndianPoker(int& money) : playerRef(money), pot(0) {
    // 플레이어 추가 (실제 소지금을 참조)
    players.push_back({ "Player", {0, 0}, playerRef, false });

    // 초기 AI 2명 설정
    random_device rd;
    mt19937 g(rd());
    shuffle(globalNamePool.begin(), globalNamePool.end(), g);

    for (int i = 0; i < 2; i++) {
        players.push_back({ globalNamePool[i], {0, 0}, 100000, false });
    }
}

void IndianPoker::InitDeck() {
    deck.clear();
    for (int r = 2; r <= 10; r++) {
        for (int s = 0; s < 4; s++) deck.push_back({ s, r });
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
}

string IndianPoker::GetCardStr(IndianCard c) {
    string s;
    if (c.suit == 0) s = "♠";
    else if (c.suit == 1) s = "◆";
    else if (c.suit == 2) s = "♥";
    else s = "♣";
    return s + to_string(c.rank);
}

void IndianPoker::ShowTable(bool revealPlayerCard) {
    system("cls");
    SetColor(14);
    cout << "==========================================================" << endl;
    cout << "                [ 인 디 언  포 커 ]" << endl;
    cout << "==========================================================" << endl;
    SetColor(15);

    for (int i = 0; i < (int)players.size(); i++) {
        cout << " [" << players[i].name << "] : ";
        if (players[i].isFolded) {
            SetColor(8); cout << "기권(FOLD)";
        }
        else {
            if (i == 0 && !revealPlayerCard) {
                SetColor(11); cout << "?? (이마 확인 불가)";
            }
            else {
                SetColor(players[i].card.suit == 1 || players[i].card.suit == 2 ? 12 : 15);
                cout << GetCardStr(players[i].card);
            }
        }
        SetColor(15);
        // 플레이어는 소지금 playerRef와 연동, AI는 개별 money 출력
        cout << "  (잔고: " << (i == 0 ? playerRef : players[i].money) << "$)" << endl;
    }
    cout << "----------------------------------------------------------" << endl;
    cout << " 현재 판돈: " << pot << "$" << " | 남은 카드: " << (int)deck.size() << "장" << endl;
    cout << "----------------------------------------------------------" << endl;
}

int IndianPoker::EvaluateWinner() {
    int topIdx = -1;
    int maxRank = -1;
    for (int i = 0; i < (int)players.size(); i++) {
        if (players[i].isFolded) continue;
        if (players[i].card.rank > maxRank) {
            maxRank = players[i].card.rank;
            topIdx = i;
        }
    }
    return topIdx;
}

void IndianPoker::Play() {
    InitDeck();

    while (true) {
        // 1. 카드 소진 체크
        if (deck.size() < 3) {
            system("cls");
            SetColor(14); cout << "\n [ 카드 소진! 게임을 종료합니다 ]" << endl;
            (void)_getch(); return;
        }

        // 2. 플레이어 파산 체크 (자동 충전 없음)
        if (playerRef < 5000) {
            system("cls");
            SetColor(12);
            cout << "\n [!] 소지 자금이 부족합니다 (최소 5,000$ 필요)" << endl;
            cout << " [!] 카지노에서 쫓겨났습니다! 광산에 가서 돈을 벌어오세요." << endl;
            SetColor(15);
            (void)_getch();
            return; // 메인 화면으로 퇴장
        }

        // 3. AI 파산 및 교체 체크
        for (int i = 1; i < 3; i++) {
            if (players[i].money < 5000) {
                system("cls");
                SetColor(11);
                cout << "\n [!] " << players[i].name << "이(가) 파산하여 광산 노역소로 압송되었습니다." << endl;
                Sleep(1000);

                // 이름 풀에서 새로운 이름 선정
                string newName = globalNamePool[rand() % globalNamePool.size()];
                cout << " [!] 새로운 도박꾼 [" << newName << "]이(가) 판에 합류합니다!" << endl;

                players[i].name = newName;
                players[i].money = 100000; // 신입의 판돈
                SetColor(15);
                Sleep(1500);
            }
        }

        int currentMaxBet = 5000;
        int penaltyAmount = 10000;
        pot = 0;
        vector<int> roundBets(3, 0);
        bool playerGaveUp = false;

        // 4. 참가비 지불
        for (int i = 0; i < 3; i++) {
            if (i == 0) playerRef -= 5000;
            else players[i].money -= 5000;

            roundBets[i] = 5000;
            pot += 5000;
            players[i].card = deck.back(); deck.pop_back();
            players[i].isFolded = false;
        }

        // 5. 배팅 루프
        bool bettingInProgress = true;
        while (bettingInProgress) {
            int activePlayers = 0;
            for (auto& p : players) if (!p.isFolded) activePlayers++;
            if (activePlayers <= 1) break;

            for (int i = 0; i < 3; i++) {
                if (players[i].isFolded) continue;
                int callAmount = currentMaxBet - roundBets[i];
                ShowTable(false);

                if (i == 0) { // Player
                    cout << "\n >>> [ PLAYER TURN ] <<<" << endl;
                    cout << " [1] 콜 [2] 레이즈 [3] 다이 : ";
                    int choice; cin >> choice;

                    if (choice == 3) {
                        players[0].isFolded = true; playerGaveUp = true;
                        if (players[0].card.rank == 10) {
                            playerRef -= penaltyAmount;
                            cout << " !! 10 페널티 발생 !! (-10,000$)" << endl; Sleep(1000);
                        }
                        cout << "\n [ 기권하셨습니다. ]" << endl; Sleep(800);
                    }
                    else if (choice == 2) {
                        while (true) {
                            cout << " 추가 레이즈 금액 (내 잔고: " << playerRef << "$): ";
                            long long r; cin >> r;
                            if (r <= 0) { cout << " [!] 0보다 커야 합니다.\n"; continue; }
                            if (r > playerRef) r = playerRef;
                            if (r > 1000000) r = 1000000;

                            currentMaxBet += (int)r;
                            int gap = currentMaxBet - roundBets[0];
                            playerRef -= gap; pot += gap; roundBets[0] += gap;
                            break;
                        }
                    }
                    else { // 콜
                        playerRef -= callAmount; pot += callAmount; roundBets[0] += callAmount;
                    }
                }
                else { // AI
                    cout << "\n [" << players[i].name << "] 판단 중..."; Sleep(1000);
                    int myR = players[i].card.rank;
                    int maxVisible = 0;
                    for (int j = 0; j < 3; j++) if (i != j && !players[j].isFolded && players[j].card.rank > maxVisible) maxVisible = players[j].card.rank;

                    bool raised = false;
                    if (myR >= maxVisible && myR >= 7 && rand() % 10 < 4) {
                        int r = 10000 + (rand() % 5 * 5000);
                        currentMaxBet += r;
                        int gap = currentMaxBet - roundBets[i];
                        players[i].money -= gap; pot += gap; roundBets[i] += gap;
                        cout << " [" << r << "$ 레이즈!!]"; raised = true;
                    }

                    if (!raised) {
                        if (myR >= maxVisible || myR == 10 || (callAmount <= 20000 && rand() % 10 < 7)) {
                            players[i].money -= callAmount; pot += callAmount; roundBets[i] += callAmount;
                            cout << " [콜]";
                        }
                        else {
                            players[i].isFolded = true; cout << " [다이]";
                        }
                    }
                    Sleep(800);
                }
                int check = 0;
                for (auto& p : players) if (!p.isFolded) check++;
                if (check <= 1) break;
            }

            bool allMatched = true;
            for (int i = 0; i < 3; i++) if (!players[i].isFolded && roundBets[i] < currentMaxBet) allMatched = false;
            if (allMatched) bettingInProgress = false;
        }

        // 6. 라운드 결과
        ShowTable(true);
        int win = EvaluateWinner();
        if (win != -1) {
            SetColor(10); cout << "\n 승자: " << players[win].name << " (+" << pot << "$)" << endl;
            if (win == 0) playerRef += pot;
            else players[win].money += pot;
            SetColor(15);
        }

        cout << "\n [내 카드: " << GetCardStr(players[0].card) << "]" << endl;
        cout << " [아무 키나 누르면 다음 판 진행]";
        (void)_getch();
    }
}