const {BrowserWindow, Menu, app, shell, dialog, ipcMain } = require('electron')

let template = [{
    label: 'File',
    submenu:[{
        label: 'Open',
        click:(item, focusedWindow)=>{
            console.log('hello open')
            focusedWindow.webContents.send('show-chart')
        }
    }]
}]

app.on('ready', () => {
    const menu = Menu.buildFromTemplate(template)
    Menu.setApplicationMenu(menu)
})
