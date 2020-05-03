
const {app, BrowserWindow} = require('electron')
const glob = require('glob')
const path = require('path')
const marked = require('marked')

//定义debug标志
const debug = /--debug/.test(process.argv[2])

function init(){

    loadJS()

    function createWindow(){
        const windowOptions = {
            width: 1080,
            minWidth: 680,
            height: 840,
            title: app.getName()
        }

        mainWindow = new BrowserWindow(windowOptions)
        mainWindow.loadURL(path.join('file://', __dirname, '/index.html'))

        // Launch fullscreen with DevTools open, usage: npm run debug
        if (true) {
            mainWindow.webContents.openDevTools()
        }
    
        mainWindow.on('closed', () => {
            mainWindow = null
        })
    }

    app.on('ready', () => {
        createWindow()
    })
    
    app.on('window-all-closed', () => {
        if (process.platform !== 'darwin') {
            app.quit()
        }
    })
    
    app.on('activate', () => {
        if (mainWindow === null) {
            createWindow()
        }
    })
}

function loadJS () {
    const files = glob.sync(path.join(__dirname, 'main-process/**/*.js'))
    files.forEach((file) => { require(file) })
  }

init()