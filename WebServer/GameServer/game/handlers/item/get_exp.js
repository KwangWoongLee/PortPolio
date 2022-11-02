'use strict';

const fnItem = require('functions/fnItem');
const fnLog = require('functions/fnLog');
const conf = require('game/config');
const com = require('com');
const session = require('com/session');
const { error } = require('com/code');
const mysql = require('db/mysql');
const ginfo = require('com/ginfo');
const redis = require('db/redis');

module.exports = async function (input, callback) {
  const ret = {};
  const val = {};

  com.packet_start(ret, val, input, error.none);

  // main process
  try {
    const userdb = await mysql.userdb_connect(val);
    let { exp } = await fnItem.get_exp(userdb, input.aidx);
    if (!exp) exp = 0;
    // val.log = fnLog.new_log(input.cmd, aidx);
    // val.log.add('login', [login_id]);

    ret.data.exp = exp;
  } catch (e) {
    val.err = e;
  }

  // packet end
  com.packet_end(ret, val, val.err, callback);
};
