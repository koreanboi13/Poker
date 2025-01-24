#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#include <iostream>

struct bot_settings
{
    float checkLow, checkHigh, callLow, callHigh, raiseLow, raiseHigh, allinLow;
    short blef;
};
struct hand
{
    short first, second;
};

struct table
{
    short flop1, flop2, flop3, turn, river;
};
float win_ch = 0;
float draw_chanse = 0;

struct game
{
    short baton;
    int bank;
    short restart;
    int bot_balance, user_balance;
    int bot_bet, user_bet;
    float user_chance;
    float bot_chance;
    float win_chance;
};
struct game_bot_vs_bot
{
    short baton;
    int bank;
    short restart;
    int bot1_balance, bot2_balance;
    int bot1_bet, bot2_bet;
    float bot1_chance;
    float bot2_chance;
    float bot1_win_chance;
    float bot2_win_chance;
};
game Game; //Datagame datagame
game_bot_vs_bot Game2;
hand user;
hand bot;
table tb;
bot_settings bs;
bot_settings Danya = { 0.1,0.4,0.2,0.4,0.4,0.9,0.9,10 };
bot_settings Kirill = { 0.2,0.5,0.25,0.5,0.5,0.85,0.85,5 };
short used_cards[53] = { 0 };
short used_cards_copy[53] = { 0 };
void bot_raise();
void bot_raise2(short key);
void bot_call();
void bot_call2(short key);
void bot_all_in();
void bot_all_in2(short key);
void bot_check();
void bot_check2(short key);
void action_bot(short key);
void action();
void bot_fold();
void bot_fold2(short key);
short ans;
short ch_bot;

int HR[32487834];

short card_generation() {
start:
    short card = rand() % 52 + 1;
    while (true)
    {
        if (used_cards[card] != 0)
            goto start;
        if (used_cards[card] == 0)
        {
            used_cards[card]++;
            return card;
        }
    }
}

int LookupHand(short* pCards)
{
    int p = HR[53 + *pCards++];
    p = HR[p + *pCards++];
    p = HR[p + *pCards++];
    p = HR[p + *pCards++];
    p = HR[p + *pCards++];
    p = HR[p + *pCards++];
    return HR[p + *pCards++];
}


void print_hand(hand ghand)
{
  
    int mast[2];
    int dost[2]; 
    fputs("\n", stdout);
    if (ghand.first == 52)
        mast[0] = 3;
    else
    {
       
        if (ghand.first >= 39 && ghand.first <= 51)
            mast[0] = 6;
        else if (ghand.first >= 26 && ghand.first <= 38)
            mast[0] = 5;
        else if (ghand.first >= 13 && ghand.first <= 25)
            mast[0] = 4;
        else if (ghand.first >= 0 && ghand.first <= 12)
            mast[0] = 3;
    }
    if (ghand.second == 52)
        mast[1] = 3;

    else
    {
        if (ghand.second >= 39 && ghand.second <= 51)
            mast[1] = 6;
        else if (ghand.second >= 26 && ghand.second <= 38)
            mast[1] = 5;
        else if (ghand.second >= 13 && ghand.second <= 25)
            mast[1] = 4;
        else if (ghand.second >= 0 && ghand.second <= 12)
            mast[1] = 3;
    }
    dost[0] = ghand.first % 13 + 2;
    dost[1] = ghand.second % 13 + 2;
    for (int i = 0; i < 2; ++i) {
        fputs(" -----", stdout);
        fputs(" ", stdout);

    }
    fputs("\n", stdout);

    for (int i = 0; i < 2; ++i) {
        printf("|%c    |", mast[i]);

        fputs("", stdout);
    }
    fputs("\n", stdout);
    for (int i = 0; i < 2; ++i) {
        switch (dost[i])
        {
        case 10:
            printf("| %d  |", dost[i]);
            break;
        case 11:
            fputs("|  J  |", stdout);
            break;
        case 12:
            fputs("|  Q  |", stdout);
            break;
        case 13:
            fputs("|  K  |", stdout);
            break;
        case 14:
            fputs("|  T  |", stdout);
            break;
        default:
            printf("|  %d  |", dost[i]);
            break;
        }
        fputs("", stdout);
    }
    fputs("\n", stdout);
    for (int i = 0; i < 2; ++i) {
        printf("|    %c|", mast[i]);

        fputs("", stdout);
    }
    fputs("\n", stdout);
    for (int i = 0; i < 2; ++i) {
        fputs(" -----", stdout);
        fputs(" ", stdout);
    }
    fputs("\n", stdout);


}

void print_table(table ttable, int size)
{
    int mast[5];
    int dost[5];
    fputs("Карты на столе\n", stdout);
    for (int i = 0; i < 5; ++i)
    {
        if (mast[i] == 52)
            mast[i] = 0;
    }

    if (ttable.flop1 == 52)
        ttable.flop1 = 0;
    if (ttable.flop2 == 52)
        ttable.flop2 = 0;
    if (ttable.flop3 == 52)
        ttable.flop3 = 0;
    if (ttable.river == 52)
        ttable.river = 0;
    if (ttable.turn == 52)
        ttable.turn = 0;

    if (ttable.flop1 >= 39 && ttable.flop1 <= 51)
        mast[0] = 6;
    else if (ttable.flop1 >= 26 && ttable.flop1 <= 38)
        mast[0] = 5;
    else if (ttable.flop1 >= 13 && ttable.flop1 <= 25)
        mast[0] = 4;
    else if (ttable.flop1 >= 0 && ttable.flop1 <= 12)
        mast[0] = 3;
    else if (ttable.flop1 == 52)
        mast[0] = 3;


    if (ttable.flop2 >= 39 && ttable.flop2 <= 51)
        mast[1] = 6;
    else if (ttable.flop2 >= 26 && ttable.flop2 <= 38)
        mast[1] = 5;
    else if (ttable.flop2 >= 13 && ttable.flop2 <= 25)
        mast[1] = 4;
    else if (ttable.flop2 >= 0 && ttable.flop2 <= 12)
        mast[1] = 3;
    else if (ttable.flop2 == 52)
        mast[0] = 3;


    if (ttable.flop3 >= 39 && ttable.flop3 <= 51)
        mast[2] = 6;
    else if (ttable.flop3 >= 26 && ttable.flop3 <= 38)
        mast[2] = 5;
    else if (ttable.flop3 >= 13 && ttable.flop3 <= 25)
        mast[2] = 4;
    else if (ttable.flop3 >= 0 && ttable.flop3 <= 12)
        mast[2] = 3;
    else if (ttable.flop3 == 52)
        mast[0] = 3;


    if (ttable.turn >= 39 && ttable.turn <= 51)
        mast[3] = 6;
    else if (ttable.turn >= 26 && ttable.turn <= 38)
        mast[3] = 5;
    else if (ttable.turn >= 13 && ttable.turn <= 25)
        mast[3] = 4;
    else if (ttable.turn >= 0 && ttable.turn <= 12)
        mast[3] = 3;
    else if (ttable.turn == 52)
        mast[0] = 3;
    if (ttable.river >= 39 && ttable.river <= 51)
        mast[4] = 6;
    else if (ttable.river >= 26 && ttable.river <= 38)
        mast[4] = 5;
    else if (ttable.river >= 13 && ttable.river <= 25)
        mast[4] = 4;
    else if (ttable.river >= 0 && ttable.river <= 12)
        mast[4] = 3;
    else if (ttable.river == 52)
        mast[0] = 3;
    dost[0] = ttable.flop1 % 13 + 2;
    dost[1] = ttable.flop2 % 13 + 2;
    dost[2] = ttable.flop3 % 13 + 2;
    dost[3] = ttable.turn % 13 + 2;
    dost[4] = ttable.river % 13 + 2;
    for (int i = 0; i < size; ++i) {
        fputs(" -----", stdout);
        fputs(" ", stdout);
    }
    fputs("\n", stdout);

    for (int i = 0; i < size; ++i) {
        printf("|%c    |", mast[i]);

        fputs("", stdout);
    }
    fputs("\n", stdout);
    for (int i = 0; i < size; ++i) {
        switch (dost[i])
        {
        case 10:
            printf("| %d  |", dost[i]);
            break;
        case 11:
            fputs("|  J  |", stdout);
            break;
        case 12:
            fputs("|  Q  |", stdout);
            break;
        case 13:
            fputs("|  K  |", stdout);
            break;
        case 14:
            fputs("|  T  |", stdout);
            break;
        default:
            printf("|  %d  |", dost[i]);
            break;
        }
        fputs("", stdout);
    }
    fputs("\n", stdout);
    for (int i = 0; i < size; ++i) {
        printf("|    %c|", mast[i]);

        fputs("", stdout);
    }
    fputs("\n", stdout);
    for (int i = 0; i < size; ++i) {
        fputs(" -----", stdout);
        fputs(" ", stdout);
    }
    fputs("\n", stdout);
}
void print_info()
{
    printf("Ставка Бота: %d Банк Бота: %d\n", Game.bot_bet, Game.bot_balance);
    printf("Ваша ставка: %d Ваш банк: %d\n", Game.user_bet, Game.user_balance);
    printf("Общий банк: %d\n", Game.bank);
    fputs("\n", stdout);
}

