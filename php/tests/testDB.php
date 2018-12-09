<?php

/*
 * Тест для обертки БД
 */
require_once '../src/db.php';

use PHPUnit\Framework\TestCase;

class DBTest extends TestCase {

    public function testConnect() {
        $db = new \REST\DB();
        $db->setCounter(0);
        $this->assertEquals(1, $db->getCounter());
        $this->assertEquals(2, $db->getCounter());
        $db->setCounter(1);
        $this->assertEquals(2, $db->getCounter());
        $this->assertEquals(2, $db->getCurrentCounter());
    }

}
