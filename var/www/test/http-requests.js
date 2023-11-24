function sendRequest(method) {
    var url = document.getElementById('url').value;
    var fileInput = document.getElementById('fileUpload');
    var headers = {
        'Accept': '*/*'
    };

    var body = null;
    var requestMessage = `${method} ${url} HTTP/1.1\n`;

    if ((method === 'POST' || method === 'PUT') && fileInput.files.length > 0) {
        var file = fileInput.files[0];
        var reader = new FileReader();

        reader.onloadend = function () {
            var base64data = reader.result.split(',')[1];
            body = 'fileContent=' + encodeURIComponent(base64data);
            headers['Content-Type'] = 'application/x-www-form-urlencoded';
            makeRequest(method, url, headers, body, requestMessage);
        };

        reader.readAsDataURL(file);
    } else {
        // DELETE와 PUT 요청에서는 Content-Type 헤더를 설정하지 않습니다.
        if (method === 'DELETE' || method === 'PUT') {
            delete headers['Content-Type'];
        } else {
            headers['Content-Type'] = 'text/plain';
        }

        makeRequest(method, url, headers, body, requestMessage);
    }
}

function makeRequest(method, url, headers, body, requestMessage) {
    // 헤더를 요청 메시지에 추가합니다.
    for (var key in headers) {
        requestMessage += `${key}: ${headers[key]}\n`;
    }
    requestMessage += '\n' + (body || '');

    console.log('Request Message:\n', requestMessage);

    var xhr = new XMLHttpRequest();
    xhr.open(method, url, true);

    for (var key in headers) {
        xhr.setRequestHeader(key, headers[key]);
    }

    xhr.onload = function () {
        var responseMessage = 'HTTP/' + this.status + ' ' + xhr.statusText + '\n';
        responseMessage += xhr.getAllResponseHeaders() + '\n';
        responseMessage += this.responseText;

        console.log('Response Message:\n', responseMessage);
        document.getElementById('response').textContent = responseMessage;
    };

    xhr.onerror = function () {
        console.error('Request failed');
    };

    xhr.send(body);
}

// 페이지가 로드되면 이벤트 리스너를 설정합니다.
document.addEventListener('DOMContentLoaded', function () {
    var buttons = document.querySelectorAll('button');
    buttons.forEach(function (button) {
        button.addEventListener('click', function () {
            sendRequest(button.textContent.trim());
        });
    });
});
