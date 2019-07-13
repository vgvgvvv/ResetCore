


var VSHADER_SOURCE = null;
var FSHADER_SOURCE = null;

function main() {
    var canvas = document.getElementById('webgl');

    var gl = getWebGLContext(canvas);

    if(!gl){
        console.log('Fail to get WebGL');
        return;
    }

    // Read shader from file
    readShaderFile(gl, 'VertexShader.glsl', 'v');
    readShaderFile(gl, 'FragmentShader.glsl', 's');

}


function onRender(gl){
    if(!initShaders(gl, VSHADER_SOURCE, FSHADER_SOURCE)){
        console.log('Fail to init shaders');
        return;
    }

    // 设置清理色
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    // 进行clear
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.drawArrays(gl.POINTS, 0, 1);
}


function readShaderFile(gl, fileName, shader) {
    var request = new XMLHttpRequest();

    request.onreadystatechange = function() {
        if (request.readyState === 4 && request.status !== 404) {
            onReadShader(gl, request.responseText, shader);
        }
    }
    request.open('GET', fileName, true); // Create a request to acquire the file
    request.send();                      // Send the request
}

// The shader is loaded from file
function onReadShader(gl, fileString, shader) {
    if (shader == 'v') { // Vertex shader
        VSHADER_SOURCE = fileString;
    } else
    if (shader == 'f') { // Fragment shader
        FSHADER_SOURCE = fileString;
    }
    // When both are available, call start().
    if (VSHADER_SOURCE && FSHADER_SOURCE) onRender(gl);
}
