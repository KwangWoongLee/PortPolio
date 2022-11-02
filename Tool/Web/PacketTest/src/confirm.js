const { ipcMain, BrowserWindow } = require('electron');
const ejse = require('ejs-electron');

function electronConfirm(title, msg) {
  let ret = false;
  return new Promise((resolve, reject) => {
    let win = new BrowserWindow({
      parent: BrowserWindow.getFocusedWindow(),
      show: false,
      modal: true,
      width: 300,
      height: 200,
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

    ejse.data('msg', `<h4>${msg}</h4>`);

    win.once('ready-to-show', function () {
      win.show();
      win.webContents.send('init-message', {});
      //win.webContents.openDevTools();
    });

    win.loadURL(`file://${_root_name}/public/confirm.ejs`);

    win.on('closed', () => {
      ipcMain.removeAllListeners('confirm-message');
      win = null;
      if (ret) resolve();
      else reject('cancel');
    });

    ipcMain.on('confirm-message', (event, arg) => {
      ret = arg == 'ok';
      win.close();
    });
  });
}

module.exports = electronConfirm;
