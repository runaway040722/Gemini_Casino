#include <iostream>
#include "System.h"
#include "SlotMachine.h"
#include "Blackjack.h"
#include "EvenOdd.h"
#include "HorseRacing.h"
#include "Poker.h"
#include "UpDown.h"
#include "BombGame.h"
#include "Mine.h"  // ★ 광산 헤더 추가
#include <limits>

using namespace std;

void ShowMainMenu(int money) {
    system("cls");

    SetColor(14); // 금색
    cout << "  ##########################################################" << endl;
    cout << "  ##                                                      ##" << endl;
    cout << "  ##      @@@@    @@@@    @@@@    @@@@    @  @    @@      ##" << endl;
    cout << "  ##     @       @    @  @        @    @  @  @   @  @     ##" << endl;
    cout << "  ##     @       @@@@@@   @@@@    @    @  @  @   @  @     ##" << endl;
    cout << "  ##     @       @    @       @   @    @  @  @   @  @     ##" << endl;
    cout << "  ##      @@@@   @    @   @@@@    @@@@    @  @    @@      ##" << endl;
    cout << "  ##                                                      ##" << endl;
    cout << "  ##              [ 대 박 기 원  카 지 노 ]               ##" << endl;
    cout << "  ##########################################################" << endl;

    SetColor(11); // 청록색 구분선
    cout << "  ----------------------------------------------------------" << endl;
    SetColor(15); // 흰색
    cout << "      [ 현재 보유 자산 : ";
    SetColor(10); // 초록색
    cout << money << " 달러";
    SetColor(15);
    cout << " ]" << endl;
    SetColor(11);
    cout << "  ----------------------------------------------------------" << endl;

    SetColor(15);
    cout << "      (1)  777 슬롯 머신" << endl;
    cout << "      (2)  블랙잭 21" << endl;
    cout << "      (3)  홀짝 룰렛 " << endl;
    cout << "      (4)  경마" << endl;
    cout << "      (5)  포커 " << endl;
    cout << "      (6)  업다운 게임" << endl;
    cout << "      (7)  폭탄 게임" << endl;
    cout << endl;
    SetColor(12); // 종료
    cout << "      (0)  게임 종료" << endl;

    SetColor(11);
    cout << "  ----------------------------------------------------------" << endl;
    SetColor(15);
    cout << "      메뉴 선택 > ";
}

int main() {
    int money = 1000;
    int choice;

    while (true) {
        // 돈이 0원 이하인지 먼저 체크해서 광산으로 보냄
        if (money <= 0) {
            PlayMine(money); // Mine.cpp 실행 (탈출 시 money는 100이 됨)
            continue;        // 광산 탈출 후 다시 메인 메뉴로
        }

        ShowMainMenu(money);

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');
            continue;
        }

        switch (choice) {
        case 1: PlaySlotMachine(money); break;
        case 2: PlayBlackjack(money);   break;
        case 3: PlayEvenOdd(money);     break;
        case 4: PlayHorseRacing(money); break;
        case 5: {
            Poker pokerGame(money);
            pokerGame.play();
            money = pokerGame.getBalance();

            cout << "\n게임을 종료합니다. 메인으로 돌아가려면 아무 키나 누르세요.";
            cin.ignore();
            cin.get();
            break;
        }
        case 6: PlayUpDown(money); break;
        case 7: PlayBombGame(money); break;
        case 0:
            system("cls");
            SetColor(14);
            cout << "\n\n  오늘 운 좋으셨네요! 다음에 또 뵙겠습니다. \n\n" << endl;
            SetColor(15);
            return 0;
        default: break;
        }
    }
    return 0;
}