#include <iostream>
#include "System.h"
#include "SlotMachine.h"
#include "Blackjack.h"
#include "EvenOdd.h"
#include "HorseRacing.h"
#include <limits>

using namespace std;

void ShowMainMenu(int money) {
    system("cls");

    // 심플하고 강렬한 메인 박스
    SetColor(14); // 금색
    cout << "  ##########################################################" << endl;
    cout << "  ##                                                      ##" << endl;
    cout << "  ##      @@@@    @@@@    @@@@    @@@@    @  @    @@      ##" << endl;
    cout << "  ##     @       @    @  @        @    @  @  @   @  @     ##" << endl;
    cout << "  ##     @       @@@@@@   @@@@    @    @  @  @   @  @     ##" << endl;
    cout << "  ##     @       @    @       @   @    @  @  @   @  @     ##" << endl;
    cout << "  ##      @@@@   @    @   @@@@    @@@@    @  @    @@      ##" << endl;
    cout << "  ##                                                      ##" << endl;
    cout << "  ##                [ 대 박 기 원  카 지 노 ]             ##" << endl;
    cout << "  ##########################################################" << endl;

    SetColor(11); // 청록색 구분선
    cout << "  ----------------------------------------------------------" << endl;
    SetColor(15); // 흰색
    cout << "     [ 현재 보유 자산 : ";
    SetColor(10); // 초록색 (돈은 역시 초록색)
    cout << money << " 달러";
    SetColor(15);
    cout << " ]" << endl;
    SetColor(11);
    cout << "  ----------------------------------------------------------" << endl;

    SetColor(15);
    cout << "     (1)  럭키 슬롯 머신" << endl;
    cout << "     (2)  블랙잭 21" << endl;
    cout << "     (3)  홀짝 맞추기" << endl;
    cout << "     (4)  몬스터 경마" << endl;
    cout << endl;
    SetColor(12); // 종료는 빨간색으로 포인트
    cout << "     (0)  게임 종료" << endl;

    SetColor(11);
    cout << "  ----------------------------------------------------------" << endl;
    SetColor(15);
    cout << "     메뉴 선택 > ";
}

int main() {
    int money = 1000;
    int choice;

    while (true) {
        ShowMainMenu(money);

        // 숫자 대신 문자 입력 시 에러 방지
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
        case 0:
            system("cls");
            SetColor(14);
            cout << "\n\n  오늘 운 좋으셨네요! 다음에 또 뵙겠습니다. \n\n" << endl;
            SetColor(15);
            return 0;
        default: break;
        }

        // 파산 체크
        if (money <= 0) {
            system("cls");
            SetColor(12);
            cout << "\n\n  [ 게임 오버 ]" << endl;
            cout << "  가진 돈을 모두 잃으셨습니다. 카지노에서 쫓겨납니다." << endl;
            SetColor(15);
            ClearBuffer();
            break;
        }
    }
    return 0;
}