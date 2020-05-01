const path = require('path')
const glob = require('glob')


const {app, BrowserWindow} = require('electron')

let win = null

function main(){

    loadDemos()

    function createWindow(){
        win = new BrowserWindow({
            width: 800,
            height: 600,
            webPreferences:{
                nodeIntegration:true
            }
        })
    
    
        win.loadURL(path.join('file://', __dirname, '/index.html'))
    
        win.webContents.openDevTools()

        win.webContents.send('show-chart')

        win.on('closed', () => {
            mainWindow = null
        })
    }
    
    app.on('ready', () => {
        createWindow()
    })
    
    // Quit when all windows are closed.
    app.on('window-all-closed', () => {
        // 在 macOS 上，除非用户用 Cmd + Q 确定地退出，
        // 否则绝大部分应用及其菜单栏会保持激活。
        if (process.platform !== 'darwin') {
            app.quit()
        }
    })
    
    app.on('activate', () => {
        // 在macOS上，当单击dock图标并且没有其他窗口打开时，
        // 通常在应用程序中重新创建一个窗口。
        if (BrowserWindow.getAllWindows().length === 0) {
            createWindow()
        }
    })
    
}

function loadDemos() {
    const files = glob.sync(path.join(__dirname, 'main-process/**/*.js'))
    files.forEach((file) => { require(file) })
}

main()