void blind()
{
    if (Game.baton)
    {

        if (Game.user_balance < 50 || Game.bot_balance < 50)
        {
            Game.user_bet += fmin(Game.user_balance, Game.bot_balance);
            Game.bot_bet += fmin(Game.user_balance, Game.bot_balance);

            Game.bot_balance = 0;
            Game.user_balance -= Game.bot_bet;
            Game.bank += 2 * Game.bot_bet;
        }
        else if (Game.user_balance < 100)
        {
            Game.bot_bet += 50;
            Game.user_bet += Game.user_balance;
            Game.bot_balance -= 50;
            Game.user_balance = 0;
            Game.bank += (Game.user_bet + Game.bot_bet);
        }
        else
        {
            Game.user_bet += 100;
            Game.bot_bet += 50;
            Game.user_balance -= 100;
            Game.user_balance -= 50;
            Game.bank += (Game.user_bet + Game.bot_bet);
        }
        Game.baton = 0;

    }
    else
    {
        if (Game.user_balance < 50 || Game.bot_balance < 50)
        {
            Game.user_bet += fmin(Game.user_balance, Game.bot_balance);
            Game.bot_bet += fmin(Game.user_balance, Game.bot_balance);
            Game.user_balance = 0;
            Game.bot_balance -= Game.user_bet;
            Game.bank += 2 * Game.user_bet;
        }
        else if (Game.bot_balance < 100)
        {
            Game.user_bet += 50;
            Game.bot_bet += Game.bot_balance;
            Game.bot_balance = 0;
            Game.user_balance -= 50;
            Game.bank += (Game.user_bet + Game.bot_bet);
        }
        else
        {
            Game.user_bet += 50;
            Game.bot_bet += 100;
            Game.user_balance -= 50;
            Game.bot_balance -= 100;
            Game.bank += (Game.user_bet + Game.bot_bet);
        }
        Game.baton = 1;
    }
}

void blind_bots()
{
    if (Game2.baton)
    {
        if (Game2.bot2_balance < 50 || Game2.bot1_balance < 50)
        {
            Game2.bot2_bet += fmin(Game2.bot2_balance, Game2.bot1_balance);
            Game2.bot1_bet += fmin(Game2.bot2_balance, Game2.bot1_balance);

            Game2.bot1_balance -= Game2.bot1_bet;
            Game2.bot2_balance -= Game2.bot1_bet;
            Game2.bank += 2 * Game2.bot1_bet;
        }

        else if (Game2.bot1_balance < 100)
        {
            Game2.bot2_bet += 50;
            Game2.bot2_balance -= 50;
            Game2.bot1_bet += Game2.bot1_balance;
            Game2.bot1_balance = 0;
            Game2.bank += (Game2.bot2_bet + Game2.bot1_bet);
        }

        else
        {
            Game2.bot1_bet += 100;
            Game2.bot2_bet += 50;
            Game2.bot1_balance -= 100;
            Game2.bot2_balance -= 50;
            Game2.bank += (Game2.bot2_bet + Game2.bot1_bet);
        }
        Game2.baton = 0;

    }
    else
    {
        if (Game2.bot2_balance < 50 || Game2.bot1_balance < 50)
        {
            Game2.bot2_bet += fmin(Game2.bot2_balance, Game2.bot1_balance);
            Game2.bot1_bet += fmin(Game2.bot2_balance, Game2.bot1_balance);
            Game2.bot1_balance = 0;
            Game2.bot2_balance -= Game2.bot1_bet;
            Game2.bank += 2 * Game2.bot1_bet;
        }


        else if (Game2.bot2_balance < 100)
        {
            Game2.bot1_bet += 50;
            Game2.bot2_bet += Game2.bot2_balance;
            Game2.bot2_balance = 0;
            Game2.bot1_balance -= 50;
            Game2.bank += (Game2.bot2_bet + Game2.bot1_bet);
        }

        else
        {
            Game2.bot1_bet += 50;
            Game2.bot2_bet += 100;
            Game2.bot1_balance -= 50;
            Game2.bot2_balance -= 100;
            Game2.bank += (Game2.bot2_bet + Game2.bot1_bet);
        }
        Game2.baton = 1;
    }
}
void fold()
{
    Game.bot_balance += Game.bank;
    Game.bank = 0;
    Game.user_bet = 0;
    Game.bot_bet = 0;
    Game.restart = 1;
    print_info();
}

