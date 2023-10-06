#include "aes128.hpp"

#include <iostream>
#include <stdlib.h>

void Aes128::Encryption(std::vector<unsigned char>* input, unsigned char Key[16]){ 
    unsigned char* _input = new unsigned char[16];
    short _k = 0;
    //std::cout << input->size() << std::endl;
    std::size_t _size = input->size();
    for(std::size_t _i = 0; _i < _size; _i++){
        if(_i%16 == 0){
            _k = 0;
            if(_i != 0){
                EncryptionPiece(_input, Key);
                for(short _j = 0; _j < 16; _j++)
                    (*input)[_i-16+_j] = _input[_j];
            }
        }
        else{
            _k++;
        } 
        _input[_k] = (*input)[_i];
    }
}

void Aes128::Decryption(std::vector<unsigned char>* input, unsigned char Key[16]){ 
    unsigned char* _input = new unsigned char[16];
    short _k = 0;
    std::size_t _size = input->size();
    for(std::size_t _i = 0; _i < _size; _i++){
        if(_i%16 == 0){
            _k = 0;
            if(_i != 0){
                DecryptionPiece(_input, Key);
                for(short _j = 0; _j < 16; _j++)
                    (*input)[_i-16+_j] = _input[_j];
            }
        }
        else{
            _k++;
        } 
        _input[_k] = (*input)[_i];
    }
}

void Aes128::EncryptionPiece(unsigned char* input, unsigned char roundKey[16]){
    unsigned char _roundKey[16];
    for(short _i = 0; _i < 16; _i++){
        _roundKey[_i] = roundKey[_i];
    }
    for(short roundVal = 0; roundVal < 11; roundVal++){
        if(roundVal == 0){
            AddRoundKey(input, _roundKey);
        }else if(roundVal == 10){
            KeySchedule(_roundKey, (roundVal-1));
            SubstituteBytes(input);
            ShiftRows(input);
            AddRoundKey(input, _roundKey);
        }else{
            KeySchedule(_roundKey, (roundVal-1));
            Round(input, _roundKey);
        }
    }
}

void Aes128::CalculateKey(unsigned char* roundKey, short round){
    for(short i = 0; i < (round+1); i++){
        KeySchedule(roundKey, i);
    }
}

void Aes128::DecryptionPiece(unsigned char* input, unsigned char roundKey[16]){
    unsigned char _roundKey[16];
    for(short _i = 0; _i < 16; _i++){
        _roundKey[_i] = roundKey[_i];
    }
    CalculateKey(_roundKey, 9);
    for(short roundVal = 10; roundVal >= 0; roundVal--){
        if(roundVal == 0){
            InvKeySchedule(_roundKey, roundVal);
            AddRoundKey(input, _roundKey);
        }else if(roundVal == 10){
            AddRoundKey(input, _roundKey);
            InvShiftRows(input);
            InvSubstituteBytes(input);          
        }else{
            InvKeySchedule(_roundKey, roundVal);
            InvRound(input, _roundKey);
        }
    }
}

void Aes128::Round(unsigned char* input, unsigned char* roundKey){
    SubstituteBytes(input);
    ShiftRows(input);
    MixColumns(input);
    AddRoundKey(input, roundKey);
}

void Aes128::InvRound(unsigned char* input, unsigned char* roundKey){
    AddRoundKey(input, roundKey);
    InvMixColumns(input);
    InvShiftRows(input);
    InvSubstituteBytes(input);
}

void Aes128::SubstituteBytes(unsigned char* input){
    for(short _i = 0; _i<16; _i++){
        input[_i] = sbox[input[_i]];
    }
}

void Aes128::InvSubstituteBytes(unsigned char* input){
    for(short _i = 0; _i<16; _i++){
        input[_i] = inverse_sbox[input[_i]];
    }
}

void Aes128::ShiftRows(unsigned char* input){
    unsigned char _trush;
    _trush = input[4];
    input[4] = input[5]; input[5] = input[6]; input[6] = input[7]; input[7] = _trush;
    _trush = input[8];
    input[8] = input[10]; input[10] = _trush;
    _trush = input[9];
    input[9] = input[11]; input[11] = _trush;
    _trush = input[15];
    input[15] = input[14]; input[14] = input[13]; input[13] = input[12]; input[12] = _trush;
}

void Aes128::InvShiftRows(unsigned char* input){
    unsigned char _trush;
    _trush = input[7];
    input[7] = input[6]; input[6] = input[5]; input[5] = input[4]; input[4] = _trush;
    _trush = input[8];
    input[8] = input[10]; input[10] = _trush;
    _trush = input[9];
    input[9] = input[11]; input[11] = _trush;
    _trush = input[12];
    input[12] = input[13]; input[13] = input[14]; input[14] = input[15]; input[15] = _trush;
}

