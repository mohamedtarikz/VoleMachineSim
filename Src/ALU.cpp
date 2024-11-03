#include "VoleMachine.h"
#include <regex>
#include <valarray>

// Convert a decimal number to its floating-point binary representation
string ALU::cnvrtToFloatingPoint(double dec) {
    char sign = '0'; // Determine the sign bit
    if(dec < 0){
        sign = '1';
        dec *= -1;
    }
    int intPart = (int)dec; // Extract the integer part
    double fracPart = dec - intPart; // Extract the fractional part
    string intPartBin = decToBin(intPart,0); // Convert integer part to binary
    string fracPartBin;
    while(fracPart > 0){
        fracPart *= 2;
        if(fracPart >= 1){
            fracPartBin += '1';
            fracPart -= 1;
        }
        else{
            fracPartBin += '0';
        }
    }
    string all = intPartBin + fracPartBin; // Combine integer and fractional parts
    while(all.size() < 8){
        all += '0';
    }
    int idx = 0;
    while(all[idx] == '0' && idx < all.size()){
        idx++;
    }
    int exp = intPartBin.size() - idx + 3; // Calculate the exponent
    if(!(exp >= 0 && exp <= 7)){
        throw runtime_error("Floating point overflow");
    }
    string mant = all.substr(idx + 1, 4); // Extract the mantissa
    return sign + decToBin(exp, 3) + mant; // Combine sign, exponent, and mantissa
}

// Convert a binary string to its two's complement integer representation
int ALU::cnvrtTwosComplement(string bin) {
    int ret = 0;
    for(int i = 0; i < bin.size(); ++i){
        if(i == 0 && bin[i] == '1'){
            ret -= pow(2, bin.size() - 1);
        }
        else{
            ret += (bin[i] - '0') * pow(2, bin.size() - i - 1);
        }
    }
    return ret;
}

// Convert a binary string to its decimal representation
int ALU::binToDec(string bin) {
    int ret = 0;
    int idx = bin.size() - 1;
    for (int i = 0; i < bin.size(); ++i) {
        ret += (bin[i] - '0') * pow(2, idx);
        idx--;
    }
    return ret;
}

// Convert a decimal number to its binary representation with a specified size
string ALU::decToBin(int dec, int size) {
    string ret;
    while(dec > 0){
        ret += to_string(dec % 2);
        dec /= 2;
    }
    while(ret.size() < size){
        ret += '0';
    }
    reverse(ret.begin(), ret.end());
    return ret;
}

// Validate if a given hexadecimal string represents a valid instruction
bool ALU::isValid(string hex) {
    bool valid = false;
    if(hex[0] == '1' || hex[0] == '2' || hex[0] == '3' || hex[0] == '5' || hex[0] == '6' || hex[0] == '7' || hex[0] == '8' || hex[0] == '9' || hex [0] == 'B' || hex[0] == 'D'){
        valid |= regex_match(hex, regex("(1|2|3|5|6|7|8|9|B|D)[0-9A-F]{3}"));
    }
    else if(hex[0] == '4'){
        valid |= regex_match(hex, regex("40[0-9A-F]{2}"));
    }
    else if(hex[0] == 'A'){
        valid |= regex_match(hex, regex("A[0-9A-F]0[0-9A-F]"));
    }
    else if(hex[0] == 'C'){
        valid |= regex_match(hex, regex("C[0]{3}"));
    }
    return valid;
}

// Convert a hexadecimal string to its decimal representation
int ALU::hexToDec(string hex) {
    int ret = 0;
    int idx = hex.size() - 1;
    for (int i = 0; i < hex.size(); ++i) {
        if(hex[i] >= 'A' && hex[i] <= 'F'){
            ret += (hex[i] - 'A' + 10) * pow(16, idx);
        }
        else {
            ret += (hex[i] - '0') * pow(16, idx);
        }
        idx--;
    }
    return ret;
}

// Convert a decimal number to its hexadecimal representation
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
    while(ret.size() < 2){
        ret += '0';
    }
    reverse(ret.begin(), ret.end());
    return ret;
}

// Sum two floating-point numbers and store the result in a register
void ALU::sumFloatingPoint(int idxRegister1, int idxRegister2, int idxRegister3, Register& reg) {
    string s1 = decToBin(reg.getCell(idxRegister2));
    string s2 = decToBin(reg.getCell(idxRegister3));
    int exp1 = binToDec(s1.substr(1, 3)) - 4;
    int exp2 = binToDec(s2.substr(1, 3)) - 4;
    double mant1 = binToDec(s1.substr(4, 4)) / 16.0 + 1;
    double mant2 = binToDec(s2.substr(4, 4)) / 16.0 + 1;
    if(exp1 > exp2){
        swap(exp1, exp2);
        swap(mant1, mant2);
    }
    while(exp2 > exp1){
        mant1 *= 2;
        exp1--;
    }
    double ans = mant1 * pow(2, exp1) * pow(-1, s1[0] - '0') + mant2 * pow(2, exp2) * pow(-1, s2[0] - '0');
    if(!(ans >= -31 && ans <= 31)){
        throw runtime_error("Floating point overflow");
    }
    string s = cnvrtToFloatingPoint(ans);
    reg.setCell(idxRegister1, binToDec(s));
}

// Sum two two's complement numbers and store the result in a register
void ALU::sumTwosComplement(int idxRegister1, int idxRegister2, int idxRegister3, Register &reg) {
    string a = decToBin(reg.getCell(idxRegister2), 8);
    string b = decToBin(reg.getCell(idxRegister3), 8);
    int x = cnvrtTwosComplement(a);
    int y = cnvrtTwosComplement(b);
    int z = x + y;
    if(!(z >= -128 && z <= 127)){
        throw runtime_error("Two's complement overflow");
    }
    if(z < 0){
        string s = decToBin(-z, 8);
        for(int i = 0; s[i] != '1' && i < s.size(); ++i){
            s[i] = (s[i] == '0' ? '1' : '0');
        }
        z = binToDec(s);
    }
    reg.setCell(idxRegister1, z);
}

void ALU::orOperator(int idxRegister1, int idxRegister2, int idxRegister3, Register& reg){
    reg.setCell(idxRegister1, reg.getCell(idxRegister2) | reg.getCell(idxRegister3));
}

void ALU::andOperator(int idxRegister1, int idxRegister2, int idxRegister3, Register& reg){
    reg.setCell(idxRegister1, reg.getCell(idxRegister2) & reg.getCell(idxRegister3));
}

void ALU::xorOperator(int idxRegister1, int idxRegister2, int idxRegister3, Register& reg){
    reg.setCell(idxRegister1, reg.getCell(idxRegister2) ^ reg.getCell(idxRegister3));
}

void ALU::rotation(int idxRegister, int n, Register& reg){
    string str = decToBin(reg.getCell(idxRegister));
    string result;

    for(int i = str.size() - n; i < str.size(); i++){
        result += str[i];
    }
    for (int i = 0; i < str.size() - n; i++) {
        result += str[i];
    }

    reg.setCell(idxRegister, binToDec(result));
}