void call()
{
    if (Game.user_balance <= (Game.bot_bet - Game.user_bet))
    {
        Game.bot_balance += Game.bot_bet - Game.user_bet;
        Game.bank -= Game.bot_bet - Game.user_bet;
        Game.bot_bet = Game.user_bet;
        Game.bot_bet += Game.user_balance;
        Game.bot_balance -= Game.user_balance;
        Game.user_bet += Game.user_balance;
        Game.bank += Game.user_balance << 1;
        Game.user_balance = 0;
        print_info();
    }
    else
    {
        Game.user_balance -= (Game.bot_bet - Game.user_bet);
        Game.bank += (Game.bot_bet - Game.user_bet);
        Game.user_bet += (Game.bot_bet - Game.user_bet);
        print_info();
    }
    Game.restart = 0;
}
void bet_player()
{
    int  bet;
    int  res;
    if (Game.user_balance != 0 && Game.user_balance >= (Game.bot_bet - Game.user_bet)) {
        do {
            fputs("Сделайте ставку: ", stdout);
            res = scanf("%d", &bet);
            while (getchar() != '\n');
            if (res == 1 && bet > 0 && bet >= (Game.bot_bet - Game.user_bet) && bet <= Game.user_balance) {
                if (bet > Game.bot_balance + Game.bot_bet - Game.user_bet)
                    bet = Game.bot_balance + Game.bot_bet - Game.user_bet;


            }
            else fputs("Ошибка!\n", stdout);
        } while (res != 1 || bet <= 0 || bet < (Game.bot_bet - Game.user_bet) || bet > Game.user_balance);
        Game.bank += bet;
        Game.user_bet += bet;
        Game.user_balance -= bet;
    }
    else fputs("Ошибка! Ваш баланс не позволяет выполнить это действие!!!!!!!!!!!!\n", stdout);
}

void raise()
{
    bet_player();
    print_info();
    action_bot(ch_bot);
}


void cheсk(short key)
{
    Game.restart = 0;
    print_info();
    if (Game.baton)
        action_bot(ch_bot);
}

void all_in()
{
    int raz = Game.user_bet;
    if (Game.user_balance <= Game.bot_balance + Game.bot_bet - Game.user_bet)
    {
        Game.user_bet += Game.user_balance;
        Game.bank += Game.user_balance;
        Game.user_balance = 0;
        print_info();
        action_bot(ch_bot);
    }
    else
    {
        Game.bank += Game.bot_balance + Game.bot_bet - Game.user_bet;
        Game.user_bet += Game.bot_balance + Game.bot_bet - Game.user_bet;
        Game.user_balance -= Game.bot_balance + Game.bot_bet - raz;
        print_info();
        action_bot(ch_bot);
    }
}

void bot_check()
{
    fputs("\n Бот Чекает!\n", stdout);
    print_info();
}

void bot_raise()
{
    fputs("\n Бот Поднимает ставку!\n", stdout);
    int bet;
    float procent = ((rand() % 30) + 10) / 100.0;
    bet = procent * Game.bank;
    int  difference = Game.user_bet - Game.bot_bet;
    if (Game.user_balance == 0)  bet = difference;
    else {
        while (bet < difference) bet += procent * Game.bank;
        if (bet > Game.bot_balance) bet = Game.bot_balance;
    }

    Game.bank += bet;
    Game.bot_bet += bet;
    Game.bot_balance -= bet;
    if (Game.bot_bet < Game.user_bet) {
        Game.bank -= Game.user_bet - Game.bot_bet;
        Game.user_bet = Game.bot_bet;
    }
    action();
}

void bot_all_in()
{
    fputs("\nШОК!!! Бот ставит все свои деньги!!!\n", stdout);
    Game.bank += Game.bot_balance;
    Game.bot_bet += Game.bot_balance;
    Game.bot_balance = 0;
    action();
}

void bot_call()
{
    fputs("\n Бот колит!\n", stdout);

    if ((Game.user_bet - Game.bot_bet) <= Game.bot_balance) {
        Game.bot_balance -= (Game.user_bet - Game.bot_bet);
        Game.bank += (Game.user_bet - Game.bot_bet);
        Game.bot_bet += (Game.user_bet - Game.bot_bet);
        print_info();
    }
    else {
        Game.user_balance += (Game.user_bet - Game.bot_bet);
        Game.bank -= (Game.user_bet - Game.bot_bet);

        Game.user_bet = Game.bot_bet;
        Game.user_bet += Game.bot_balance;
        Game.user_balance -= Game.bot_balance;
        Game.bot_bet += Game.bot_balance;
        Game.bank += Game.bot_balance << 1;
        Game.bot_balance = 0;
        print_info();
    }
    Game.restart = 0;
}

void bot_fold()
{
    fputs("\n Бот Фолдит!!! Это победа!!\n", stdout);
    Game.restart = 1;
    Game.user_balance += Game.bank;
    Game.bot_bet = 0;
    Game.user_bet = 0;
    Game.bank = 0;
    print_info();
}

void bot_bet()
{
    fputs("\nВремя ставить Боту гигигага!\n", stdout);
    int bet;
    bet = Game.win_chance * Game.bank;
    int  raz = Game.user_bet - Game.bot_bet;
    if (Game.user_balance == 0)
        bet = raz;
    else
    {
        while (bet < raz)
            bet += Game.win_chance * Game.bank;
        if (bet > Game.bot_balance)
            bet = Game.bot_balance;
    }


    if (Game.bot_bet < Game.user_bet)
    {
        Game.bank -= Game.user_bet - Game.bot_bet;
        Game.user_bet = Game.bot_bet;
    }
    Game.bank += bet;
    Game.bot_bet += bet;
    Game.bot_balance -= bet;
    print_info();
    action();
}

void action_bot(short key)
{
    if (key == 1) {
        if (Game.bot_balance != 0 && Game.user_balance != 0 || Game.bot_bet != Game.user_bet)
        {
            if (Game.bot_bet < Game.user_bet)
            {
                if ((Game.win_chance > Danya.raiseLow && Game.win_chance < Danya.raiseHigh) || (rand() % 100) < Danya.blef)
                {
                    bot_raise();

                }
                else if ((Game.win_chance < Danya.callHigh && Game.win_chance > Danya.callLow) || (rand() % 100) < Danya.blef)
                {
                    bot_call();

                }
                else if ((Game.win_chance > Danya.allinLow) || (rand() % 100) < Danya.blef)
                {
                    bot_all_in();

                }
                else
                    bot_fold();


            }
            else if ((Game.bot_bet == Game.user_bet) || (rand() % 100) < Danya.blef)
            {
                if ((Game.win_chance > Danya.allinLow) || (rand() % 100) < Danya.blef)
                    bot_all_in();
                else if (Game.win_chance > Danya.raiseLow && Game.win_chance < Danya.raiseHigh)
                    bot_raise();
                else
                    bot_check();
            }
        }
    }
    if (key == 2)
    {
        if ((Game.bot_balance != 0 && Game.user_balance != 0) || Game.bot_bet != Game.user_bet)
        {
            if (Game.bot_bet < Game.user_bet)
            {
                if ((Game.win_chance > Kirill.raiseLow && Game.win_chance < Kirill.raiseHigh) || (rand() % 100) < Kirill.blef)
                    bot_raise();
                else if ((Game.win_chance < Kirill.callHigh && Game.win_chance > Kirill.callLow) || (rand() % 100) < Kirill.blef)
                    bot_call();
                else if (Game.win_chance > Kirill.allinLow || (rand() % 100) < Kirill.blef)
                    bot_all_in();
            }
            else if (Game.bot_bet == Game.user_bet || (rand() % 100) < Kirill.blef)
            {
                if (Game.win_chance > Kirill.allinLow || (rand() % 100) < Kirill.blef)
                    bot_all_in();
                else if (Game.win_chance > Kirill.raiseLow && Game.win_chance < Kirill.raiseHigh)
                    bot_raise();
                else
                    bot_check();
            }
        }
    }
}

