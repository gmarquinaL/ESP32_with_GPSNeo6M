<?php
    $inp = file_get_contents("php://input");
    if(!empty($inp)){
        $s = explode("&",$inp);
        $a = explode("=",$s[0]);
        $b = explode("=",$s[1]);
        $file = fopen("gps_data.txt", "w") or die("Unable to open file!");
        $txt = $a[1] . "," . $b[1];
        fwrite($file, $txt);
        fclose($file);
    }else{
        $file = fopen("gps_data.txt", "r") or die("Unable to open file!");
        echo fread($file,filesize("gps_data.txt"));
        fclose($file); 
    }
?>
