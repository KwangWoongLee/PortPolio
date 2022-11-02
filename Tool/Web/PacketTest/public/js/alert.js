const { ipcRenderer, remote } = require('electron');
var ginfo = remote.require('./src/ginfo');

$(function () {
  $('.btn').click(() => {
    ipcRenderer.sendSync('alert-message', 'ok');
  });

  $(document).on('keyup', (event) => {
    if (event.key === 'Escape') {
      remote.getCurrentWindow().close();
    } else if (event.key === 'Enter') {
      ipcRenderer.sendSync('alert-message', 'ok');
    } else if (event.key === 'F12') {
      remote.getCurrentWindow().openDevTools();
    }
  });
});

ipcRenderer.on('init-message', (event, arg) => {
  ginfo.debug_message('alert init renderer');
});
