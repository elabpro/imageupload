/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DB.h
 * Author: eugine
 *
 * Created on 13 декабря 2018 г., 16:00
 */

#ifndef DB_H
#define DB_H

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/address.hpp>

#include <redisclient/redissyncclient.h>
#include <iostream>
#include <functional>

class DB {
public:
    DB(std::string host);
    DB(const DB& orig);
    virtual ~DB();
    void setCounter(int value);
    int getCounter();
    int getCurrentCounter();
private:
    std::string redisHost = "localhost";
    redisclient::RedisSyncClient connect();
    const std::string redisKey = "counter";
};

#endif /* DB_H */

