'use strict';

const fnUnit = require('functions/fnUnit');
const fnItem = require('functions/fnItem');
const fnLog = require('functions/fnLog');
const conf = require('game/config');
const com = require('com');
const session = require('com/session');
const { error } = require('com/code');
const mysql = require('db/mysql');
const ginfo = require('com/ginfo');
const redis = require('db/redis');
const ref = require('reference');


module.exports = async function (input, callback) {
  const ret = {};
  const val = {};

  com.packet_start(ret, val, input, error.none);

  // main process
  try {
    const userdb = await mysql.userdb_connect(val);
    const {item_code, cnt} = await fnItem.get_item(userdb, input.aidx, input.item_idx);
    const unit_data = ref.unit.get_data();
    
    const unit = _.find(unit_data, {exchange_item_code : item_code});
    if(!unit) throw error.not_found_unit_item;
    
    if(cnt < ginfo.exchange_maeum_cnt) throw error.deficient_item;
    
    await mysql.userdb_transaction(val);
    await fnUnit.exchange_unit(userdb, input.aidx, unit.code, input.item_idx);

    // val.log = fnLog.new_log(input.cmd, aidx);
    // val.log.add('login', [login_id]);

  } catch (e) {
    val.err = e;
  }

  // packet end
  com.packet_end(ret, val, val.err, callback);
};
