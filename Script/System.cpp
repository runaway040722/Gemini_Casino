#include "System.h"
#include <vector>
#include <string>
#include <windows.h>
#include <conio.h>
#include <iostream>

using namespace std;

// 이름 풀 (NPC용)
vector<string> globalNamePool = {
    // [기존 리스트 및 이전 추가분]
    "민지", "학진", "재용", "라이덴", "서연", "규찬", "미토마", "이강인",
    "미쿠", "카인", "선바", "선우", "유현", "정우", "세현", "동현",
    "영환", "프리렌", "니노", "아루", "유린", "카프카", "호두", "감우",
    "응광", "클레", "부현", "은랑", "스파클", "나히다", "마비카", "닐루",
    "푸리나", "청작", "제레", "호날두", "오라일리", "아리마 카나", "키타가와 마린", "페른",
    "바레사", "리넷", "이네파", "나비아", "클로린드", "파루잔", "아를레키노", "신학", "모나", "스커크",
    "피슬", "실로닌", "파스카", "네페르", "바르카", "진", "야란", "데히야", "종려", "벤티",
    "라이오슬리", "느비예트", "사이노", "치치", "라피", "아니스", "네온", "바이퍼", "쟈칼", "디젤",
    "신데렐라", "베히모스", "말라니", "코코미", "카미사토 아야카", "카미사토 아야토", "라우마", "야호다", "아인", "로산나",
    "마키마", "레제", "히메노", "에이드", "프리바티", "아르카나", "헬름", "마스트", "크라운", "모더니아",
    "도로시", "라푼젤", "레이블", "일레그", "나유타", "그레이브", "라플라스", "스노우 화이트", "레드후드", "블랑",
    "슈엔", "앤더슨", "버닝엄", "미소노 미카", "하야세 유우카", "앵커", "아로나", "리쿠하치마 아루", "반디", "블레이드",
    "선데이", "로빈", "헤르타", "펄", "어벤츄린", "토파즈", "제이드", "마이데이", "아낙사", "사이퍼",
    "아글라이아", "키레네", "파이논", "트리비", "개척자", "여행자", "히아킨", "에버나이트", "경류", "달리아",
    "스파키", "아케론", "애쉬베일", "카스토리스", "곽향", "케리드라", "히실렌스",
    "정운", "데브라위너", "살라", "로드리", "음바페", "세이버", "블랙스완", "단항", "창민", "설의",
    "키리후지 나기사", "록시 미굴디아", "츠카츠키 리오", "야나미 안나", "야마다 안나", "나가토로 하야세",
    "이치노세 아스나", "미카모 네루", "루니", "렘", "람", "파워", "포치타", "하야카와 아키",
    "야마다 료", "키타 이쿠요", "이지치 니지카", "이지치 세이카", "고토 히토리", "아와 스바루",
    "우타즈미 사쿠라코", "카와라기 모모카", "이세리 니나", "루파", "에비즈카 토모", "일레이나",
    "미타카 아사", "요루", "키가", "나나미 치아키", "류우게 키사키", "힘멜", "유리조노 세이아",
    "콜롬비나", "소"
};

void gotoxy(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void FlushBuffer() {
    while (_kbhit()) _getch();
    cin.clear();
}

void ClearBuffer() {
    while (_kbhit()) _getch();
    cin.clear();
    if (cin.rdbuf()->in_avail()) {
        cin.ignore(1000, '\n');
    }
    SetColor(14);
    cout << "\n [아무 키나 누르면 메뉴로 돌아갑니다]";
    _getch();
}

// 이게 네가 원한 깔끔한 베팅 함수 전체 로직이야
int GetBetAmount(int currentMoney) {
    int bet = 0;
    while (true) {
        system("cls"); // 베팅 시작할 때 화면 깔끔하게 정리

        // 상단 헤더 느낌
        SetColor(14);
        cout << "===============================================" << endl;
        cout << "                [ 베팅 단계 ]" << endl;
        cout << "===============================================" << endl;

        SetColor(15);
        cout << "\n [ 보유 자산: " << currentMoney << "$ ]" << endl;

        SetColor(14);
        cout << " [ 베팅 금액 투입 ] (올인: a) >> ";

        string input;
        cin >> input;

        if (input == "a" || input == "A") {
            if (currentMoney <= 0) {
                SetColor(12);
                cout << "\n >> 잔액이 부족하여 올인을 할 수 없습니다!" << endl;
                Sleep(1000);
                continue;
            }
            bet = currentMoney;
            SetColor(11);
            cout << "\n >>> 올인 완료!! 모든 자산을 걸었습니다!" << endl;
        }
        else {
            try {
                bet = stoi(input);
            }
            catch (...) {
                SetColor(12);
                cout << "\n >> 숫자 또는 'a'를 정확히 입력해 주세요." << endl;
                Sleep(1000);
                continue;
            }

            if (bet > currentMoney) {
                SetColor(12);
                cout << "\n >> 자산이 부족합니다! (현재 최대 베팅: " << currentMoney << "$)" << endl;
                Sleep(1000);
                continue;
            }
            if (bet <= 0) {
                SetColor(12);
                cout << "\n >> 최소 1$ 이상은 투입해야 합니다." << endl;
                Sleep(1000);
                continue;
            }
        }

        // 베팅 성공 피드백
        SetColor(10);
        cout << " >> 베팅 성공! 잠시 후 게임이 시작됩니다...";
        Sleep(800);

        system("cls"); // ★ 베팅 끝나면 화면 싹 지우고 게임판으로 넘김
        break;
    }
    return bet;
}

void PrintResult(int& money, int bet, int winAmount) {
    int profit = winAmount - bet; // 순수익 계산

    cout << "\n-----------------------------------------------" << endl;

    if (winAmount > 0) {
        money += winAmount; // 자산 업데이트

        SetColor(10); // 초록색
        cout << " [결과] 당첨! 축하합니다." << endl;
        SetColor(15);
        cout << "  - 총 당첨금 : +$" << winAmount << endl;

        if (profit > 0) {
            SetColor(11); // 하늘색
            cout << "  - 실제 수익 : +$" << profit << endl;
        }
        else if (profit == 0) {
            SetColor(15);
            cout << "  - 실제 수익 : +$0 (본전치기!)" << endl;
        }
        else {
            SetColor(12); // 빨간색 (딴 돈보다 건 돈이 클 때)
            cout << "  - 실제 수익 : -$" << abs(profit) << " (손해 발생)" << endl;
        }
    }
    else {
        // winAmount가 0인 경우 (완전 꽝)
        SetColor(12);
        cout << " [결과] 꽝! 베팅금을 모두 잃었습니다." << endl;
        cout << "  - 손실 금액 : -$" << bet << endl;
    }

    SetColor(14); // 노란색
    cout << "\n [ 현재 자산 : $" << money << " ]" << endl;
    SetColor(15);
    cout << "-----------------------------------------------" << endl;
}
