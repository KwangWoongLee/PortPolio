const { ipcRenderer, remote } = require('electron');
var ginfo = remote.require('./src/ginfo');

$(function () {
  $('button').click(() => {
    send_json_edit_file_message();
  });

  $(document).on('keyup', (event) => {
    if (event.key === 'Escape') {
      remote.getCurrentWindow().close();
    } else if (event.key === 'Enter') {
      send_json_edit_file_message();
    } else if (event.key === 'F12') {
      remote.getCurrentWindow().openDevTools();
    }
  });
});

ipcRenderer.on('init-message', (event, arg) => {
  window.title = arg;
  ginfo.debug_message('json_edit init renderer');
});

function send_json_edit_file_message() {
  if (window.title) {
    ipcRenderer.sendSync('json-edit-file-message', json_edit_file_message());
    remote.getCurrentWindow().close();
  }
}

function json_edit_file_message() {
  ginfo.debug_message('renderer: req_json_edit_file : ' + window.title);
  ginfo.debug_message($('.json-edit').val());
  return {
    filename: window.title,
    value: $('.json-edit').val(),
  };
}
