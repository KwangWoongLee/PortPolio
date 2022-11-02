const { ipcMain, BrowserWindow } = require('electron');
const ejse = require('ejs-electron');
const com = require('./com');

function connect_selector() {
  let reg_id = false;
  return new Promise((resolve) => {
    let win = new BrowserWindow({
      parent: BrowserWindow.getFocusedWindow(),
      show: false,
      modal: true,
      width: 500,
      height: 330,
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
    win.setTitle('접속 정보 선택');

    ejse.data('conf', conf);

    win.once('ready-to-show', function () {
      win.show();
      win.webContents.send('init-message', {});
      //win.webContents.openDevTools();
    });

    win.loadURL(`file://${_root_name}/public/connect_selector.ejs`);

    win.on('closed', () => {
      ipcMain.removeAllListeners('change-message');
      ipcMain.removeAllListeners('close-message');
      resolve(reg_id);
    });

    ipcMain.on('change-message', (event, arg) => {
      conf.select_idx = arg.select_idx;
      conf.login_id = arg.login_id;
      conf.version = arg.version;
      win.reload();
      event.returnValue = 0;
    });

    ipcMain.on('close-message', (event, arg) => {
      conf.select_idx = arg.select_idx;
      conf.login_id = arg.login_id;
      conf.version = arg.version;
      reg_id = arg.reg_id;
      com.write_config_file();
      win.close();
    });
  });
}

module.exports = connect_selector;
