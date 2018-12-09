<?php
require_once '../include/config.php';
require_once 'myapi.php';

try {
    $api = new \REST\MyApi();
    $api->init();
    echo $api->run();
} catch (Exception $e) {
    echo json_encode(Array('error' => $e->getMessage()));
}
