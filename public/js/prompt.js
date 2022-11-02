const { ipcRenderer, remote } = require('electron');
var ginfo = remote.require('./src/ginfo');

$(function () {
  $('.btn-success').click(() => {
    const value = $('input').val();
    if (value) ipcRenderer.sendSync('prompt-message', { ok: true, data: value });
  });

  $('.btn-warning').click(() => {
    ipcRenderer.sendSync('prompt-message', { ok: false, data: {} });
  });

  $(document).on('keyup', (event) => {
    if (event.key === 'Escape') {
      ipcRenderer.sendSync('prompt-message', { ok: false, data: {} });
    } else if (event.key === 'Enter') {
      const value = $('input').val();
      if (value) ipcRenderer.sendSync('prompt-message', { ok: true, data: value });
    } else if (event.key === 'F12') {
      remote.getCurrentWindow().openDevTools();
    }
  });
});

ipcRenderer.on('init-message', (event, arg) => {
  ginfo.debug_message('prompt init renderer');
});