void action()
{
    if ((Game.bot_balance != 0 && Game.user_balance != 0) || ((Game.bot_balance != 0 || Game.user_balance != 0) && Game.bot_bet != Game.user_bet))
    {
        int  res = 0;
        int  choose = 0;
        print_info();
        if (Game.bot_bet > Game.user_bet) {
        checkpoint:
            fputs("\n1-fold\n2-call\n3-raise\n4-all-in\nВыберите действие: ", stdout);
            do {
                res = scanf("%d", &choose);

                while (getchar() != '\n');
                if ((res == 1) && ((choose == 1) || (choose == 2) || (choose == 3) || (choose == 4))) {
                    switch (choose) {
                    case 1:
                        fold();
                        break;
                    case 2:
                        call();
                        break;
                    case 3:
                        raise();
                        break;
                    case 4:
                        all_in();
                        break;
                    }
                }
                else {
                    fputs("Ошибка!\n", stdout);
                    goto checkpoint;
                }
            } while (res != 1);
        }

        else
            if (Game.bot_bet < Game.user_bet) {
                action_bot(ch_bot);
            }
            else
                if (Game.bot_bet == Game.user_bet && Game.baton) {
                checkpoint1:
                    fputs("\n1-check\n2-bet\n3-all-in\nВыберите действие: ", stdout);
                    do {
                        res = scanf("%d", &choose);

                        while (getchar() != '\n');
                        if ((res == 1) && ((choose == 1) || (choose == 2) || (choose == 3))) {
                            switch (choose) {
                            case 1:
                                cheсk(ch_bot);
                                break;
                            case 2:
                                raise();
                                break;
                            case 3:
                                all_in();
                                break;
                            }
                        }
                        else {
                            fputs("Ошибка\n", stdout);
                            goto checkpoint1;
                        }
                    } while (res != 1);
                }
                else if (Game.bot_bet == Game.user_bet && Game.baton != 1) {
                    int check = Game.bot_bet;
                    action_bot(ch_bot);
                    if (check == Game.bot_bet) goto checkpoint1;

                }
    }
    else print_info();
}


float chanse(short stage)
{

    table table_chanse;
    hand  hand_chanse;
    memcpy(used_cards_copy, used_cards, sizeof(short) * 53);
    table_chanse = tb;
    float chanse = 0;
    short user_table[7] = { 0 };
    short bot_table[7] = { 0 };
    switch (stage)
    {
    case 1:
    {
        for (short i = 0; i < 10000; ++i) {
            user_table[0] = bot_table[0] = table_chanse.flop1;
            user_table[1] = bot_table[1] = table_chanse.flop2;
            user_table[2] = bot_table[2] = table_chanse.flop3;
            user_table[3] = bot_table[5] = card_generation();
            user_table[4] = bot_table[6] = card_generation();
            user_table[5] = card_generation();
            user_table[6] = card_generation();
            bot_table[3] = bot.first;
            bot_table[4] = bot.second;
            if ((LookupHand(bot_table) >> 12) > (LookupHand(user_table) >> 12))
                chanse++;
            else if ((LookupHand(bot_table) >> 12) == (LookupHand(user_table) >> 12) && (LookupHand(bot_table) & 0x00000FFF) > (LookupHand(user_table) & 0x00000FFF))
                chanse++;
            memcpy(used_cards, used_cards_copy, sizeof(short) * 53);
        }
        return chanse / 10000;
        break;
    }
    case 2:
    {
        for (short i = 0; i < 10000; ++i) {
            user_table[0] = bot_table[0] = table_chanse.flop1;
            user_table[1] = bot_table[1] = table_chanse.flop2;
            user_table[2] = bot_table[2] = table_chanse.flop3;
            user_table[3] = bot_table[5] = table_chanse.turn;
            user_table[4] = bot_table[6] = card_generation();
            user_table[5] = card_generation();
            user_table[6] = card_generation();
            bot_table[3] = bot.first;
            bot_table[4] = bot.second;
            if ((LookupHand(bot_table) >> 12) > (LookupHand(user_table) >> 12))
                chanse++;
            else if ((LookupHand(bot_table) >> 12) == (LookupHand(user_table) >> 12) && (LookupHand(bot_table) & 0x00000FFF) > (LookupHand(user_table) & 0x00000FFF))
                chanse++;
            memcpy(used_cards, used_cards_copy, sizeof(short) * 53);
        }
        return chanse / 10000;
        break;
    }
    case 3:
    {
        for (short i = 0; i < 10000; ++i) {
            user_table[0] = bot_table[0] = table_chanse.flop1;
            user_table[1] = bot_table[1] = table_chanse.flop2;
            user_table[2] = bot_table[2] = table_chanse.flop3;
            user_table[3] = bot_table[5] = table_chanse.turn;
            user_table[4] = bot_table[6] = table_chanse.river;
            user_table[5] = card_generation();
            user_table[6] = card_generation();
            bot_table[3] = bot.first;
            bot_table[4] = bot.second;
            if ((LookupHand(bot_table) >> 12) > (LookupHand(user_table) >> 12))
                chanse++;
            else if ((LookupHand(bot_table) >> 12) == (LookupHand(user_table) >> 12) && (LookupHand(bot_table) & 0x00000FFF) > (LookupHand(user_table) & 0x00000FFF))
                chanse++;
            memcpy(used_cards, used_cards_copy, sizeof(short) * 53);
        }
        return chanse / 10000;
    }

    }
}

