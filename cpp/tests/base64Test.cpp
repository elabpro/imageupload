/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   base64Test.cpp
 * Author: eugine
 *
 * Created on 14 декабря 2018 г., 9:36
 */

#include <stdlib.h>
#include <iostream>
#include "base64.h"

/*
 * Simple C++ Test Suite
 */

void test1() {
    std::cout << "base64Test test 1" << std::endl;
    base64 *b = new base64();
    std::string s = "";
    std::string result = b->base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
    if (result.compare(s) != 0) {
        std::cout << "%TEST_FAILED% time=0 testname=test1 (base64Test) message=" << result << std::endl;
    }
    s = "abc";
    std::string expectedResult = "YWJj";
    result = b->base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
    if (result.compare(expectedResult) != 0) {
        std::cout << "%TEST_FAILED% time=0 testname=test1 (base64Test) message=" << result << std::endl;
    }
}

void test2() {
    std::cout << "base64Test test 2" << std::endl;
    base64 *b = new base64();
    std::string s = "";
    std::string result = b->base64_decode(s);
    if (result.compare(s) != 0) {
        std::cout << "%TEST_FAILED% time=0 testname=test2 (base64Test) message=" << result << std::endl;
    }
    s = "YWJj";
    std::string expectedResult = "abc";
    result = b->base64_decode(s);
    if (result.compare(expectedResult) != 0) {
        std::cout << "%TEST_FAILED% time=0 testname=test2 (base64Test) message=" << result << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% base64Test" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (base64Test)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (base64Test)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (base64Test)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (base64Test)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

