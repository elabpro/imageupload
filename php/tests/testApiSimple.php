<?php

/*
 * Тестирование API
 */
require_once '../include/config.php';

use PHPUnit\Framework\TestCase;

class ApiSimpleTest extends TestCase {

    var $apiURL = "http://localhost:8080";

    protected function setUp() {
        global $apiURL;

        $this->apiURL = $apiURL;
    }

    /**
     * Проверяем скачивание списка файлов
     *
     * @global type $imagesDir
     * @global type $apiURL
     */
    public function testGetFiles() {
        ;
        // Проверка загрузки файла
        $cmd = "curl -s -X GET " . $this->apiURL . "/api/images";
        $data = exec($cmd);
        $ids = json_decode($data);
        $this->assertTrue(is_array($ids));
    }

    /**
     * Проверяем скачивание списка файла
     *
     * @global type $imagesDir
     * @global type $apiURL
     */
    public function testGetFile() {
        // Проверка загрузки файла
        $tmpFilename = tempnam("/tmp", "test");
        $cmd = "curl -s -X GET " . $this->apiURL . "/api/images/1 --output " . $tmpFilename;
        exec($cmd);
        $this->assertTrue(file_exists($tmpFilename));
        $this->assertEquals(filesize("data/test.png"), filesize($tmpFilename));
        unlink($tmpFilename);
    }

}
