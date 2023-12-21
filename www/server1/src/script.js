console.log("barev");

function submitForm() {
    const formData = new FormData(document.getElementById('myForm'));
    fetch('/submit', {
        method: 'POST',
        body: formData
    })
    .then(response => response.text())
    .then(data => {
        document.getElementById('response').innerText = data;
    })
    .catch(error => {
        console.error('Error:', error);
    });
}

function fileUpload() {
    const formData = new FormData(document.getElementById('fileInput'));
    fetch('/uploader.py', {
        method: 'POST',
        body: formData
    })
    .then(response => response.text())
    .then(data => {
        console.log('success:');
    })
    .catch(error => {
        console.error('Error:', error);
    });
}