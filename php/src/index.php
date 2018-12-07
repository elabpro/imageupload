<?php
require_once '../include/config.php';
require_once 'myapi.php';
require_once 'db.php';

try {
    $api = new REST\MyApi();
    $db = new REST\DB();
    $api->setDB($db);
    echo $api->run();
} catch (Exception $e) {
    echo json_encode(Array('error' => $e->getMessage()));
}
