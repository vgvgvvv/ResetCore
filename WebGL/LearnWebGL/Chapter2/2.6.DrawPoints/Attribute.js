


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

    var a_Position = gl.getAttribLocation(gl.program, 'a_Position');

    if(a_Position < 0){
        console.log('Fail to get a_Position');
        return;
    }

    canvas.onmousedown = function (ev) {
        onClick(ev, gl, canvas, a_Position);
    }

    // 设置清理色
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    // 进行clear
    gl.clear(gl.COLOR_BUFFER_BIT);

}

var g_Points = [];//click positions

function onClick(ev, gl, canvas, a_Position) {
    var x = ev.clientX;
    var y = ev.clientY;
    var rect = ev.target.getBoundingClientRect();

    x = ((x - rect.left) - canvas.width/2)/(canvas.width/2);
    y = (canvas.height/2 - (y - rect.top))/(canvas.height/2);

    g_Points.push(x);
    g_Points.push(y);

    gl.clear(gl.COLOR_BUFFER_BIT);

    var len = g_Points.length;

    for(var i = 0; i < len; i += 2){
        gl.vertexAttrib3f(a_Position, g_Points[i], g_Points[i+1], 0.0);

        gl.drawArrays(gl.POINTS, 0, 1);
    }
}

