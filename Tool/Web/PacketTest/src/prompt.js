const { ipcMain, BrowserWindow } = require('electron');
const ejse = require('ejs-electron');

function electronPrompt(title, type, value, msg) {
  let ret = { ok: false, data: '' };
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

    ejse.data('type', type);
    ejse.data('value', value);
    ejse.data('msg', `<h4>${msg}</h4>`);

    win.once('ready-to-show', function () {
      win.show();
      win.webContents.send('init-message', {});
      //win.webContents.openDevTools();
    });

    win.loadURL(`file://${_root_name}/public/prompt.ejs`);

    win.on('closed', () => {
      ipcMain.removeAllListeners('prompt-message');
      win = null;
      if (ret.ok) resolve(ret.data);
      else reject('cancel');
    });

    ipcMain.on('prompt-message', (event, arg) => {
      ret.ok = arg.ok;
      ret.data = arg.data;
      win.close();
    });
  });
}

module.exports = electronPrompt;
