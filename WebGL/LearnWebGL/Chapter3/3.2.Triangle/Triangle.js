


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

    /**
     * gl.POINTS
     * gl.LINES
     * gl.LINE_STRIP
     * gl.LINE_LOOP
     * gl.TRIANGLES
     * gl.TRIANGLE_STRIP
     * gl.TRIANGLE_FAN
     */
    gl.drawArrays(gl.TRIANGLES, 0, n);

}

function initVertexBuffers(gl){

    var vertices = new Float32Array([
        0.0, 0.5, -0.5, -0.5, 0.5, -0.5
    ])
    var n = 3;

    var vertexBuffer = gl.createBuffer();
    if(!vertexBuffer){
        console.log('Failed to create the buffer object ');
        return -1;
    }

    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);

    gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

    var a_Position = gl.getAttribLocation(gl.program, 'a_Position');

    gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, 0, 0);

    gl.enableVertexAttribArray(a_Position);

    return n;
}


