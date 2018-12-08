<?php

/*
 * Тестирование API
 */
require_once '../include/config.php';
require_once '../src/file.php';
require_once 'HttpRequest.php';

use PHPUnit\Framework\TestCase;

class ApiTest extends TestCase {

    var $apiURL = "htt://localhost:8080/api/images";

    protected function setUp() {
        global $apiURL;

        $this->apiURL = $apiURL;
    }

    /**
     * Проверяем загрузку файлов
     *
     * @global type $imagesDir
     */
    public function testGetFile() {
        global $imagesDir;
        $id = 1;
        $filename = $imagesDir . "/" . $id . ".png";
        $testFilename = "data/test.png";
        // Проверяем скачивание файла
        copy($testFilename, $filename);
        $this->assertTrue(file_exists($filename));
        $f = new \REST\File();
        $result = $f->download($this->apiURL . "/api/images/" . $id);
        // Проверим только по размеру, по содержанию пока проверять не будем
        $this->assertEquals(filesize($result), filesize($filename));
        unlink($imagesDir . "/" . $id . ".png");
        // Проверка загрузки файла
        $data = exec("curl -s -F \"file1=@" . $testFilename . "\" -F \"file2=@" . $testFilename . "\" -X POST " . $this->apiURL . "/api/images");
        $ids = json_decode($data);
        $this->assertTrue(is_array($ids));
        $this->assertEquals(2, count($ids));
        $this->assertGreaterThan(0, $ids[0]);
    }

    /**
     * Проверяем загрузку файлов по JSON
     *
     * @global type $imagesDir
     */
    public function testGetFileJSON() {
        global $imagesDir;
        $id = 1;
        $filename = $imagesDir . "/" . $id . ".png";
        $testFilename = "data/test.png";

        // Проверяем загрузку по JSON
        $content = base64_encode(file_get_contents($testFilename));
        $jsonData = json_encode(array(
            array("name" => "file1", "content" => $content),
            array("name" => "file2", "content" => $content)));
        $tmpFilename = tempnam("/tmp", "test");
        file_put_contents($tmpFilename, $jsonData);
        $data = exec("curl --noproxy localhost -s --header \"Content-Type: application/json\" -d @" . $tmpFilename . " -X POST " . $this->apiURL . "/api/images");
        $ids = json_decode($data);
        $this->assertTrue(is_array($ids));
        $this->assertEquals(2, count($ids));
        $this->assertGreaterThan(0, $ids[0]);
        unlink($tmpFilename);
    }

    /**
     * Проверяем загрузку по URL
     *
     * @global type $imagesDir
     */
    public function testGetUrl() {
        global $imagesDir;
        $id = 1;
        $testURL = $this->apiURL . "/api/images/" . $id;
        $filename = $imagesDir . "/" . $id . ".png";
        $testFilename = "data/test.png";
        // Проверяем скачивание файла
        copy($testFilename, $filename);

        // Проверим только по размеру, по содержанию пока проверять не будем
        $data = exec("curl -s -F \"url=" . $testURL . "\" -X POST " . $this->apiURL . "/api/images");
        $ids = json_decode($data);
        $this->assertTrue(is_array($ids));
        $this->assertEquals(1, count($ids));
        $this->assertGreaterThan(0, $ids[0]);
    }

    /**
     * Проверяем загрузку по URL в JSON
     *
     * @global type $imagesDir
     */
    public function testGetUrlJSON() {
        global $imagesDir;
        $id = 1;
        $testURL = $this->apiURL . "/api/images/" . $id;
        $testFilename = "data/test.png";
        // Проверяем загрузку по JSON
        $content = base64_encode(file_get_contents($testFilename));
        $jsonData = json_encode(array(
            array("name" => "file1", "url" => $testURL),
            array("name" => "file2", "url" => $testURL)));
        $tmpFilename = tempnam("/tmp", "test");
        file_put_contents($tmpFilename, $jsonData);
        $data = exec("curl --noproxy localhost -s --header \"Content-Type: application/json\" -d @" . $tmpFilename . " -X POST " . $this->apiURL . "/api/images");
        $ids = json_decode($data);
        $this->assertTrue(is_array($ids));
        $this->assertEquals(2, count($ids));
        $this->assertGreaterThan(0, $ids[0]);
        unlink($tmpFilename);
    }

}
