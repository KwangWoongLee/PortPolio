/*
 * api : https://www.electronjs.org/docs/api
 */

const { app, Menu, Tray, BrowserWindow, ipcMain, globalShortcut } = require('electron');
const ejse = require('ejs-electron');
let main_win;
const isMac = process.platform === 'darwin';
const com = require('./src/com');
const fs = require('fs');
const sender = require('./sender');
const updater = require('./updater');
const ginfo = require('./src/ginfo');
global.conf = {};
global._root_name = __dirname;

const connect_selector = require('./src/connect_selector');

const single_instance = app.requestSingleInstanceLock();
if (!single_instance) {
  app.quit();
  app.exit();
}

function create_window() {
  main_win = new BrowserWindow({
    width: 700,
    height: 740,
    resizable: false,
    show: false,
    icon: __dirname + '/public/res/favicon.ico',
    webPreferences: {
      defaultFontSize: 7,
      nodeIntegration: true,
      enableRemoteModule: true,
      webviewTag: true,
    },
  });

  main_win.once('ready-to-show', function () {
    main_win.show();
    init_renderer();
  });

  main_win.on('closed', (event) => {
    main_win = null;
  });

  init_dialog();
}

app.on('ready', create_window);

app.on('window-all-closed', () => {
  if (!isMac) {
    app.quit();
  }
});

app.on('activate', () => {
  if (main_win === null) {
    create_window();
  }
});

let tray = null;
app.whenReady().then(() => {
  tray = new Tray(__dirname + '/public/res/favicon.ico');
  tray.on('click', () => {
    main_win.show();
  });
  main_win.on('close', (event) => {
    // if (tray) {
    //   event.preventDefault();
    //   main_win.hide();
    // }
  });

  const contextMenu = Menu.buildFromTemplate([
    {
      label: '종료',
      type: 'normal',
      checked: true,
      click: () => {
        tray.destroy();
        tray = null;
        app.quit();
      },
    },
  ]);
  tray.setToolTip('This is my application.');
  tray.setContextMenu(contextMenu);
});

function set_menu() {
  const template = [
    {
      label: '메뉴',
      submenu: [
        {
          label: '설정창',
          accelerator: 'CmdOrCtrl+O',
          click: () => {
            connect_dlg_open();
          },
        },
        { type: 'separator' },
        {
          label: '종료',
          accelerator: 'CmdOrCtrl+Q',
          click: () => {
            com
              .open_confirm('종료', '프로그램을 종료합니다.')
              .then(() => {
                app.quit();
              })
              .catch((e) => {
                console.log('종료 취소함');
              });
          },
        },
        {
          label: '개발툴',
          accelerator: 'F12',
          click: () => {
            //BrowserWindow.getFocusedWindow().toggleDevTools();
            const w = BrowserWindow.getFocusedWindow();
            if (w.isDevToolsOpened()) w.closeDevTools();
            else w.openDevTools({ mode: 'undocked' });
          },
        },
      ],
    },
    {
      role: 'help',
      submenu: [
        {
          label: '패킷문서',
          accelerator: 'F1',
          click: async () => {
            com.create_child_brower('http://49.50.172.145:8880/', main_win);
          },
        },
        {
          label: 'ElectronAPI',
          accelerator: 'F2',
          click: async () => {
            com.create_child_brower('https://electronjs.org', main_win);
          },
        },
      ],
    },
  ];
  const menu = Menu.buildFromTemplate(template);
  //Menu.setApplicationMenu(menu);
  com.ApplicationMenu();
  main_win.setMenu(menu);
}

function init_dialog() {
  // config.json load
  const file = fs.readFileSync(process.cwd() + '/config.json');
  conf = JSON.parse(file);

  ejse.data('title', '패킷 테스트 APP , Ver ' + conf.app_version);
  main_win.loadURL(`file://${__dirname}/public/index.ejs`);

  set_menu();

  globalShortcut.register('F11', () => {
    main_win.maximize();
  });
}

function init_renderer() {
  const ret = com.load_protocol();
  main_win.webContents.send('init-message', { protocols: ret.data });

  ginfo.init(main_win);
  sender.init(main_win, com.get_protocols());
  ginfo.console_message('packet message init success!!');

  connect_dlg_open();

  //com.open_alert('패킷테스트', '패킷 테스트 툴입니다.');
}

// --------------- message --------------

ipcMain.on('connect-open-message', (event, arg) => {
  connect_dlg_open();
});

ipcMain.on('json-edit-file-message', (event, arg) => {
  const proto_name = com.write_protocol_file(arg.filename, arg.value);
  main_win.webContents.send('json-edit-file-message', {
    title: proto_name,
    str: arg.value,
  });

  event.returnValue = 0;
});

ipcMain.on('packet-init-message', (event, arg) => {
  send_init_packet_message();
});

ipcMain.on('send-message', async function (event, arg) {
  for (let i = 0; i < arg.length; i++) {
    const packet_name = arg[i];
    await sender.send_packet(packet_name);
  }

  main_win.webContents.send('send_complate', {});
});

ipcMain.on('send-infinity-message', async function (event, arg) {
  let promise = com.open_alert(
    '패킷 전송중',
    '창을 닫을때까지 보냅니다.<br>멈추는데 시간이 <br>조금 걸릴수도 있습니다.'
  );
  promise.then(() => {
    promise.done = true;
  });

  while (!promise.done) {
    for (let i = 0; i < arg.length; i++) {
      const packet_name = arg[i];
      await sender.send_packet(packet_name);
    }
    com.sleep(1);
  }

  main_win.webContents.send('send_complate', {});
});
// ----------------- function ----------------------

function send_init_packet_message() {
  main_win.webContents.send('init-packet-message', conf.server_list[conf.select_idx].init);
}

function connect_dlg_open() {
  connect_selector().then((reg_id) => {
    send_init_packet_message();
    if (reg_id) {
      sender.send_packet(conf.register_packet);
    }
  });
}
