#include "knight.h"

void display(int HP, int level, int remedy, int maidenkiss, int phoenixdown, int rescue) {
    cout << "HP=" << HP
        << ", level=" << level
        << ", remedy=" << remedy
        << ", maidenkiss=" << maidenkiss
        << ", phoenixdown=" << phoenixdown
        << ", rescue=" << rescue << endl;
}

void adventureToKoopa(string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue){
    //Display Knight base status + set event
    string events_code, event_pack;
    rescue = -1; // -1 Bouken not done, 0 Can not rescue, 1 rescue succeed
    getinput(file_input, HP, level, remedy, maidenkiss, phoenixdown, rescue, events_code, event_pack);
    int MaxHP = HP;
    int knight_status = 1;      //1 - Normal, 2 - Tiny, 3 - Frog
    int special_class = 0;      //0 - Knight of the Round Table, 1 - Lancelot, 2 - King Arthur 
    int nth_event = 1;          //Event Counter  
    int debuff_state = 0;       //Debuff turns remaining
    checkArthur(MaxHP, special_class);
    checkLancelot(MaxHP,special_class);
    //Special state in Event
    int level_before_frog; //Use when knight transform frog
    int meet_Merlin = 0;   //Use when meet Merlin
    int meet_Aclepius = 0; //Use when meet Asclepius
    stringstream database;
    database << events_code;
    // Event code with event
    /*
    Event_code  ||                 Event
        00      ||  Bowser surrender and give back the princess
        01      ||  Battle with MadBear 
        02      ||  Battle with Bandit
        03      ||  Battle with LordLupin
        04      ||  Battle with Elf
        05      ||  Battle with Troll
        06      ||  Battle with Shaman
        07      ||  Battle with Vajsh
        11      ||  Take energy mushroom MushMario 
        12      ||  Take Fibonacci mushroom MushFibo
        13      ||  Take ghost mushrooms MushGhost
        15      ||  Take Remedy
        16      ||  Take MaidenKiss
        17      ||  Take PhoenixDown
        18      ||  Meet Majo Merlin
        19      ||  Meet God Asclepius
        99      ||  Battle with Bowser
    */
    while(!database.eof()){
        //Start turn
        string event_code;
        database >> event_code;
        if(debuff_state > 0) debuff_state--;
        //Event run
        if(event_code.size() == 1){
            if(event_code[0] == '0') rescue = 1;
            if(event_code[0] == '1'||event_code[0] == '2'||event_code[0] == '3'||event_code[0] == '4'||event_code[0] == '5'){
                int event_num = event_code[0] - '0';
                combat_creep(nth_event, HP, level, special_class, event_num);
            }
            if(event_code[0] == '6'){
                if(knight_status == 1) combat_shaman(nth_event, HP, level, remedy, special_class, knight_status, debuff_state);
            }
            if(event_code[0] == '7'){
                if(knight_status == 1) combat_Vajsh(nth_event, HP, level, level_before_frog, maidenkiss, special_class, knight_status, debuff_state);
            }
        }    
        if(event_code.size() == 2){
            if(event_code == "11"){
                take_MushMario(HP, MaxHP, level, phoenixdown);
            }
            if(event_code == "12"){
                take_FibonacciMushFibo(HP);
            }   
            if(event_code == "15" || event_code == "16" || event_code == "17"){
                int event_num = 10 + (event_code[1] - '0');
                take_glossary(remedy, maidenkiss, phoenixdown, event_num);
            } 
            if(event_code == "18" && meet_Merlin == 0){
                size_t tmp4 = event_pack.find(',');
                size_t tmp5 = event_pack.find(',', tmp4 + 1) + 1;
                string file_merlin_pack = event_pack.substr(tmp5);
                meet_MajoMerlin(HP, file_merlin_pack);
                returnMaxHP(HP, MaxHP);
                meet_Merlin++;
            }
            if(event_code == "19" && meet_Aclepius == 0){
                size_t tmp4 = event_pack.find(',');
                size_t tmp5 = event_pack.find(',', tmp4 + 1);
                string file_aclepius_pack = event_pack.substr(tmp4 + 1, tmp5 - tmp4 - 1);
                meet_Asclepius(remedy, maidenkiss, phoenixdown, file_aclepius_pack);
                returnMaxRemedy(remedy);
                returnMaxMaidenKiss(maidenkiss);
                returnMaxPhoenixDown(phoenixdown);
                meet_Aclepius++;
            }
            if(event_code == "99") combat_boss(level, rescue, special_class);
        }
        if(event_code.size() >= 3){
            string MushGhosts_type = event_code.substr(2, event_code.size() - 1);
            size_t tmp4 = event_pack.find(',');
            string file_mush_ghost = event_pack.substr(0, tmp4);
            take_MushGhost(HP, MaxHP, phoenixdown, file_mush_ghost, MushGhosts_type);
        }
        //Dispel Debuff + Revive
        if(remedy > NoRemedy && knight_status == 2){
            HP *= 5;
            remedy--;
            knight_status = 1;
            debuff_state = 0;
        }
        if(maidenkiss > NoMaidenKiss && knight_status == 3){
            level = level_before_frog;
            maidenkiss--;
            knight_status = 1;
            debuff_state = 0;
        }
        if(HP <= 0){
            if(phoenixdown == NoPhoenixDown) rescue = 0;
            else{
                HP = MaxHP;
                phoenixdown--;
                if(knight_status == 3) level = level_before_frog; 
                knight_status = 1;
                debuff_state = 0;
                HP = MaxHP;
            }
        }
        if(knight_status != 1 && debuff_state == 0){
            if(knight_status == 2) HP *= 5;
            if(knight_status == 3) level = level_before_frog;
            knight_status = 1;
        }
        //End turn
        if(database.eof() && rescue == -1) rescue = 1;
        display_state;
        if(rescue > -1) break;
        nth_event++;
    }
    database.str("");
    database.clear();
}  

