const dateformat = require('dateformat');
const zlib = require('zlib');
const gzip = zlib.createGzip();
const crypto = require('crypto');
const http = require('http');
const { BrowserWindow, Menu } = require('electron');

// com brower
const alert = require('./alert');
const confirm = require('./confirm');
const prompt = require('./prompt');

const fs = require('fs');
const path = require('path');
let protocols = {};
let codes = {
  name: {},
  cmd: {},
};

exports.load_protocol = function () {
  try {
    const proto_list = fs.readdirSync(process.cwd() + '/protocol');
    for (var i = 0; i < proto_list.length; i++) {
      const str = proto_list[i];

      const proto_name = path.basename(str, '.json');
      const file = fs.readFileSync(process.cwd() + '/protocol/' + str);
      const obj = JSON.parse(file);

      protocols[proto_name] = obj;

      console.log(proto_name);
    }
  } catch (e) {
    console.log(e);
  }

  return { data: protocols };
};

exports.load_code = function () {
  const file = fs.readFileSync(process.cwd() + '/code.txt', 'utf8');
  const list = file.split('\n');
  const line_head = 'code.game_packet.';
  const line_tail = ';';
  for (let i = 0; i < list.length; i++) {
    const line = list[i];
    const line_head_idx = line.indexOf(line_head);
    if (line_head_idx == -1) continue;
    const line_tail_idx = line.indexOf(line_tail);
    if (line_tail_idx == -1) continue;

    const tmp1 = line.slice(line_head_idx + line_head.length, line_tail_idx);

    const line_list = tmp1.split('=');

    const packet_name = line_list[0].trim();
    const packet_cmd = Number(line_list[1].trim());
    codes.name[packet_cmd] = packet_name;
    codes.cmd[packet_name] = packet_cmd;
  }

  return codes;
};

exports.open_alert = function (title, msg) {
  return alert(title, msg);
};

exports.open_confirm = function (title, msg) {
  return confirm(title, msg);
};

exports.open_prompt = function (title, type, value, msg) {
  // type : text, number, password, date, time, datetime-local
  // ex time) const date = new Date(r.data); , ex etc) const data = r.data
  return prompt(title, type, value, msg);
};

exports.get_protocols = function () {
  return protocols;
};

exports.write_protocol_file = function (filename, value) {
  const fname = process.cwd() + '/protocol/' + filename;
  fs.writeFileSync(fname, value);
  const proto_name = path.basename(filename, '.json');
  protocols[proto_name] = JSON.parse(value);
  return proto_name;
};

exports.write_config_file = function () {
  const fname = process.cwd() + '/config.json';
  fs.writeFileSync(fname, JSON.stringify(conf, null, 2));
  console.log('save config file');
};

var keyset = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890';
var iv = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
var make_key = function () {
  var len = keyset.length - 1;

  var key = '';
  var AesKey = '';
  for (var i = 0; i < 4; i++) {
    var rand = Math.floor(Math.random() * len);
    key = key.concat(keyset.charAt(rand));
  }

  for (var i = 0; i < 8; i++) {
    AesKey = AesKey.concat(key);
  }

  return { key: key, AesKey: AesKey };
};
exports.zlib = zlib;

exports.gzip_uncompress = function (data) {
  var ret;
  try {
    var buf = new Buffer(data, 'base64');
    var dec = zlib.unzipSync(buf);
    ret = dec.toString();
  } catch (err) {
    if (err) {
      console.log('gzip_uncompress error(err=' + err + ')');
    }
  }

  return ret;
};
exports.gzip_compress = function (data) {
  var ret;
  try {
    var buf = zlib.deflateSync(data);
    ret = buf.toString('base64');
  } catch (err) {
    if (err) {
      console.log('gzip_compress error(err=' + err + ')');
    }
  }

  return ret;
};

