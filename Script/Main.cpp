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
#include "TexasHoldem.h"
#include "RussianRoulette.h"
#include "Mine.h"
#include <limits>
#include <climits> // INT_MAX 사용을 위해 필요

using namespace std;

void ShowMainMenu(int money) {
    system("cls");
    SetColor(14); // 금색
    cout << "  ##########################################################" << endl;
    cout << "  ##                                                      ##" << endl;
    cout << "  ##      @@@@@   @@@    @@@@@  @@@@@  @   @   @@@        ##" << endl;
    cout << "  ##     @       @   @  @         @    @@  @  @   @       ##" << endl;
    cout << "  ##     @       @@@@@   @@@@     @    @ @ @  @   @       ##" << endl;
    cout << "  ##     @       @   @       @    @    @  @@  @   @       ##" << endl;
    cout << "  ##      @@@@@  @   @  @@@@@   @@@@@  @   @   @@@        ##" << endl;
    cout << "  ##                                                      ##" << endl;
    cout << "  ##             [ 대 박 기 원  카 지 노 ]                ##" << endl;
    cout << "  ##########################################################" << endl;

    SetColor(11); // 청록색
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
    cout << "      (11) 텍사스 홀덤" << endl;
    cout << "      (12) 러시안 룰렛" << endl; 

    cout << endl;
    SetColor(12); // 빨간색 (종료)
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

        // 1. 메뉴 입력 시도
        if (!(cin >> choice)) {
            ClearBuffer();
            continue;
        }

        // 2. 입력 후 버퍼에 남은 '엔터'키를 여기서 미리 제거 (중요!)
        // 이 코드가 있어야 각 게임 진입 시 첫 입력이 씹히지 않습니다.
        cin.ignore(INT_MAX, '\n');

                switch (choice) {
                case 1:
                    system("cls"); // 화면을 싹 비우고 게임 시작
                    PlaySlotMachine(money);
                    break;

                case 2:
                    system("cls");
                    PlayBlackjack(money);
                    break;

                case 3:
                    system("cls");
                    PlayEvenOdd(money);
                    break;

                case 4:
                    system("cls");
                    PlayHorseRacing(money);
                    break;

                case 5: {
                    system("cls");
                    Poker pokerGame(money);
                    pokerGame.play();
                    break;
                }

                case 6:
                    system("cls");
                    PlayUpDown(money);
                    break;

                case 7:
                    system("cls");
                    PlayBombGame(money);
                    break;

                case 8:
                    system("cls");
                    PlayNumberBaseball(money);
                    break;

                case 9: {
                    system("cls");
                    IndianPoker iPoker(money);
                    iPoker.Play();
                    break;
                }

                case 10: {
                    system("cls");
                    PopUpGame popGame(money);
                    popGame.play();
                    break;
                }

                case 11: {
                    system("cls");
                    TexasHoldem holdem(money);
                    holdem.Play();
                    break;
                }
                case 12: { // 추가
                    PlayRussianRoulette(money);
                    break;
                }
        case 99: { // 치트키
            money += 1000000;
            break;
        }
        case 0:
            system("cls");
            SetColor(14);
            cout << "\n\n  오늘 운 좋으셨네요! 다음에 또 뵙겠습니다. \n\n" << endl;
            SetColor(15);
            return 0;
        default:
            cout << "\n 잘못된 메뉴 선택입니다.";
            Sleep(500);
            break;
        }
    }
    return 0;
}