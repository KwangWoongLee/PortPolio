const { ipcRenderer, remote } = require('electron');
var ginfo = remote.require('./src/ginfo');
var json_edit = remote.require('./src/json_edit');
let protocols = {};
$(function () {
  window.old_filter = '';
  $('#filter').on('propertychange change keyup paste input', function () {
    const filter = $('#filter').val();
    if (window.old_filter != filter) {
      window.old_filter = filter;
      dir_filter(filter);
    }
  });

  $('.connect-open').click(() => {
    ipcRenderer.send('connect-open-message', {});
  });

  $('.packet-dir').on('click', '.element', function () {
    $('.send-packet').append('<div class="element">' + $(this).text() + '</div>');
  });

  $('.send-packet').on('click', '.element', function () {
    $(this).remove();
  });

  $('.send-packet , .packet-dir').on('contextmenu', '.element', function () {
    window.proto = $(this).text();
    packet_edit_view(JSON.stringify(protocols[proto], null, 2));
  });

  $('.msg-clear').click(() => {
    clear_msg();
  });

  $('#clear-packet-btn').click(() => {
    $('.send-packet > .element').remove();
  });

  $('#start-packet-btn').click(() => {
    ipcRenderer.send('packet-init-message', {});
  });

  $('#proto-edit-btn').click(() => {
    if (window.proto) {
      json_edit(window.proto + '.json', $('.packet-edit').text());
    }
  });

  $('#send-btn').click(() => {
    packet_send('send-message');
  });

  $('#send-infinity-btn').click(() => {
    packet_send('send-infinity-message');
  });
});

function write_msg(msg) {
  const msg_el = $('.message-console');
  msg_el.append(msg + '\n');
  msg_el.scrollTop(msg_el[0].scrollHeight);
}

function clear_msg() {
  $('.message-console').text('');
}

function packet_edit_view(msg) {
  $('#packet-edit-label').text(window.proto + '.json');
  $('.packet-edit').text('');
  $('.packet-edit').append(msg + '\n');
}

// ----------- message listner ----------------

ipcRenderer.on('init-message', (event, arg) => {
  protocols = arg.protocols;
  for (let name in protocols) {
    $('.packet-dir').append('<div class="element">' + name + '</div>');
  }
  ginfo.debug_message('main init renderer');
});

ipcRenderer.on('console-message', (event, arg) => {
  write_msg(arg);
});

ipcRenderer.on('json-edit-file-message', (event, arg) => {
  window.proto = arg.title;
  packet_edit_view(arg.str);
  protocols[arg.title] = JSON.parse(arg.str);
});

ipcRenderer.on('init-packet-message', (event, arg) => {
  $('.send-packet > .element').remove();
  for (let i = 0; i < arg.length; i++) {
    $('.send-packet').append('<div class="element">' + arg[i] + '</div>');
  }
});

ipcRenderer.on('send_complate', (event, arg) => {
  $('#send-btn').attr('disabled', false);
  $('#send-infinity-btn').attr('disabled', false);
});

ipcRenderer.on('login-message', (event, arg) => {
  $('#session-txt').val('session : ' + arg.session);
  $('#aidx-txt').val('accidx : ' + arg.aidx);
});

// ----------- function ----------------------

function packet_send(message_channel) {
  let list = [];
  for (let i = 0; i < $('.send-packet > .element').length; i++) {
    list.push($('.send-packet > .element').eq(i).text());
  }

  if (list.length) {
    $('#send-btn').attr('disabled', true);
    $('#send-infinity-btn').attr('disabled', true);
    ipcRenderer.send(message_channel, list);
  }
}

function dir_filter(filter) {
  const d = $('.packet-dir > .element');
  if (filter == '') {
    d.css('display', 'block');
    return;
  }

  for (let i = 0; i < d.length; i++) {
    const obj = d.eq(i);
    const txt = obj.text();
    if (txt.indexOf(filter) == -1) obj.css('display', 'none');
    else obj.css('display', 'block');
  }
}
