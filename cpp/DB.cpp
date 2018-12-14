/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DB.cpp
 * Author: eugine
 * 
 * Created on 13 декабря 2018 г., 16:00
 */

#include "DB.h"

DB::DB() {

}

DB::DB(const DB& orig) {
}

DB::~DB() {
}

redisclient::RedisSyncClient DB::connect() {
    boost::asio::ip::address address = boost::asio::ip::address::from_string("127.0.0.1");
    const unsigned short port = 6379;
    boost::asio::ip::tcp::endpoint endpoint(address, port);

    boost::asio::io_service ioService;
    redisclient::RedisSyncClient redis(ioService);
    boost::system::error_code ec;

    redis.connect(endpoint, ec);
    return redis;
}

void DB::setCounter(int value) {
    redisclient::RedisSyncClient redis = this->connect();
    redisclient::RedisValue result;
    const std::string v = std::to_string(value);
    result = redis.command("SET",{redisKey, v});

    if (result.isError()) {
        std::cout << "SET error: " << result.toString() << "\n";
    }
}

int DB::getCounter() {
    redisclient::RedisSyncClient redis = this->connect();
    redisclient::RedisValue result;
    result = redis.command("INCR",{redisKey});

    if (result.isOk()) {
        return result.toInt();
    } else {
        std::cerr << "GET error: " << result.toString() << "\n";
        return -1;
    }
}

int DB::getCurrentCounter() {
    redisclient::RedisSyncClient redis = this->connect();
    redisclient::RedisValue result;
    result = redis.command("GET",{redisKey});

    if (result.isOk()) {
        return atoi(result.toString().c_str());
    } else {
        std::cout << "GET error: " << result.toString() << "\n";
        return -1;
    }
}
