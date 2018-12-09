<?php

namespace REST;

require_once 'api.php';
require_once 'file.php';

class MyApi extends \REST\Api {

    public $apiName = 'images';
    private $myF;

    public function init() {
        $this->myF = new \REST\File();
    }

    /**
     * Метод GET
     * Вывод списка всех записей
     *
     * @usage http://localhost/api/images
     *
     * @return string
     */
    public function indexAction() {
        global $imagesDir;
        $images = array();
        $d = opendir($imagesDir);
        while ($file = readdir($d)) {
            if (is_file($imagesDir . "/" . $file)) {
                $images[] = $file;
            }
        }
        if (count($images) > 0) {
            return $this->response($images, 200);
        }
        return $this->response('Data not found', 404);
    }

    /**
     * Метод GET
     * Просмотр отдельной записи (по id)
     *
     * @usage http://localhost/api/images/id
     *
     * @return string
     */
    public function viewAction() {
        global $imagesDir;
        //id должен быть первым параметром после /images/x
        $id = array_shift($this->requestUri);

        if ($id > 0) {
            $filename = $this->myF->getFilename($id);
            if (file_exists($filename)) {
                return $this->responseBinary(file_get_contents($filename), 200);
            }
        }
        return $this->response('Data not found', 404);
    }

    /**
     * Метод POST
     * Загрузка фотографий в формате PNG. Форматы тела POST:
     * - multipart/encoded files
     * - JSON {{content=#base64_image_content#|url=#url#}}
     * - url=#url#
     *
     * @usage http://localhost/api/images
     *
     * @return string
     */
    public function createAction() {
        $fileIds = array();
        // Обработка файлов
        global $_FILES, $_SERVER;
        if ((isset($_SERVER["HTTP_CONTENT_TYPE"]) && $_SERVER["HTTP_CONTENT_TYPE"] == "application/json") ||
                (isset($_SERVER["CONTENT_TYPE"]) && $_SERVER["CONTENT_TYPE"] == "application/json")) {
            $jsonData = file_get_contents("php://input");
            $arr = json_decode($jsonData, TRUE);
            if (is_array($arr)) {
                foreach ($arr as $f) {
                    if (isset($f["content"]) && $f["content"] != "") {
                        $fileIds[] = $this->myF->saveImageContent($f["content"]);
                    }
                    if (isset($f["url"]) && $f["url"] != "") {
                        $tmpFilename = $this->myF->download($f["url"]);
                        if (file_exists($tmpFilename)) {
                            $fileIds[] = $this->myF->saveImageFile($tmpFilename);
                        }
                    }
                }
            }
        } else {
            foreach ($_FILES as $f) {
                if ($f["error"] === 0 && $f["size"] > 0) {
                    $fileIds[] = $this->myF->saveImageFile($f["tmp_name"]);
                }
            }
            if (isset($_POST["url"]) && $_POST["url"] != "") {
                $tmpFilename = $this->myF->download($_POST["url"]);
                if (file_exists($tmpFilename)) {
                    $fileIds[] = $this->myF->saveImageFile($tmpFilename);
                }
            }
        }
        return $this->response($fileIds, 200);
    }

    /**
     * Метод PUT
     * Обновление отдельной записи (по ее id)
     * *Не реализовано*
     * http://localhost/api/images/id
     * @return string
     */
    public function updateAction() {

    }

    /**
     * Метод DELETE
     * Удаление отдельной записи (по ее id)
     * *Не реализовано*
     * http://localhost/api/images/id
     * @return string
     */
    public function deleteAction() {

    }

}
