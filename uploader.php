<?php
$target_path = "uploads/";

$target_path = $target_path . basename( $_FILES['uploadedfile']['name']);
#echo $target_path;
if(move_uploaded_file($_FILES['uploadedfile']['tmp_name'], $target_path)) {
    #echo "apple";

    //echo $target_path;
    $res=shell_exec("python /Users/nus/dataref/datacode.py '".$target_path."' ");
    //$output = shell_exec('php download.php');
    #echo $res;
    echo "<a href='" .$res. "'> Click to download </a>";

} else{
    echo "kms";
}
?>