void getinput(string file_input,  int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown, int &rescue, string &events_code, string &event_pack){
    ifstream inputdata(file_input);
    string knight_data, events_code_error;
    getline(inputdata, knight_data);
    getline(inputdata, events_code_error);
    getline(inputdata, event_pack);
    inputknight(knight_data, HP, level, remedy, maidenkiss, phoenixdown, rescue);
    int i = events_code_error.size() - 1;
    while(true){
        if(events_code_error[i] >= '0' && events_code_error[i] <= '9') break;   
        i--;
    }
    events_code = events_code_error.substr(0, i + 1);
    // display_state;   
}

void inputknight(string knight_data, int &HP, int &level, int &remedy, int &maidenkiss, int &phoenixdown, int &rescue){
    stringstream database;
    database << knight_data;
    database >> HP >> level >> remedy >> maidenkiss >> phoenixdown;
    database.str("");
    database.clear();
}

void returnMaxHP(int &HP, int MaxHP){
    if(HP > MaxHP) HP = MaxHP;
}

void returnMaxLv(int &level){
    if(level > LevelMax) level = LevelMax;
}

void returnMaxRemedy(int &remedy){
    if(remedy > RemedyMax) remedy = RemedyMax;
}

void returnMaxMaidenKiss(int &maidenkiss){
    if(maidenkiss > MaidenKissMax) maidenkiss = MaidenKissMax;
}

void returnMaxPhoenixDown(int &phoenixdown){
    if(phoenixdown > PhoenixDownMax) phoenixdown = PhoenixDownMax;
}

void checkLancelot(int MaxHP, int &special_class){
    if(Eratosthenes(MaxHP)) special_class = 1; 
}

void checkArthur(int MaxHP, int &special_class){
    if(MaxHP == 999) special_class = 2;
}