float chanse_bots(short round, short key) {
    hand test_hand;
    table test_table;
    float chanse = 0;
    memcpy(used_cards_copy, used_cards, sizeof(short) * 53);
    float draw = 0;
    short bot_chanse[7];
    short bot1_chanse[7];
    test_table.flop1 = tb.flop1;
    test_table.flop2 = tb.flop2;
    test_table.flop2 = tb.flop3;
    if (key == 1)
    {
        switch (round) {
        case 1:
            bot1_chanse[0] = tb.flop1;
            bot1_chanse[1] = tb.flop2;
            bot1_chanse[2] = tb.flop3;
            bot_chanse[0] = tb.flop1;
            bot_chanse[1] = tb.flop2;
            bot_chanse[2] = tb.flop3;
            bot_chanse[3] = bot.first;
            bot_chanse[4] = bot.second;
            for (short i = 0; i < 10000; ++i) {
                bot1_chanse[3] = card_generation();
                bot1_chanse[4] = card_generation();
                bot1_chanse[5] = card_generation();
                bot1_chanse[6] = card_generation();
                bot_chanse[5] = bot1_chanse[3];
                bot_chanse[6] = bot1_chanse[4];
                test_table.turn = bot1_chanse[3];
                test_table.river = bot1_chanse[4];
                test_hand.first = bot1_chanse[5];
                test_hand.second = bot1_chanse[6];
                if ((LookupHand(bot_chanse) >> 12) > (LookupHand(bot1_chanse) >> 12))
                    chanse++;
                else if ((LookupHand(bot_chanse) >> 12) == (LookupHand(bot1_chanse) >> 12) && (LookupHand(bot_chanse) & 0x00000FFF) > (LookupHand(bot1_chanse) & 0x00000FFF))
                    chanse++;
                else if ((LookupHand(bot_chanse) & 0x00000FFF) == (LookupHand(bot1_chanse) & 0x00000FFF) && (LookupHand(bot_chanse) & 0x00000FFF) == (LookupHand(bot1_chanse) & 0x00000FFF)) {
                    draw += 1;
                }
                memcpy(used_cards, used_cards_copy, sizeof(short) * 53);

            }
            draw_chanse = draw / 10000;
            win_ch = chanse / 10000;
            Game2.bot2_win_chance = 1 - win_ch - draw_chanse;
            Game2.bot1_win_chance = win_ch;
            return chanse / 10000;
            break;
        case 2:
            bot1_chanse[0] = tb.flop1;
            bot1_chanse[1] = tb.flop2;
            bot1_chanse[2] = tb.flop3;
            bot1_chanse[3] = tb.turn;
            bot_chanse[0] = tb.flop1;
            bot_chanse[1] = tb.flop2;
            bot_chanse[2] = tb.flop3;
            bot_chanse[3] = bot.first;
            bot_chanse[4] = bot.second;
            bot_chanse[5] = tb.turn;
            for (short i = 0; i < 10000; ++i) {
                bot1_chanse[4] = card_generation();
                bot1_chanse[5] = card_generation();
                bot1_chanse[6] = card_generation();
                bot_chanse[6] = bot1_chanse[4];
                test_table.turn = bot1_chanse[3];
                test_table.river = bot1_chanse[4];
                test_hand.first = bot1_chanse[5];
                test_hand.second = bot1_chanse[6];
                if ((LookupHand(bot_chanse) >> 12) > (LookupHand(bot1_chanse) >> 12))
                    chanse++;
                else if ((LookupHand(bot_chanse) >> 12) == (LookupHand(bot1_chanse) >> 12) && (LookupHand(bot_chanse) & 0x00000FFF) > (LookupHand(bot1_chanse) & 0x00000FFF))
                    chanse++;
                else if ((LookupHand(bot_chanse) & 0x00000FFF) == (LookupHand(bot1_chanse) & 0x00000FFF) && (LookupHand(bot_chanse) & 0x00000FFF) == (LookupHand(bot1_chanse) & 0x00000FFF)) {
                    draw += 1;
                }
                memcpy(used_cards, used_cards_copy, sizeof(int) * 53);
            }
            draw_chanse = draw / 10000;
            win_ch = chanse / 10000;
            Game2.bot2_win_chance = 1 - win_ch - draw_chanse;
            Game2.bot1_win_chance = win_ch;
            return chanse / 10000;
            break;
        case 3:
            bot1_chanse[0] = tb.flop1;
            bot1_chanse[1] = tb.flop2;
            bot1_chanse[2] = tb.flop3;
            bot1_chanse[3] = tb.turn;
            bot1_chanse[4] = tb.river;
            bot_chanse[0] = tb.flop1;
            bot_chanse[1] = tb.flop2;
            bot_chanse[2] = tb.flop3;
            bot_chanse[3] = bot.first;
            bot_chanse[4] = bot.second;
            bot_chanse[5] = tb.turn;
            bot_chanse[6] = tb.river;
            for (short i = 0; i < 10000; ++i) {
                bot1_chanse[5] = card_generation();
                bot1_chanse[6] = card_generation();
                test_table.turn = bot1_chanse[3];
                test_table.river = bot1_chanse[4];
                test_hand.first = bot1_chanse[5];
                test_hand.second = bot1_chanse[6];
                if ((LookupHand(bot_chanse) >> 12) > (LookupHand(bot1_chanse) >> 12))
                    chanse++;
                else if ((LookupHand(bot_chanse) >> 12) == (LookupHand(bot1_chanse) >> 12) && (LookupHand(bot_chanse) & 0x00000FFF) > (LookupHand(bot1_chanse) & 0x00000FFF))
                    chanse++;
                else if ((LookupHand(bot_chanse) & 0x00000FFF) == (LookupHand(bot1_chanse) & 0x00000FFF) && (LookupHand(bot_chanse) & 0x00000FFF) == (LookupHand(bot1_chanse) & 0x00000FFF)) {
                    draw += 1;
                }
                memcpy(used_cards, used_cards_copy, sizeof(int) * 53);
            }
            draw_chanse = draw / 10000;
            win_ch = chanse / 10000;
            Game2.bot2_win_chance = 1 - win_ch - draw_chanse;
            Game2.bot1_win_chance = win_ch;
            return chanse / 10000;
            break;
        }
    }
    else
    {
        switch (round)
        {
        case 1:
            bot1_chanse[0] = tb.flop1;
            bot1_chanse[1] = tb.flop2;
            bot1_chanse[2] = tb.flop3;
            bot_chanse[0] = tb.flop1;
            bot_chanse[1] = tb.flop2;
            bot_chanse[2] = tb.flop3;
            bot_chanse[3] = user.first;
            bot_chanse[4] = user.second;
            for (short i = 0; i < 10000; ++i) {
                bot1_chanse[3] = card_generation();
                bot1_chanse[4] = card_generation();
                bot1_chanse[5] = card_generation();
                bot1_chanse[6] = card_generation();
                bot_chanse[5] = bot1_chanse[3];
                bot_chanse[6] = bot1_chanse[4];

                test_table.turn = bot1_chanse[3];
                test_table.river = bot1_chanse[4];

                test_hand.first = bot1_chanse[5];
                test_hand.second = bot1_chanse[6];
                if ((LookupHand(bot_chanse) >> 12) > (LookupHand(bot1_chanse) >> 12))
                    chanse++;
                else if ((LookupHand(bot_chanse) >> 12) == (LookupHand(bot1_chanse) >> 12) && (LookupHand(bot_chanse) & 0x00000FFF) > (LookupHand(bot1_chanse) & 0x00000FFF))
                    chanse++;
                else if ((LookupHand(bot_chanse) & 0x00000FFF) == (LookupHand(bot1_chanse) & 0x00000FFF) && (LookupHand(bot_chanse) & 0x00000FFF) == (LookupHand(bot1_chanse) & 0x00000FFF)) {
                    draw += 1;
                }
                memcpy(used_cards, used_cards_copy, sizeof(int) * 53);

            }
            draw_chanse = draw / 10000;
            win_ch = chanse / 10000;
            Game2.bot2_win_chance = 1 - win_ch - draw_chanse;
            Game2.bot1_win_chance = win_ch;
            return chanse / 10000;
            break;
        case 2:
            bot1_chanse[0] = tb.flop1;
            bot1_chanse[1] = tb.flop2;
            bot1_chanse[2] = tb.flop3;
            bot1_chanse[3] = tb.turn;
            bot_chanse[0] = tb.flop1;
            bot_chanse[1] = tb.flop2;
            bot_chanse[2] = tb.flop3;
            bot_chanse[3] = bot.first;
            bot_chanse[4] = bot.second;
            bot_chanse[5] = tb.turn;
            for (short i = 0; i < 10000; ++i) {
                bot1_chanse[4] = card_generation();
                bot1_chanse[5] = card_generation();
                bot1_chanse[6] = card_generation();
                bot_chanse[6] = bot1_chanse[4];
                test_table.turn = bot1_chanse[3];
                test_table.river = bot1_chanse[4];

                test_hand.first = bot1_chanse[5];
                test_hand.second = bot1_chanse[6];
                if ((LookupHand(bot_chanse) >> 12) > (LookupHand(bot1_chanse) >> 12))
                    chanse++;
                else if ((LookupHand(bot_chanse) >> 12) == (LookupHand(bot1_chanse) >> 12) && (LookupHand(bot_chanse) & 0x00000FFF) > (LookupHand(bot1_chanse) & 0x00000FFF))
                    chanse++;
                else if ((LookupHand(bot_chanse) & 0x00000FFF) == (LookupHand(bot1_chanse) & 0x00000FFF) && (LookupHand(bot_chanse) & 0x00000FFF) == (LookupHand(bot1_chanse) & 0x00000FFF)) {
                    draw += 1;
                }
                memcpy(used_cards, used_cards_copy, sizeof(int) * 53);

            }
            draw_chanse = draw / 10000;
            win_ch = chanse / 10000;
            Game2.bot2_win_chance = 1 - win_ch - draw_chanse;
            Game2.bot1_win_chance = win_ch;
            return chanse / 10000;
            break;
        case 3:
            bot1_chanse[0] = tb.flop1;
            bot1_chanse[1] = tb.flop2;
            bot1_chanse[2] = tb.flop3;
            bot1_chanse[3] = tb.turn;
            bot1_chanse[4] = tb.river;
            bot_chanse[0] = tb.flop1;
            bot_chanse[1] = tb.flop2;
            bot_chanse[2] = tb.flop3;
            bot_chanse[3] = bot.first;
            bot_chanse[4] = bot.second;
            bot_chanse[5] = tb.turn;
            bot_chanse[6] = tb.river;
            for (short i = 0; i < 10000; ++i) {
                bot1_chanse[5] = card_generation();
                bot1_chanse[6] = card_generation();

                test_table.turn = bot1_chanse[3];
                test_table.river = bot1_chanse[4];

                test_hand.first = bot1_chanse[5];
                test_hand.second = bot1_chanse[6];

                if ((LookupHand(bot_chanse) >> 12) > (LookupHand(bot1_chanse) >> 12))
                    chanse++;
                else if ((LookupHand(bot_chanse) >> 12) == (LookupHand(bot1_chanse) >> 12) && (LookupHand(bot_chanse) & 0x00000FFF) > (LookupHand(bot1_chanse) & 0x00000FFF))
                    chanse++;
                else if ((LookupHand(bot_chanse) & 0x00000FFF) == (LookupHand(bot1_chanse) & 0x00000FFF) && (LookupHand(bot_chanse) & 0x00000FFF) == (LookupHand(bot1_chanse) & 0x00000FFF)) {
                    draw += 1;
                }
                memcpy(used_cards, used_cards_copy, sizeof(short) * 53);

            }
            draw_chanse = draw / 10000;
            win_ch = chanse / 10000;
            Game2.bot2_win_chance = 1 - win_ch - draw_chanse;
            Game2.bot1_win_chance = win_ch;
            return chanse / 10000;
            break;
        }
    }
}

