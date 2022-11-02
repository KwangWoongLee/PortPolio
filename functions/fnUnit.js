'use strict';

const ref = require('reference');
const com = require('com');
const ginfo = require('com/ginfo');
const { error } = require('com/code');
const mysql = require('db/mysql');
const redis = require('db/redis');
const sprintf = require('sprintf');

exports.get_unit_list = function (userdb, aidx){
  return new Promise(function (resolve, reject) {
   const query = 'select unit_code from dat_unit where aidx = ?';

    mysql
      .query(userdb, query, [aidx])
      .then((results) => results.length ? resolve(results) : reject(error.not_found_unit_list))
      .catch((e) => reject(e));
  });
}

exports.exchange_unit = async function (userdb, aidx, unit_code, item_idx){
  await update_item(userdb, item_idx);
  await insert_unit(userdb, aidx, unit_code);

  function update_item(userdb, item_idx){
    const query = 'update dat_item set cnt = cnt - ? where idx = ?;';
 
     return mysql.query(userdb, query, [ginfo.exchange_maeum_cnt, item_idx]);
  }

  function insert_unit(userdb, aidx, unit_code){
      const query = 'insert into dat_unit values(?, ?);';
  
      return mysql.query(userdb, query, [aidx, unit_code]);
  }
  
}