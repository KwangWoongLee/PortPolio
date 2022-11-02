const { ipcRenderer, remote } = require('electron');
var ginfo = remote.require('./src/ginfo');

$(function () {
  $('.btn').click(() => {
    ipcRenderer.sendSync('close-message', select_server_message());
  });

  $(document).on('keyup', (event) => {
    if (event.key === 'Enter' || event.key === 'Escape') {
      ipcRenderer.sendSync('close-message', select_server_message());
    } else if (event.key === 'F12') {
      remote.getCurrentWindow().openDevTools();
    }
  });

  $('select').change(() => {
    ipcRenderer.sendSync('change-message', select_server_message());
  });
});

ipcRenderer.on('init-message', (event, arg) => {
  ginfo.debug_message('connector_selector init renderer');
});

function select_server_message() {
  return {
    select_idx: Number($('select option:selected').attr('idx')),
    login_id: $('#login-id').val(),
    version: $('#version').val(),
    reg_id: $('input:checkbox[id="reg-id"]').is(':checked'),
  };
}
