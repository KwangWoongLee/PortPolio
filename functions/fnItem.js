'use strict';

const ref = require('reference');
const com = require('com');
const ginfo = require('com/ginfo');
const { error } = require('com/code');
const mysql = require('db/mysql');
const redis = require('db/redis');
const sprintf = require('sprintf');

exports.get_item_list = function (userdb, aidx){
  return new Promise(function (resolve, reject) {
   const query = 'select idx, item_code, cnt from dat_item where aidx = ? and item_code != ?';

    mysql
      .query(userdb, query, [aidx, ginfo.exp_code])
      .then((results) => results.length ? resolve(results) : reject(error.not_found_item_list))
      .catch((e) => reject(e));
  });
}

exports.get_exp = function (userdb, aidx){
  return new Promise(function (resolve, reject) {
   const query = 'select cnt as exp from dat_item where aidx = ? and item_code = ?';

    mysql
      .query(userdb, query, [aidx, ginfo.exp_code])
      .then((results) => results.length ? resolve(results[0]) : resolve(error.not_found_my_exp))
      .catch((e) => reject(e));
  });
}

exports.get_item = function (userdb, aidx, item_idx){
  return new Promise(function (resolve, reject) {
   const query = 'select item_code, cnt from dat_item where idx = ? and aidx = ?';

    mysql
      .query(userdb, query, [item_idx, aidx])
      .then((results) => results.length ? resolve(results[0]) : reject(error.not_found_item))
      .catch((e) => reject(e));
  });
}