void combat_creep(int nth_event, int &HP, int &level, int special_class, int monster_type){
    if(special_class > 0){
        level++;
        returnMaxLv(level);
    } 
    else{
        if(combat_result(nth_event, level) == "WIN"){
            level++;
            returnMaxLv(level);
        }
        if(combat_result(nth_event, level) == "LOSE"){
            int levelO, b;
            b = nth_event % 10;
            levelO = (nth_event > 6)?((b > 5)?b:5):b;
            int base_damage; 
            switch (monster_type){
            case 1:
                base_damage = MadBear;
                break;
            case 2:
                base_damage = Bandit;
                break;
            case 3: 
                base_damage = LordLupin;
                break;
            case 4: 
                base_damage = Elf;
                break;
            case 5: 
                base_damage = Troll;
                break;
            }
            HP = HP - base_damage * levelO;
        }
    }
}

void combat_shaman(int nth_event, int &HP, int &level , int &remedy, int special_class, int &knight_status, int &debuff_state){
    if(special_class > 0){
        level += 2;
        returnMaxLv(level);
    }else{
        if(combat_result(nth_event, level) == "WIN"){
            level += 2;
            returnMaxLv(level);
        }
        if(combat_result(nth_event, level) == "LOSE"){
            if(remedy > NoRemedy) remedy--;
            else{
                if(HP < 5) HP = 1;
                else HP /= 5;
                knight_status = 2;
                debuff_state = 3;
            }
        }
    }
}

void combat_Vajsh(int nth_event, int &HP, int &level, int &level_before_frog, int &maidenkiss, int special_class, int &knight_status, int &debuff_state){
    if(special_class > 0) {
        level += 2;
        returnMaxLv(level);
    }else{
        if(combat_result(nth_event, level) == "WIN"){
            level += 2;
            returnMaxLv(level);
        }
        if(combat_result(nth_event, level) == "LOSE"){
            if(maidenkiss > NoMaidenKiss) maidenkiss--;
            else{
                level_before_frog = level;
                level = 1;
                knight_status = 3;
                debuff_state = 3;
            }
        }
    }     
}

void combat_boss (int &level, int &rescue, int special_class){
    if(special_class == 2) level = 10;
    if(special_class == 1){
        if(level >= 8) level = 10;
        else rescue = 0;
    }
    if(special_class == 0 && level != 10) rescue = 0;
}

void take_MushMario(int &HP, int MaxHP, int level, int phoenixdown){    
    int n1 = ((level + phoenixdown) % 5 + 1) * 3;
    int s1 = 0;
    for (int i = 0; i < n1; i++){
        s1 += (99 - i * 2);
    }
    HP = HP + (s1 % 100);
    returnMaxHP(HP, MaxHP);
    if(HP < MaxHP){
        int tmp1 = HP + 1;
        while((!Eratosthenes(tmp1)) && tmp1 < MaxHP) tmp1++;
        HP = tmp1;
    }
}

void take_FibonacciMushFibo (int &HP){
    if(HP > 1){
        int i = 1, j = 1, tmp2;
        while(i < HP){
            tmp2 = i;
            i = i + j;
            j = tmp2;
        }
        HP = j;
    }
}

void take_MushGhost(int &HP, int MaxHP, int &phoenixdown, string file_mush_ghost, string MushGhosts_type){
    //Setting data for MushGhosts
    ifstream inputdata(file_mush_ghost);
    int n2; inputdata >> n2;
    string Num_Of_Num, Num_Process; 
    getline(inputdata, Num_Of_Num);
    getline(inputdata, Num_Process);
    int* n2arr = new int[n2];
    for(int i = 0; i < Num_Process.size(); i++){
        if(Num_Process[i] == ',') Num_Process[i] = ' ';
    }
    stringstream database; 
    database << Num_Process;
    for (int i = 0; i < n2; i++) database >> n2arr[i];
    //Taking MushGhosts
    for (int i = 0; i < MushGhosts_type.size(); i++){
        int MushGhost_type = MushGhosts_type[i] - '0';
        switch(MushGhost_type){
        case 1:
            MushGhost_type1(HP, n2, n2arr);
            break;
        case 2:
            MushGhost_type2(HP, n2, n2arr);
            break;
        case 3: 
            MushGhost_type3(HP, n2, n2arr);
            break;
        case 4:
            MushGhost_type4(HP, n2, n2arr);
            break;
        }
        returnMaxHP(HP, MaxHP);
        if(HP <= DeadHP){
            if(phoenixdown == NoPhoenixDown) break;
            else{
                HP = MaxHP;
                phoenixdown--;
            }
        }
    }
    delete[] n2arr;
    database.str("");
    database.clear();
}

