<?php

/*
 * Тестирование API
 */
require_once '../include/config.php';
require_once '../src/file.php';
require_once 'HttpRequest.php';

use PHPUnit\Framework\TestCase;

class StackTest extends TestCase {

    /**
     * Проверяем загрузку файлов
     * 
     * @global type $imagesDir
     * @global type $apiURL
     */
    public function testGetFile() {
        return;
        global $imagesDir, $apiURL;
        $id = 1;
        $filename = $imagesDir . "/" . $id . ".png";
        $testFilename = "data/test.png";
        // Проверяем скачивание файла
        copy($testFilename, $filename);
        $this->assertTrue(file_exists($filename));
        $f = new \REST\File();
        $result = $f->download($apiURL . "/api/images/" . $id);
        // Проверим только по размеру, по содержанию пока проверять не будем
        $this->assertEquals(filesize($result), filesize($filename));
        unlink($imagesDir . "/" . $id . ".png");
        // Проверка загрузки файла
        $data = exec("curl -s -F \"file1=@" . $testFilename . "\" -F \"file2=@" . $testFilename . "\" -X POST " . $apiURL . "/api/images");
        $ids = json_decode($data);
        $this->assertTrue(is_array($ids));
        $this->assertEquals(2, count($ids));
        $this->assertGreaterThan(0, $ids[0]);
        // Проверяем загрузку по JSON
        $content = base64_encode(file_get_contents($testFilename));
        $jsonData = json_encode(array(
            array("name" => "file1", "content" => $content),
            array("name" => "file2", "content" => $content)));
        $tmpFilename = tempnam("/tmp", "test");
        file_put_contents($tmpFilename, $jsonData);
        $data = exec("curl -s --header \"Content-Type: application/json\" -d @" . $tmpFilename . " -X POST " . $apiURL . "/api/images");
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
     * @global type $apiURL
     */
    public function testGetUrl() {
        global $imagesDir, $apiURL;
        $id = 1;
        $testURL = $apiURL . "/api/images/" . $id;
        $filename = $imagesDir . "/" . $id . ".png";
        $testFilename = "data/test.png";
        // Проверяем скачивание файла
        copy($testFilename, $filename);

        // Проверим только по размеру, по содержанию пока проверять не будем
        $data = exec("curl -s -F \"url=" . $testURL . "\" -X POST " . $apiURL . "/api/images");
        $ids = json_decode($data);
        $this->assertTrue(is_array($ids));
        $this->assertEquals(1, count($ids));
        $this->assertGreaterThan(0, $ids[0]);
        // Проверяем загрузку по JSON
        $content = base64_encode(file_get_contents($testFilename));
        $jsonData = json_encode(array(
            array("name" => "file1", "url" => $testURL),
            array("name" => "file2", "url" => $testURL)));
        $tmpFilename = tempnam("/tmp", "test");
        file_put_contents($tmpFilename, $jsonData);
        $data = exec("curl -s --header \"Content-Type: application/json\" -d @" . $tmpFilename . " -X POST " . $apiURL . "/api/images");
        $ids = json_decode($data);
        $this->assertTrue(is_array($ids));
        $this->assertEquals(2, count($ids));
        $this->assertGreaterThan(0, $ids[0]);
        unlink($tmpFilename);
    }

}
