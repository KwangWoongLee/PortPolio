const { ipcMain, BrowserWindow } = require('electron');
const ejse = require('ejs-electron');

function json_edit(title, value) {
  return new Promise((resolve) => {
    let win = new BrowserWindow({
      parent: BrowserWindow.getFocusedWindow(),
      show: false,
      modal: true,
      width: 300,
      height: 300,
      resizable: false,
      minimizable: false,
      fullscreenable: false,
      maximizable: false,
      icon: _root_name + '/public/res/favicon.ico',
      webPreferences: {
        nodeIntegration: true,
        enableRemoteModule: true,
      },
    });

    win.setMenu(null);
    win.setMenuBarVisibility(false);
    win.setTitle(title);

    ejse.data('title', title);
    ejse.data('value', value);

    win.once('ready-to-show', function () {
      win.show();
      win.webContents.send('init-message', title);
      //win.webContents.openDevTools();
    });

    win.loadURL(`file://${_root_name}/public/json_edit.ejs`);

    win.on('closed', () => {
      resolve();
    });
  });
}

module.exports = json_edit;
