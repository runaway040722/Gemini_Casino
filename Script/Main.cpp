#include <iostream>
#include "System.h"
#include "SlotMachine.h"
#include "Blackjack.h"
#include "EvenOdd.h"
#include "HorseRacing.h"
#include "Poker.h"
#include "UpDown.h"
#include "BombGame.h"
#include "NumberBaseball.h"
#include "IndianPoker.h"
#include "PopUpGame.h"
#include "Mine.h"
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
    cout << "      (4)  경마 게임" << endl;
    cout << "      (5)  4인 포커" << endl;
    cout << "      (6)  업다운 게임" << endl;
    cout << "      (7)  폭탄 돌리기" << endl;
    cout << "      (8)  숫자 야구" << endl;
    cout << "      (9)  인디언 포커" << endl;
    cout << "      (10) 통 아저씨" << endl;

    cout << endl;
    SetColor(12); // 종료
    cout << "      (0)  게임 종료" << endl;

    SetColor(11);
    cout << "  ----------------------------------------------------------" << endl;
    SetColor(15);
    cout << "      메뉴 선택 > ";
}

int main() {
    int money = 100000; // 초기 자본
    int choice;

    while (true) {
        // 자산 소진 시 광산으로 강제 이동
        if (money <= 0) {
            PlayMine(money);
            continue;
        }

        ShowMainMenu(money);

        if (!(cin >> choice)) {
            FlushBuffer();
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
            break;
        }
        case 6: PlayUpDown(money); break;
        case 7: PlayBombGame(money); break;
        case 8: PlayNumberBaseball(money); break;
        case 9: {
            IndianPoker iPoker(money);
            iPoker.Play();
            break;
        }
        case 10: {
            PopUpGame popGame(money); // playerMoney는 메인에서 관리하는 변수명에 맞게 수정
            popGame.play();
            break;
        }
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