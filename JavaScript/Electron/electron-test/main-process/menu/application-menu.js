const {BrowserWindow, Menu, app, shell, dialog} = require('electron')

let template = [{
    label: 'File',
    submenu:[{
        label: 'Open',
        click:()=>{
            console.log('hello open')
        }
    }]
}]

app.on('ready', () => {
    const menu = Menu.buildFromTemplate(template)
    Menu.setApplicationMenu(menu)
})
