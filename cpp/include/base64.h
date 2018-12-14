/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   base64.h
 * Author: eugine
 *
 * Created on 14 декабря 2018 г., 9:18
 */

#ifndef BASE64_H
#define BASE64_H

#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

class base64 {
public:
    base64();
    base64(const base64& orig);
    virtual ~base64();
    string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
    string base64_decode(string const& encoded_string);
private:
    const string BASE64CHARS =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz"
            "0123456789+/";
    inline bool is_base64(unsigned char c);
};

#endif /* BASE64_H */

