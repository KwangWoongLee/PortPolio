const fs = require('fs');
const com = require('./src/com');
const ginfo = require('./src/ginfo');
const crypto = require('crypto');

let win;
let ps;
let code = {};

let user = {
  aidx: 0,
  session: '',
  name: '',
};
exports.init = function (w, p) {
  win = w;
  ps = p;

  code = com.load_code();
};

exports.get_login_user = function () {
  return user;
};

function get_send_data(packet_name) {
  const cmd = code.cmd[packet_name];
  let input = com.getClone(ps[packet_name]);
  input.cmd = cmd;

  if (packet_name == 'reg_id') {
    input.login_id = conf.login_id;
    input.uuid = conf.login_id;
    input.name = conf.login_id;
    // input.version = conf.version;

    if (input.login_type == 1) {
      input.login_id = com.get_login_id_hash(conf.login_id);
    }
  } else if (packet_name == 'login') {
    input.login_id = conf.login_id;
    input.uuid = conf.login_id;
    // input.version = conf.version;
    if (input.login_type == 1) {
      input.login_id = com.get_login_id_hash(conf.login_id);
    }
  } else if (packet_name == 'account_switching') {
    input.login_id = conf.login_id;
    input.name = conf.login_id;
    if (input.login_type == 1) {
      input.login_id = com.get_login_id_hash(conf.login_id);
    }
  }
  input.aidx = user.aidx;
  input.session = user.session;

  ginfo.console_message(`${packet_name} : send = ${JSON.stringify(input, null, 2)}`);
  return input;
}

function get_recv_data(recv) {
  const cmd = recv.cmd - 1;
  const data = recv.data;
  const status = recv.status;
  const packet_name = code.name[cmd];

  if (status != 200) {
    ginfo.console_message(`${packet_name} : recv error = ${JSON.stringify(recv, null, 2)}`);
    return false;
  }

  if (packet_name == 'login') {
    user.aidx = data.user_info.aidx;
    user.session = data.user_info.session;
    user.name = data.user_info.name;

    // ginfo.console_message(`login info = ${JSON.stringify(user, null, 2)}`);

    win.webContents.send('login-message', {
      aidx: user.aidx,
      session: user.session,
    });
  }
  ginfo.console_message(`${packet_name} : recv = ${JSON.stringify(recv, null, 2)}`);
  return true;
}

exports.send_packet = async function (packet_name) {
  try {
    const server = conf.server_list[conf.select_idx];
    const input = get_send_data(packet_name);
    const recv = await com.SendRequest(server.url, server.port, input);
    const ret = get_recv_data(recv);
    return ret;
  } catch (e) {
    ginfo.console_message(`${packet_name} : request error = ${e}`);
    return false;
  }
};
