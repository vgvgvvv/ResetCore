


var VSHADER_SOURCE = 'void main(){\n' +
    '    gl_Position = vec4(0.0, 0.0, 0.0, 1.0);\n' +
    '    gl_PointSize = 10.0;\n' +
    '}';
var FSHADER_SOURCE = 'void main(){\n' +
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

    // 设置清理色
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

     // 进行clear
    gl.clear(gl.COLOR_BUFFER_BIT);

    gl.drawArrays(gl.POINTS, 0, 1);

}
