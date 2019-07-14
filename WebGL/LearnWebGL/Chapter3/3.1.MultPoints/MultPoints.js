


var VSHADER_SOURCE =
    'attribute vec4 a_Position;\n' +
    'void main() {\n' +
    '    gl_Position = a_Position;\n' +
    '    gl_PointSize = 10.0;\n' +
    '}';
var FSHADER_SOURCE =
    'void main(){\n' +
    '    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n' +
    '}';

function main() {
    var canvas = document.getElementById('webgl');

    var gl = getWebGLContext(canvas);

    if(!gl){
        console.log('Fail to get WebGL');
        return;
    }

    if(!initShaders(gl, VSHADER_SOURCE, FSHADER_SOURCE)){
        console.log('Fail to init shaders');
        return;
    }

    var n = initVertexBuffers(gl);
    if(n < 0){
        console.log('Fail to init vertex buffers');
        return;
    }

    // 设置清理色
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    // 进行clear
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.drawArrays(gl.POINTS, 0, n);

}

/**
 * 1. 创建缓冲区对象
 * 2. 绑定缓冲区对象
 * 3. 将数据写入缓冲区对象
 * 4. 将缓冲区对象分配给一个attribute变量
 * 5. 开启attribute变量
 * @param gl
 * @returns {number}
 */
function initVertexBuffers(gl){

    var vertices = new Float32Array([
        0.0, 0.5, -0.5, -0.5, 0.5, -0.5
    ])
    var n = 3;

    // 1. 创建缓冲区对象
    var vertexBuffer = gl.createBuffer();
    if(!vertexBuffer){
        console.log('Failed to create the buffer object ');
        return -1;
    }

    // 2. 绑定缓冲区对象
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);

    // 3. 将数据写入缓冲区对象
    gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

    var a_Position = gl.getAttribLocation(gl.program, 'a_Position');

    // 4. 将缓冲区对象分配给一个attribute变量
    gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, 0, 0);

    // 5. 开启attribute变量
    gl.enableVertexAttribArray(a_Position);

    return n;
}


