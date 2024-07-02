#ifndef __KNIGHT_H__
#define __KNIGHT_H__

//basic status
#define DeadHP          00
#define NoRemedy        00
#define NoMaidenKiss    00
#define NoPhoenixDown   00
#define LevelMin        01
#define LevelMax        10
#define RemedyMax       99
#define MaidenKissMax   99
#define PhoenixDownMax  99
//base dmg
#define MadBear   10
#define Bandit    15
#define LordLupin 45
#define Elf       75 
#define Troll     95
// display status
#define display_state display(HP, level, remedy, maidenkiss, phoenixdown, rescue)

#include "main.h"

//setting
void display         (int HP, int level, int remedy, int maidenkiss, int phoenixdown, int rescue);
void adventureToKoopa(string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue);
void getinput        (string file_input, int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown, int &rescue, 
                        string &events_code, string &event_pack);
void inputknight     (string knight_data, int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown, int &rescue);
// reset limit
void returnMaxHP          (int &HP, int MaxHP);
void returnMaxLv          (int &level);
void returnMaxRemedy      (int &remedy);
void returnMaxMaidenKiss  (int &maidenkiss);
void returnMaxPhoenixDown (int &phoenixdown);
// special character
void checkLancelot(int MaxHP, int &special_class);
void checkArthur  (int MaxHP, int &special_class);
// combat + buff event
void combat_creep           (int nth_event, int &HP, int &level, int special_class, int monster_type);
void combat_shaman          (int nth_event, int &HP, int &level, int &remedy, int special_class, 
                                int &knight_status, int &debuff_state);
void combat_Vajsh           (int nth_event, int &HP, int &level, int &level_before_frog, int &maidenkiss, int special_class,
                                int &knight_status, int &debuff_state);
void combat_boss            (int &level, int &rescue, int special_class);
void take_MushMario         (int &HP, int MaxHP, int level, int phoenixdown);
void take_FibonacciMushFibo (int &HP);
void take_MushGhost         (int &HP, int MaxHP, int &phoenixdown, string file_mush_ghost, string MushGhosts_type);
void take_glossary          (int &remedy, int &maidenkiss, int &phoenixdown, int medicine_type);
void meet_Asclepius         (int &remedy, int &maidenkiss, int &phoenixdown, string file_aclepius_pack);
void meet_MajoMerlin        (int &HP, string file_merlin_pack);

string combat_result        (int nth_event, int level);
//bonus event for MushGhost (event_code : 13*)
void MushGhost_type1(int &HP, int n2, int* n2arr);
void MushGhost_type2(int &HP, int n2, int* n2arr);
void MushGhost_type3(int &HP, int n2, int* n2arr1);
void MushGhost_type4(int &HP, int n2, int* n2arr1);
//Important Algorithms
bool Eratosthenes(int HP);

#endif // __KNIGHT_H__