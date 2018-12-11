<?php

/*
 * Требуется запускать перед включение API
 */

require_once __DIR__ . "/../include/config.php";
require_once __DIR__ . "/file.php";
require_once __DIR__ . "/db.php";

$maxId = 0;
// Check images
$imagesDir = __DIR__ . "/".$imagesDir;
$d = opendir($imagesDir);
while($file = readdir($d)){
    $filename = $imagesDir . "/" . $file;
    if(is_file($filename)){
        if(strpos($file,".lock") > 0){
            $fileId = $file+ 0;
            print "Deleting image " . $fileId . "\n";
            unlink($imagesDir . "/" . $fileId);
            unlink($filename);
        }else{
            if($maxId < $file){
                $maxId = $file + 0;
            }
        }
    }
}
// Check thumbs
$myF = new \REST\File();
$imagesThumbDir = __DIR__ . "/".$imagesThumbDir;
$d = opendir($imagesThumbDir);
while($file = readdir($d)){
    $filename = $imagesThumbDir . "/" . $file;
    if(is_file($filename)){
        if(strpos($file,".lock") > 0){
            $fileId = $file+0;
            unlink($imagesThumbDir . "/" . $fileId);
            print "Restore thumb for " . $fileId . "\n";
	    $myF->createThumb($fileId);
        }else{
            if($maxId < $file){
                $maxId = $file + 0;
            }
        }
    }
}

print "Setting counter to " . $maxId . "\n";
$db = new \REST\DB();
$db->setCounter($maxId);

