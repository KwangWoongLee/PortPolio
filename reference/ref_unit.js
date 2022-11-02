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

  logger.info('load unit reference table ' + (val.err ? 'fail : ' + val.err : 'success !!'));

  return val.err;

  function select_data() {
    return new Promise(function (resolve, reject) {
      const temp_data = {};

      const query = com.make_query(
        [
          'select code, name, exchange_item_code, skill01, skill02, skill03, skill04, skill05, skill06, skill07, skill08, skill09 from ref_unit;',
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

exports.get = function (unit_code) {
  return data[unit_code];
};


exports.get_data = function () {
  return data;
};
