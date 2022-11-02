'use strict';

const com = require('com');
const mysql = require('db/mysql');
const logger = require('com/log');

let data = {};

exports.load = async function (mod) {
  const val = {};
  try {
    await mysql.comdb_connect(val);
    await select_data();
  } catch (e) {
    val.err = e;
  }

  mysql.release(val, val.err);

  logger.info('load item reference table ' + (val.err ? 'fail : ' + val.err : 'success !!'));

  return val.err;

  function select_data() {
    return new Promise(function (resolve, reject) {
      const temp_data = {};

      const query = com.make_query(
        [
          'select code, description from ref_item;',
        ],
        []
      );

      val.comdb.query(query, [], function (err, results) {
        if (!err) {
          for (const info of results) {
            temp_data[info.code] = info;
          }
        }

        if (err) reject(err);
        else {
          data = temp_data;
          resolve();
        }
      });
    });
  }

};

exports.get = function (item_code) {
  return data[item_code];
};

exports.get_data = function () {
  return data;
};
