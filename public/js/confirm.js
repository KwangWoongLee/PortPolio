const { ipcRenderer, remote } = require('electron');
var ginfo = remote.require('./src/ginfo');

$(function () {
  $('.btn-success').click(() => {
    ipcRenderer.sendSync('confirm-message', 'ok');
  });

  $('.btn-warning').click(() => {
    ipcRenderer.sendSync('confirm-message', 'cancel');
  });

  $(document).on('keyup', (event) => {
    if (event.key === 'Escape') {
      ipcRenderer.sendSync('alert-message', 'cancel');
    } else if (event.key === 'Enter') {
      ipcRenderer.sendSync('alert-message', 'ok');
    } else if (event.key === 'F12') {
      remote.getCurrentWindow().openDevTools();
    }
  });
});

ipcRenderer.on('init-message', (event, arg) => {
  ginfo.debug_message('confirm init renderer');
});
