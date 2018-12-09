<?php

/*
 * Обертка для работы с файлами
 */

namespace REST;

require_once 'db.php';

class File {

    var $db;

    public function __construct() {
        $this->db = new \REST\DB();
    }

    /**
     * Скачать файл по ссылке
     * @param type $url
     * @return type
     */
    public function download($url) {
        if ($url != "" && $this->valid($url)) {
            $ch = \curl_init();
            /**
             * Set the URL of the page or file to download.
             */
            \curl_setopt($ch, CURLOPT_URL, $url);
            $tmpname = tempnam("/tmp", "download");
            $fp = fopen($tmpname, 'w+');
            \curl_setopt($ch, CURLOPT_FILE, $fp);
            \curl_exec($ch);
            \curl_close($ch);
            fclose($fp);
            return $tmpname;
        } else {
            return false;
        }
    }

    public function getFilename($id) {
        global $imagesDir;
        $id += 0;
        return $imagesDir . "/" . $id;
    }

    public function getThumbFilename($id) {
        global $imagesThumbDir;
        return $imagesThumbDir . "/" . $id;
    }

    /**
     * Запись content в файл
     *
     * @param string $content
     * @return int
     */
    public function saveImageContent($content) {
        $fileId = $this->db->getCounter();
        $filename = $this->getFilename($fileId);
        $filenameLock = $filename . ".lock";
        file_put_contents($filenameLock, "0");
        file_put_contents($filename, base64_decode($content));
        unlink($filenameLock);
        $this->createThumb($fileId);
        return $fileId;
    }

    /**
     * Перенос файла
     *
     * @param string $tmpFilename
     * @return int
     */
    public function saveImageFile($tmpFilename) {
        $fileId = $this->db->getCounter();
        $filename = $this->getFilename($fileId);
        $filenameLock = $filename . ".lock";
        file_put_contents($filenameLock, "0");
        move_uploaded_file($tmpFilename, $filename);
        unlink($filenameLock);
        $this->createThumb($fileId);
        return $fileId;
    }

    /**
     * Заглушка для проверки валидности допустимых URL
     */
    private function valid($url) {
        return true;
    }

    /**
     * Создание превью 100x100
     *
     * @param int $id
     * @return int 1 - успешно
     */
    function createThumb($id) {
        $filenameLock = $this->getThumbFilename($id) . ".lock";
        file_put_contents($filenameLock, "0");
        exec("convert " . $this->getFilename($id) . " -resize 100x100\! " . $this->getThumbFilename($id));
        unlink($filenameLock);
        return 1;
    }

}