void take_glossary(int &remedy, int &maidenkiss, int &phoenixdown, int medicine_type){
    switch (medicine_type){
    case 15:
        remedy++;
        returnMaxRemedy(remedy);
        break;
    case 16:
        maidenkiss++;
        returnMaxMaidenKiss(maidenkiss);
        break;  
    case 17:
        phoenixdown++;
        returnMaxPhoenixDown(phoenixdown);
        break;
    }
}

void meet_Asclepius(int &remedy, int &maidenkiss, int &phoenixdown, string file_aclepius_pack){
    ifstream inputdata(file_aclepius_pack);
    string Num_Of_Row, Num_Of_Num;
    int r1; inputdata >> r1;
    getline(inputdata, Num_Of_Row);
    int c1; inputdata >> c1;
    getline(inputdata, Num_Of_Num);
    stringstream database;
    int* c1arr = new int[c1];
    string medicine_line;
    for(int i = 0; i < r1; i++){
        int medicine_taking = 3;
        getline(inputdata, medicine_line);
        database << medicine_line;
        for(int j = 0; j < c1; j++){
            if(medicine_taking == 0) break;
            database >> c1arr[j];
            if(c1arr[j] == 16){
                remedy++;
                medicine_taking--;
            }
            if(c1arr[j] == 17){
                maidenkiss++;
                medicine_taking--;      
            }
            if(c1arr[j] == 18){
                phoenixdown++;
                medicine_taking--;
            }
        }
        database.str("");
        database.clear();
    }
    delete[] c1arr;
}

void meet_MajoMerlin(int &HP, string file_merlin_pack){
    ifstream inputdata(file_merlin_pack);
    string Num_Of_MagicThing;
    int n9; inputdata >> n9;
    getline(inputdata, Num_Of_MagicThing);
    string MagicThing_data;
    for(int i = 0; i < n9; i++){
        getline(inputdata, MagicThing_data);
        if(MagicThing_data.find("Merlin") != string::npos || MagicThing_data.find("merlin") != string::npos) HP += 3;
        else if((MagicThing_data.find('M') != string::npos || MagicThing_data.find('m') != string::npos) &&
                (MagicThing_data.find("E") != string::npos || MagicThing_data.find('e') != string::npos) && 
                (MagicThing_data.find("R") != string::npos || MagicThing_data.find('r') != string::npos) &&
                (MagicThing_data.find("L") != string::npos || MagicThing_data.find('l') != string::npos) &&
                (MagicThing_data.find("I") != string::npos || MagicThing_data.find('i') != string::npos) &&
                (MagicThing_data.find("N") != string::npos || MagicThing_data.find('n') != string::npos)   ) HP += 2;
    }
}

string combat_result(int nth_event, int level){
    int levelO, b;
    b = nth_event % 10;
    levelO = (nth_event > 6)?((b > 5)?b:5):b;
    if (level > levelO) return "WIN";
    else if (level == levelO) return "DRAW";
        else return "LOSE";
}

