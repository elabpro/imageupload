<?php

/**
 * Обертка для работы с БД на основе Redis
 */

namespace REST;

require_once 'vendor/autoload.php';

\Predis\Autoloader::register();

class DB {

    private $redis;
    private $initCounter = 0;

    public function __construct() {
        global $redisHost,$redisPort;
        try {
            if (isset($redisHost) && isset($redisPort)) {
                $this->redis = new \Predis\Client(array(
                    "scheme" => "tcp",
                    "host" => $redisHost,
                    "port" => $redisPort
                ));
            } else {
                $this->redis = new \Predis\Client();
            }
//            $this->redis->set("counter", $this->initCounter);
        } catch (Exception $e) {
            die($e->getMessage());
        }
    }

    public function setCounter($v = 0) {
        if (is_object($this->redis)) {
            $this->redis->set("counter", $v);
        }
    }

    public function getCounter() {
        return $this->redis->incr("counter");
    }

    public function getCurrentCounter() {
        return $this->redis->get("counter");
    }

}