void game_generation(short* raund_user, short* raund_bot, short stage)
{
    switch (stage)
    {
    case 1:
    {
        raund_bot[0] = raund_user[0] = tb.flop1 = card_generation();
        raund_bot[1] = raund_user[1] = tb.flop2 = card_generation();
        raund_bot[2] = raund_user[2] = tb.flop3 = card_generation();
        raund_user[3] = user.first = card_generation();
        raund_user[4] = user.second = card_generation();
        raund_bot[3] = bot.first = card_generation();
        raund_bot[4] = bot.second = card_generation();
        break;
    }
    case 2:
    {
        raund_user[0] = raund_bot[0] = tb.flop1;
        raund_user[1] = raund_bot[1] = tb.flop2;
        raund_user[2] = raund_bot[2] = tb.flop3;
        raund_user[3] = raund_bot[3] = tb.turn = card_generation();
        raund_user[4] = user.first;
        raund_bot[4] = bot.first;
        raund_user[5] = user.second;
        raund_bot[5] = bot.second;
        break;
    }
    case 3:
    {
        raund_user[0] = raund_bot[0] = tb.flop1;
        raund_user[1] = raund_bot[1] = tb.flop2;
        raund_user[2] = raund_bot[2] = tb.flop3;
        raund_user[3] = raund_bot[3] = tb.turn;
        raund_user[4] = raund_bot[4] = tb.river = card_generation();
        raund_user[5] = user.first;
        raund_bot[5] = bot.first;
        raund_user[6] = user.second;
        raund_bot[6] = bot.second;
        break;
    }
    }
}

void win()
{
    if (Game.user_chance > Game.bot_chance) {
        fputs("\nРука бота:", stdout);
        print_hand(bot);

        fputs("\nВаша рука:", stdout);
        print_hand(user);

        ("Стол:\n");
        print_table(tb, 5);
        fputs("Вы выиграли!\n", stdout);
        Game.user_balance += Game.bank;
        Game.bank = 0;
        Game.bot_bet = 0;
        Game.user_bet = 0;
    }
    else if (Game.user_chance < Game.bot_chance) {
        fputs("Рука бота:\n", stdout);
        print_hand(bot);
        fputs("\nВаша рука:", stdout);
        print_hand(user);
        fputs("Стол:\n", stdout);
        print_table(tb, 5);
        fputs("Бот выиграл\n", stdout);
        Game.bot_balance += Game.bank;
        Game.bank = 0;
        Game.bot_bet = 0;
        Game.user_bet = 0;
    }
    else
    {
        fputs("\nРука бота:", stdout);
        print_hand(bot);
        fputs("\nВаша рука:", stdout);
        print_hand(user);
        fputs("Стол:\n", stdout);
        print_table(tb, 5);
        fputs("Ничья\n", stdout);
        Game.user_balance += Game.bank >> 1;
        Game.bot_balance += Game.bank >> 1;
        Game.bank = 0;
        Game.bot_bet = 0;
        Game.user_bet = 0;
    }
}

