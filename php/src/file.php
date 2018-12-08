<?php

/*
 * Обертка для работы с файлами
 */

namespace REST;

class File {

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
        return $imagesDir . "/" . $id . ".png";
    }

    public function getThumbFilename($id) {
        global $imagesThumbDir;
        return $imagesThumbDir . "/" . $id . ".png";
    }

    /**
     * Заглушка для проверки валидности допустимых URL
     */
    private function valid($url) {
        return true;
    }

}
