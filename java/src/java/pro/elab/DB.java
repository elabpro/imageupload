/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package pro.elab;

import redis.clients.jedis.Jedis;

/**
 *
 * @author eugine
 */
public class DB {

    Jedis redis;

    DB() {
        redis = new Jedis("localhost");
    }

    public int setCounter(int v) {
        redis.set("counter", Integer.toString(v));
        return v;
    }

    public int getCounter() {
        return redis.incr("counter").intValue();
    }

    public int getCurrentCounter() {
        return Integer.valueOf(redis.get("counter"));
    }
}