void win_bots()
{
    if (Game2.bot2_chance > Game2.bot1_chance) {
        Game2.bot2_balance += Game2.bank;
        Game2.bank = 0;
        Game2.bot1_bet = 0;
        Game2.bot2_bet = 0;
    }
    else if (Game2.bot2_chance < Game2.bot1_chance) {
        Game2.bot1_balance += Game2.bank;
        Game2.bank = 0;
        Game2.bot1_bet = 0;
        Game2.bot2_bet = 0;
    }
    else
    {
        Game2.bot2_balance += Game2.bank >> 1;
        Game2.bot1_balance += Game2.bank >> 1;
        Game2.bank = 0;
        Game2.bot1_bet = 0;
        Game2.bot2_bet = 0;
    }
}

void bot_vs_bot() {
    if (Game2.bot1_balance != 0 && Game2.bot2_balance != 0 || Game2.bot1_bet != Game2.bot2_bet) {
        if (Game2.bot2_bet == Game2.bot1_bet) {
            if (Game2.baton) {
                if (Game2.bot1_win_chance > Danya.checkLow && Game2.bot1_win_chance < Danya.checkHigh || (rand() % 100) < Danya.blef) {
                    Game2.restart = 0;
                }
                else if (Game2.bot1_win_chance > Danya.raiseLow && Game2.bot1_win_chance < Danya.raiseHigh || (rand() % 100) < Danya.blef) {
                    int bet;
                    int count = 0;
                    bet = Game2.bot1_win_chance * Game2.bank;
                    while (bet < Game2.bot2_bet - Game2.bot1_bet)
                    {
                        bet += Game2.bot1_win_chance * Game2.bank;
                        count++;
                        if (bet < -1000000 || count == 10000)
                        {
                            Game2.restart = 1;
                            bet = 500000;
                            break;
                        }
                    }
                    if (bet == 500000)
                        return;
                    if (bet > fmin(Game2.bot1_balance, Game2.bot2_balance)) bet = fmin(Game2.bot1_balance, Game2.bot2_balance);
                    Game2.bank += bet;
                    Game2.bot1_bet += bet;
                    Game2.bot1_balance -= bet;
                }
                else if (Game2.bot1_win_chance > Danya.allinLow || (rand() % 100) < Danya.blef) {
                    int bet = Game2.bot1_balance;
                    if (bet > Game2.bot2_balance) bet = Game2.bot2_balance;
                    Game2.bank += bet;
                    Game2.bot1_bet += bet;
                    Game2.bot1_balance -= bet;
                }
                else {
                    Game2.restart = 1;
                    Game2.bot2_balance += Game2.bank;
                    Game2.bot1_bet = 0;
                    Game2.bot2_bet = 0;
                    Game2.bank = 0;

                }
            }
            else {

                if (Game2.bot2_win_chance > Kirill.checkLow && Game2.bot2_win_chance < Kirill.checkHigh || (rand() % 100) < Kirill.blef) {
                    Game2.restart = 0;

                }
                else if (Game2.bot2_win_chance > Kirill.raiseLow && Game2.bot2_win_chance < Kirill.raiseHigh || (rand() % 100) < Kirill.blef) {


                    int bet;
                    int count = 0;
                    bet = Game2.bot2_win_chance * Game2.bank;
                    while (bet < Game2.bot1_bet - Game2.bot2_bet)
                    {
                        bet += Game2.bot1_win_chance * Game2.bank;
                        count++;
                        if (bet < -1000000 || count == 10000)
                        {
                            Game2.restart = 1;
                            bet = 500000;
                            break;
                        }
                    }
                    if (bet == 500000)
                        return;
                    if (bet > fmin(Game2.bot1_balance, Game2.bot2_balance)) bet = fmin(Game2.bot1_balance, Game2.bot2_balance);
                    Game2.bank += bet;
                    Game2.bot2_bet += bet;
                    Game2.bot2_balance -= bet;
                }
                else if (Game2.bot2_win_chance > Kirill.allinLow || (rand() % 100) < Kirill.blef) {
                    int bet = Game2.bot2_balance;
                    if (bet > Game2.bot1_balance) bet = Game2.bot1_balance;
                    Game2.bank += bet;
                    Game2.bot2_bet += bet;
                    Game2.bot2_balance -= bet;
                }
                else {
                    Game2.restart = 1;
                    Game2.bot1_balance += Game2.bank;
                    Game2.bot2_bet = 0;
                    Game2.bot1_bet = 0;
                    Game2.bank = 0;

                }
            }
        }
        while (Game2.bot2_bet != Game2.bot1_bet) {
            if (Game2.bot1_bet < Game2.bot2_bet) {
                if (Game2.bot1_win_chance > Danya.callLow && Game2.bot1_win_chance < Danya.callHigh || (rand() % 100) < Danya.blef) {
                    Game2.bank += Game2.bot2_bet - Game2.bot1_bet;
                    Game2.bot1_balance -= Game2.bot2_bet - Game2.bot1_bet;
                    Game2.bot1_bet += Game2.bot2_bet - Game2.bot1_bet;


                    Game2.restart = 0;

                }
                else if (Game2.bot1_win_chance > Danya.raiseLow && Game2.bot1_win_chance < Danya.raiseHigh || (rand() % 100) < Danya.blef) {

                    int bet;
                    int count = 0;
                    bet = Game2.bot1_win_chance * Game2.bank;
                    while (bet < Game2.bot2_bet - Game2.bot1_bet) {
                        bet += Game2.bot1_win_chance * Game2.bank;
                        count++;
                        if (bet < -1000000 || count == 10000)
                        {
                            Game2.restart = 1;
                            bet = 500000;
                            break;
                        }
                    }
                    if (bet == 500000)
                        break;
                    if (bet > fmin(Game2.bot1_balance, Game2.bot2_balance)) bet = fmin(Game2.bot1_balance, Game2.bot2_balance);
                    Game2.bank += bet;
                    Game2.bot1_bet += bet;
                    Game2.bot1_balance -= bet;

                }
                else if (Game2.bot1_win_chance > Danya.allinLow || (rand() % 100) < Danya.blef) {
                    int bet = Game2.bot1_balance;
                    if (bet > Game2.bot2_balance) bet = Game2.bot2_balance;
                    Game2.bank += bet;
                    Game2.bot1_bet += bet;
                    Game2.bot1_balance -= bet;


                }
                else {
                    printf("fold\n");
                    Game2.restart = 1;
                    Game2.bot2_balance += Game2.bank;
                    Game2.bot1_bet = 0;
                    Game2.bot2_bet = 0;
                    Game2.bank = 0;

                }
            }

            if (Game2.bot2_bet < Game2.bot1_bet) {
                if (Game2.bot2_win_chance > Kirill.callLow && Game2.bot2_win_chance < Kirill.callHigh || (rand() % 100) < Kirill.blef) {
                    Game2.bank += Game2.bot1_bet - Game2.bot2_bet;
                    Game2.bot2_balance -= Game2.bot1_bet - Game2.bot2_bet;
                    Game2.bot2_bet += Game2.bot1_bet - Game2.bot2_bet;


                    Game2.restart = 0;

                }
                else if (Game2.bot2_win_chance > Kirill.raiseLow && Game2.bot2_win_chance < Kirill.raiseHigh || (rand() % 100) < Kirill.blef) {
                    int bet;
                    int count = 0;
                    bet = Game2.bot2_win_chance * Game2.bank;
                    while (bet < Game2.bot1_bet - Game2.bot2_bet)
                    {
                        bet += Game2.bot2_win_chance * Game2.bank;
                        count++;
                        if (bet < -1000000 || count == 10000)
                        {
                            Game2.restart = 1;
                            bet = 500000;
                            break;
                        }
                    }
                    if (bet == 500000)
                        break;
                    if (bet > fmin(Game2.bot1_balance, Game2.bot2_balance)) bet = fmin(Game2.bot1_balance, Game2.bot2_balance);
                    if (bet > Game2.bot1_balance) bet = Game2.bot1_balance;
                    Game2.bank += bet;
                    Game2.bot2_bet += bet;
                    Game2.bot2_balance -= bet;

                }
                else if (Game2.bot2_win_chance > Kirill.allinLow || (rand() % 100) < Kirill.blef) {
                    int bet = Game2.bot2_balance;
                    if (bet > Game2.bot1_balance) bet = Game2.bot1_balance;
                    Game2.bank += bet;
                    Game2.bot2_bet += bet;
                    Game2.bot2_balance -= bet;

                }
                else {
                    printf("fold\n");
                    Game2.restart = 1;
                    Game2.bot1_balance += Game2.bank;
                    Game2.bot2_bet = 0;
                    Game2.bot1_bet = 0;
                    Game2.bank = 0;

                }
            }
        }
    }
}

