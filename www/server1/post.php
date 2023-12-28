<?php
if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_FILES["filef"])) {
    $uploadDir = 'data_base/'; // Directory where the file will be uploaded (should be created beforehand)
    $uploadFile = $uploadDir . basename($_FILES['filef']['name']);

        echo $uploadFile;
        echo $_FILES["filef"]["name"] . "    ";
        echo $_FILES["filef"]["tmp_name"] . "    ";

    if (move_uploaded_file($_FILES['filef']['tmp_name'], $uploadFile)) {
        echo "File successfully uploaded: " . htmlspecialchars(basename($_FILES['filef']['name']));
    } else {
        echo "Error uploading file.";
    }
} else {
    echo "Invalid request or no file was selected for upload.";
}

// echo $_SERVER["CONTENT_LENGTH"];
// echo $_FILES["filef"]["size"];
?>