void MushGhost_type1(int &HP, int n2, int* n2arr){
    int maxi = 0, mini = 0, max = n2arr[0], min = n2arr[0]; 
    for (int i = 0; i < n2; i++){
        if(n2arr[i] >= max) {
            maxi = i;
            max = n2arr[i];
        }
        else if(n2arr[i] <= min){
            mini = i;
            min = n2arr[i];
        }
    }
    HP = HP - (maxi + mini);
}

void MushGhost_type2(int &HP, int n2, int* n2arr){
    int mtx = n2arr[0], mti = 0, i = 0;
    bool checkDown = true;
    while(n2arr[i] < n2arr[i + 1] && i + 1 < n2) i++;
    if(i + 1 == n2){
        mti = i;
        mtx = n2arr[mti];
    } else {
        for(int j = i; j + 1 < n2; j++){ 
            if(n2arr[j] <= n2arr[j + 1]){ 
                mti = -3;
                mtx = -2;
                checkDown = false;
                break;
            }
        }
    }
    if(checkDown){
        mti = i;
        mtx = n2arr[mti];
    }
    HP = HP - (mtx + mti);
}

void MushGhost_type3(int &HP, int n2, int* n2arr1){
    int* n2arr = new int[n2];
    for(int i = 0; i < n2; i++){
        if(n2arr1[i] < 0) n2arr[i] = n2arr1[i]*(-1);
        else n2arr[i] = n2arr1[i];
        n2arr[i] = (17 * n2arr[i] + 9) % 257;
    }
    int maxi2 = 0, mini2 = 0, max = n2arr[0], min = n2arr[0]; 
    for (int i = 0; i < n2; i++){
        if(n2arr[i] > max){
            maxi2 = i;
            max = n2arr[i];
        }
        else if(n2arr[i] < min){
            mini2 = i;
            min = n2arr[i];
        }
    }
    HP = HP - (maxi2 + mini2);
    delete[] n2arr;
}

void MushGhost_type4(int &HP, int n2, int* n2arr1){
    int* n2arr = new int[n2];
    for(int i = 0; i < n2; i++){
        if(n2arr1[i] < 0) n2arr[i] = n2arr1[i]*(-1);
        else n2arr[i] = n2arr1[i];
        n2arr[i] = (17 * n2arr[i] + 9) % 257;
    }
    int max2_3x, max2_3i;
    if(n2 >= 3){
        if(n2arr[0] == n2arr[1] && n2arr[0] == n2arr[2]){
            max2_3x = -5; 
            max2_3i = -7;
        } else {
            int max_3i = 0, max_3x = n2arr[0];
            max2_3x = -1; 
            for (int i = 1; i < 3; i++){
                if(n2arr[i] > max_3x){
                    max2_3i = max_3i;
                    max2_3x = max_3x;
                    max_3i = i;
                    max_3x = n2arr[i];
                } else if(n2arr[i] < max_3x) {
                    if(n2arr[i] > max2_3x){
                        max2_3i = i;
                        max2_3x = n2arr[i];
                    }
                }
            }
        }
    } else if(n2 == 2){
        if(n2arr[0] == n2arr[1]){
            max2_3x = -5; 
            max2_3i = -7;
        } else {
            if(n2arr[0] > n2arr[1]){
                max2_3x = n2arr[1];
                max2_3i = 1;
            } else {
                max2_3i = 0;
                max2_3x = n2arr[0];
            }
        }
    }else{
        max2_3x = -5; 
        max2_3i = -7;
    } 
    HP = HP - (max2_3x + max2_3i);
    delete[] n2arr;
}

bool Eratosthenes(int HP){
    bool checkPrime[1000];
    for(int i = 0; i <= 999; i++) checkPrime[i] = true;
    checkPrime[0] = false; checkPrime[1] = false;
    for(int i = 2; i * i < 1000; i++){
        if(checkPrime[i] == true){
            for(int j = i * i; j < 1000; j += i){
                checkPrime[j] = false;
            }
        }
    }
    if(checkPrime[HP]) return true;
    else return false;
}

//Program finished by Tuan on 21/02/2023 at 4:50 PM