int main()
{
    memset(HR, 0, sizeof(HR));
    FILE* fin = fopen("HandRanks.dat", "rb");
    if (!fin)
        return false;
    size_t bytesread = fread(HR, sizeof(HR), 1, fin);	// Заполнили таблицу HandRanks
    fclose(fin);
    short col1 = 0, col2 = 0;
    setlocale(LC_ALL, "");
    srand(time(NULL));
    Game.user_balance = 2000;
    Game.bot_balance = 2000;
    Game.baton = 0;
    Game2.bot1_balance = 2000;
    Game2.bot2_balance = 2000;
    Game2.baton = 0;
    fputs("Выберите выариант:\n1 - игра против Бота\n2 - Бот против Бота\n", stdout);
    scanf("%d", &ans);
    bot_settings game_mode;
    switch (ans)
    {
    case 1:
    {
        fputs("Выберите выариант:\n1 - игра против Бота Даня\n2 - игра против Бота Кирилл\n", stdout);
        ch_bot;
        scanf("%d", &ch_bot);
        while (Game.user_balance != 0 && Game.bot_balance != 0)
        {
            memset(used_cards, 0, sizeof(short) * 53);
            short river_user[7];
            short river_bot[7];
            game_generation(river_user, river_bot, 1);
            fputs("\nВаша рука:", stdout);
            print_hand(user);
            print_table(tb, 3);
            blind();
            Game.win_chance = chanse(1);
            action();
            if (Game.restart == 1) continue;
            game_generation(river_user, river_bot, 2);
            fputs("\nВаша рука:", stdout);
            print_hand(user);
            print_table(tb, 4);
            Game.win_chance = chanse(2);
            action();
            if (Game.restart == 1) continue;

            game_generation(river_user, river_bot, 3);
            fputs("\nВаша рука:", stdout);
            print_hand(user);
            print_table(tb, 5);
            Game.user_chance = (LookupHand(river_user) & 0x00000FFF);
            Game.bot_chance = (LookupHand(river_bot) & 0x00000FFF);
            Game.win_chance = chanse(3);
            action();
            if (Game.restart == 1)
            {
                continue;

            }
            win();
            if (Game.user_balance == 0 || Game.bot_balance == 0)
                break;
            char yn;
            fputs("Начать новую раздачу(y/n)?", stdout);
            scanf("%c", &yn);
            if (yn == 'y')
                continue;
            else
                break;
            fputs("\n", stdout);

        }
        if (Game.user_balance == 0) fputs("Бот выиграл игру :(\n", stdout);
        if (Game.bot_balance == 0) fputs("Вы выиграли игру! :)\n", stdout);
        break;
    }
    case 2:
    {
        float time = 0;
        for (short pop = 0; pop < 10; pop++)
        {
            clock_t start = clock();
            col1 = 0;
            col2 = 0;
            for (short kol = 0; kol < 100; kol++)
            {
                Game2.bot1_balance = 2000;
                Game2.bot2_balance = 2000;
                Game2.baton = 0;
                while (Game2.bot1_balance != 0 && Game2.bot2_balance != 0)
                {
                    memset(used_cards, 0, sizeof(short) * 53);
                    short river_bot2[7];
                    short river_bot1[7];
                    game_generation(river_bot2, river_bot1, 1);
                    blind_bots();
                    //printf("Блайнд %d %d\n", Game2.bot1_bet, Game2.bot2_bet);
                    chanse_bots(1, 1);
                    //printf("1 %f %f\n", Game2.bot1_win_chance, Game2.bot2_win_chance);
                    bot_vs_bot();
                    //printf("Первый ход %d %d %d %d\n", Game2.bot1_balance, Game2.bot2_balance, Game2.bot1_bet, Game2.bot2_bet);
                    if (Game2.restart == 1) continue;

                    game_generation(river_bot2, river_bot1, 2);
                    chanse_bots(2, 1);
                    //printf("2 %f %f\n", Game2.bot1_win_chance, Game2.bot2_win_chance);
                    bot_vs_bot();
                    //printf("Второй ход %d %d %d %d\n", Game2.bot1_balance, Game2.bot2_balance, Game2.bot1_bet, Game2.bot2_bet);
                    if (Game2.restart == 1) continue;

                    game_generation(river_bot2, river_bot1, 3);
                    Game2.bot2_chance = (LookupHand(river_bot2) & 0x00000FFF);
                    Game2.bot1_chance = (LookupHand(river_bot1) & 0x00000FFF);
                    chanse_bots(3, 1);
                    //printf("3 %f %f\n", Game2.bot1_win_chance, Game2.bot2_win_chance);
                    bot_vs_bot();
                    //printf("Третий ход %d %d %d %d\n", Game2.bot1_balance, Game2.bot2_balance, Game2.bot1_bet, Game2.bot2_bet);
                    if (Game2.restart == 1)
                    {
                        continue;
                    }
                    win_bots();
                    if (Game2.bot2_balance == 0 || Game2.bot1_balance == 0)
                        break;
                }
                if (Game2.bot1_balance == 0)
                    col1++;
                else if (Game2.bot2_balance == 0)
                    col2++;
            }
            printf("Кирилл %d Даня %d\n", col2, col1);
            clock_t end = clock();
            printf("ВРЕМЯ РАБОТЫ %f\n", (double)(end - start) / CLOCKS_PER_SEC);
            time += (double)(end - start) / CLOCKS_PER_SEC;
        }
        printf("СРЕДНЕЕ ВРЕМЯ %f\n", time / 10);
    }
    }
}
