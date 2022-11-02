'use strict';

const fnUser = require('functions/fnUser');
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
    const comdb = await mysql.comdb_connect(val);
    await fnUser.set_agreement(comdb, val.aidx, input.type);

    // val.log = fnLog.new_log(input.cmd, aidx);
    // val.log.add('login', [login_id]);
  } catch (e) {
    val.err = e;
  }

  // packet end
  com.packet_end(ret, val, val.err, callback);
};