exports.gzip_compress_crypt = function (data) {
  var ret;
  try {
    var key = make_key();
    var buf = zlib.deflateSync(data);
    var cipher = crypto.createCipheriv('aes-256-cbc', key.AesKey, Buffer.from(iv));
    var crypted = cipher.update(buf, 'utf8', 'base64');
    crypted += cipher.final('base64');

    ret = key.key.concat(crypted);
  } catch (err) {
    if (err) {
      console.log('gzip_compress_crypt error(err=' + err + ')');
    }
  }

  return ret;
};

exports.gzip_uncompress_crypt = function (data) {
  var ret;
  try {
    var key = data.slice(0, 4);
    var crypted = data.slice(4, data.length);

    for (var i = 0; i < 3; i++) {
      key = key.concat(key);
    }

    var decipher = crypto.createDecipheriv('aes-256-cbc', key, Buffer.from(iv));
    var start = decipher.update(crypted, 'base64');
    var end = decipher.final();
    var decrypted = Buffer.concat([start, end]);
    var dec = zlib.unzipSync(decrypted);
    ret = dec.toString();
  } catch (err) {
    if (err) {
      console.log('gzip_uncompress_crypt error : ' + err + ')');
    }
  }

  return ret;
};

exports.get_date_string = function (date) {
  return dateformat(new Date(), 'yyyymmddHHMMss');
};

exports.sleep = function (time) {
  return new Promise((resolve, reject) => {
    setTimeout(() => {
      resolve();
    }, time);
  });
};

exports.getType = function (obj) {
  const objectName = Object.prototype.toString.call(obj);
  const match = /\[object (\w+)\]/.exec(objectName);
  return match[1].toLowerCase();
};

exports.getClone = function (obj) {
  if (null == obj || 'object' != typeof obj) return obj;
  if (this.getType(obj) == 'date') {
    let copy = new Date();
    copy.setTime(obj.getTime());
    return copy;
  }
  if (this.getType(obj) == 'array') {
    let copy = [];
    for (let i = 0, len = obj.length; i < len; i++) {
      copy[i] = this.getClone(obj[i]);
    }
    return copy;
  }
  if (this.getType(obj) == 'object') {
    let copy = {};
    for (let attr in obj) {
      if (obj.hasOwnProperty(attr)) copy[attr] = this.getClone(obj[attr]);
    }
    return copy;
  }
  return obj;
};

exports.SendRequest = function (host, port, send_data) {
  const that = this;
  return new Promise((resolve, reject) => {
    let resData = '';
    const post_option = {
      host: host,
      port: port,
      method: 'POST',
    };
    const req = http.request(post_option, function (res) {
      res.on('data', function (chunk) {
        resData += chunk;
      });

      res.on('end', function () {
        resolve(JSON.parse(that.gzip_uncompress_crypt(resData)));
      });
    });

    req.on('error', function (err) {
      console.log(err.message);
      reject(err);
    });

    req.write(that.gzip_compress_crypt(JSON.stringify(send_data)));
    req.end();
  });
};

exports.get_login_id_hash = function (id) {
  const login_secret = 'Ga9lsf6bxShLilPAhwTG9LCcXC1ipX0lfju9ua7y';
  const data = `${login_secret}${id}`;
  const hash = crypto.createHash('sha256').update(data).digest('hex');
  return hash;
};

exports.create_child_brower = function (url, parent) {
  let child = new BrowserWindow({
    parent: parent,
    width: 1500,
    height: 1200,
    modal: true,
    show: false,
    title: '프리블록스',
    icon: _root_name + '/public/res/icon.png',
  });

  child.loadURL(url);
  child.show();
};

exports.current_contents = () => BrowserWindow.getFocusedWindow().webContents;

exports.ApplicationMenu = function () {
  const child_menu = Menu.buildFromTemplate([
    {
      label: '◀',
      click: () => {
        if (this.current_contents().canGoBack()) {
          this.current_contents().goBack();
        }
      },
    },
    {
      label: '▶',
      click: () => {
        if (this.current_contents().canGoForward()) {
          this.current_contents().goForward();
        }
      },
    },
    {
      label: '↺',
      accelerator: 'F5',
      click: () => this.current_contents().reload(),
    },
  ]);
  Menu.setApplicationMenu(child_menu);
};
