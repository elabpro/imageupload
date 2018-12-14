/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Config.cpp
 * Author: eugine
 * 
 * Created on 14 декабря 2018 г., 13:13
 */

#include "Config.h"

Config::Config() {
}

Config::Config(const Config& orig) {
}

Config::~Config() {
}

/**
 * Replace all parts in string
 * @param str
 * @param from
 * @param to
 */
void Config::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

/**
 * Show config
 */
void Config::print() {
    std::cout << "------------------------------" << std::endl;
    std::cout << "redisHost:\t" << redisHost << std::endl;
    std::cout << "listenHost:\t" << listenHost << std::endl;
    std::cout << "listenPort:\t" << listenPort << std::endl;
    std::cout << "imagesDir:\t" << imagesDir << std::endl;
    std::cout << "imagesThumbDir:\t" << imagesThumbDir << std::endl;
    std::cout << "------------------------------" << std::endl;
}
