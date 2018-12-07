<?php

namespace REST;

require_once 'api.php';
require_once 'file.php';

class MyApi extends \REST\Api {

    public $apiName = 'images';
    private $db;

    public function setDB($db) {
        $this->db = $db;
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

        if ($images) {
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
            $filename = $imagesDir . "/" . $id . ".png";
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
        $myF = new \REST\File();
        $fileIds = array();
        // Обработка файлов
        global $_FILES, $_SERVER;
        if ($_SERVER["HTTP_CONTENT_TYPE"] == "application/json") {
            $jsonData = file_get_contents("php://input");
            $arr = json_decode($jsonData, TRUE);
            if (is_array($arr)) {
                foreach ($arr as $f) {
                    if ($f["content"] != "") {
                        $fileId = $this->db->getCounter();
                        file_put_contents($myF->getFilename($fileId), base64_decode($f["content"]));
                        $fileIds[] = $fileId;
                        $this->createThumb($fileId);
                    }
                    if ($f["url"] != "") {
                        $tmpFilename = $myF->download($f["url"]);
                        if (file_exists($tmpFilename)) {
                            $fileId = $this->db->getCounter();
                            move_uploaded_file($tmpFilename, $myF->getFilename($fileId));
                            $fileIds[] = $fileId;
                            $this->createThumb($fileId);
                        }
                    }
                }
            }
        } else {
            foreach ($_FILES as $f) {
                if ($f["error"] === 0 && $f["size"] > 0) {
                    $fileId = $this->db->getCounter();
                    move_uploaded_file($f["tmp_name"], $myF->getFilename($fileId));
                    $fileIds[] = $fileId;
                    $this->createThumb($fileId);
                }
            }
            if (isset($_POST["url"]) && $_POST["url"] != "") {
                $tmpFilename = $myF->download($_POST["url"]);
                if (file_exists($tmpFilename)) {
                    $fileId = $this->db->getCounter();
                    move_uploaded_file($tmpFilename, $myF->getFilename($fileId));
                    $fileIds[] = $fileId;
                    $this->createThumb($fileId);
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

    /**
     * Создание превью 100x100
     * 
     * @param int $id
     * @return int 1 - успешно
     */
    function createThumb($id) {
        $myF = new \REST\File();
        exec("convert " . $myF->getFilename($id) . " -resize 100x100\! " . $myF->getThumbFilename($id));
        return 1;
    }

}
