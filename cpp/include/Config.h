/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Config.h
 * Author: eugine
 *
 * Created on 14 декабря 2018 г., 13:13
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>

class Config {
public:
    Config();
    Config(const Config& orig);
    virtual ~Config();

    std::string redisHost = "localhost";
    std::string listenHost = "localhost";
    int listenPort = 8080;
    std::string imagesDir = "images.full";
    std::string imagesThumbDir = "images.thumb";

    void replaceAll(std::string& str, const std::string& from, const std::string& to);
    void print();
private:

};

#endif /* CONFIG_H */

