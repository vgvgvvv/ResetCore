


function main() {
    var canvas = document.getElementById('webgl');

    var gl = getWebGLContext(canvas);

    if(!gl){
        console.log('Fail to get WebGL');
        return;
    }

    // 设置清理色
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

     // 进行clear
    gl.clear(gl.COLOR_BUFFER_BIT);

}