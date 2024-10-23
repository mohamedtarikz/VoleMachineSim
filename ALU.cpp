#include "VoleMachineSim.h"
#include <regex>
#include <valarray>

bool ALU::isValid(string hex) {
    bool valid = false;
    if(hex[0] == '1' || hex[0] == '2' || hex[0] == '3' || hex[0] == '5' || hex[0] == '6' || hex [0] == 'B'){
        valid |= regex_match(hex, regex("(1|2|3|5|6|B)[0-9A-F]{3}"));
    }
    else if(hex[0] == '4'){
        valid |= regex_match(hex, regex("40[0-9A-F]{2}"));
    }
    else if(hex[0] == 'C'){
        valid |= regex_match(hex, regex("C[0]{3}"));
    }
    return valid;
}

int ALU::hexToDec(string hex) {
    int ret = 0;
    int idx = hex.size() - 1;
    for (int i = 0; i < hex.size(); ++i) {
        ret += (hex[i] - '0') * pow(16, idx);
        idx--;
    }
    return ret;
}

string ALU::decToHex(int dec) {
    string ret;
    while(dec > 0){
        int rem = dec % 16;
        if(rem < 10){
            ret += to_string(rem);
        }
        else{
            ret += (char)('A' + rem - 10);
        }
        dec /= 16;
    }
    reverse(ret.begin(), ret.end());
    return ret;
}