void Aes128::MixColumns(unsigned char* input){
    for(short _j = 0; _j<4; _j++){
        unsigned char s0 = input[_j + 0];
        unsigned char s1 = input[_j + 4];
        unsigned char s2 = input[_j + 8];
        unsigned char s3 = input[_j + 12];

        input[_j +  0] = mul2[s0] ^ mul3[s1] ^ s2       ^ s3;
        input[_j +  4] = s0       ^ mul2[s1] ^ mul3[s2] ^ s3;
        input[_j +  8] = s0       ^ s1       ^ mul2[s2] ^ mul3[s3];
        input[_j + 12] = mul3[s0] ^ s1       ^ s2       ^ mul2[s3];
    }
}

void Aes128::InvMixColumns(unsigned char* input){
    for(short _j = 0; _j<4; _j++){
        unsigned char s0 = input[_j + 0];
        unsigned char s1 = input[_j + 4];
        unsigned char s2 = input[_j + 8];
        unsigned char s3 = input[_j + 12];

        input[_j +  0] = mulE[s0] ^ mulB[s1] ^ mulD[s2] ^ mul9[s3];
        input[_j +  4] = mul9[s0] ^ mulE[s1] ^ mulB[s2] ^ mulD[s3];
        input[_j +  8] = mulD[s0] ^ mul9[s1] ^ mulE[s2] ^ mulB[s3];
        input[_j + 12] = mulB[s0] ^ mulD[s1] ^ mul9[s2] ^ mulE[s3];
    }
}

void Aes128::AddRoundKey(unsigned char* input, unsigned char* roundKey){
    for(short _i = 0; _i<16; _i++){
        input[_i] = input[_i] ^ roundKey[_i];
    }
}

void Aes128::KeySchedule(unsigned char* roundKey, short rconVal){
    unsigned char s0[4] = {roundKey[0], roundKey[4], roundKey[8], roundKey[12]};
    unsigned char s1[4] = {roundKey[1], roundKey[5], roundKey[9], roundKey[13]};
    unsigned char s2[4] = {roundKey[2], roundKey[6], roundKey[10], roundKey[14]};
    unsigned char s3[4] = {roundKey[3], roundKey[7], roundKey[11], roundKey[15]};
    unsigned char s3_c[4] = {roundKey[3], roundKey[7], roundKey[11], roundKey[15]};

    unsigned char _trush = s3_c[0];
    s3_c[0] = s3_c[1]; s3_c[1] = s3_c[2]; s3_c[2] = s3_c[3]; s3_c[3] = _trush;

    for(short _i = 0; _i<4; _i++){
        s3_c[_i] = sbox[s3_c[_i]];

        s0[_i] = s0[_i] ^ s3_c[_i];

        if (_i == 0)
            s0[_i] = s0[_i] ^ rcon[rconVal];
        else
            s0[_i] = s0[_i] ^ 0x00;

        roundKey[_i*4] = s0[_i];

        _trush = roundKey[_i*4];
        _trush = _trush ^ s1[_i];
        roundKey[_i*4+1] = _trush;

        _trush = roundKey[_i*4+1];
        _trush = _trush ^ s2[_i];
        roundKey[_i*4+2] = _trush;

        _trush = roundKey[_i*4+2];
        _trush = _trush ^ s3[_i];
        roundKey[_i*4+3] = _trush;
    }
}

void Aes128::InvKeySchedule(unsigned char* roundKey, short rconVal){
    unsigned char s0[4];
    unsigned char s1[4] = {roundKey[1], roundKey[5], roundKey[9], roundKey[13]};
    unsigned char s2[4] = {roundKey[2], roundKey[6], roundKey[10], roundKey[14]};
    unsigned char s3[4] = {roundKey[3], roundKey[7], roundKey[11], roundKey[15]};

    unsigned char _trush;

    for(short _i = 0; _i<4; _i++){
        _trush = roundKey[_i*4+2];
        _trush = _trush ^ s3[_i];
        roundKey[_i*4+3] = _trush;

        _trush = roundKey[_i*4+1];
        _trush = _trush ^ s2[_i];
        roundKey[_i*4+2] = _trush;

        _trush = roundKey[_i*4];
        _trush = _trush ^ s1[_i];
        roundKey[_i*4+1] = _trush;

        if (_i == 0)
            roundKey[_i*4] = roundKey[_i*4] ^ rcon[rconVal];
    }

    s0[0] = roundKey[7]; s0[1] = roundKey[11]; s0[2] = roundKey[15]; s0[3] = roundKey[3];

    for(short _i = 0; _i<4; _i++){
        s0[_i] = sbox[s0[_i]];

        roundKey[_i*4] = roundKey[_i*4] ^ s0[_i];
    }
}
